#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>
#include <cpprest/producerconsumerstream.h>
#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <archive.h>
#include <archive_entry.h>


using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


void extract(const string& archive_path, const  string& destination) {
    struct archive* a = archive_read_new();
    struct archive* ext = archive_write_disk_new();
    struct archive_entry* entry;
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;

    archive_read_support_filter_gzip(a);
    archive_read_support_format_tar(a);
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    archive_read_open_filename(a, archive_path.c_str(), 10240);

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        string dest_path = destination + "/" + archive_entry_pathname(entry);
        archive_entry_set_pathname(entry, dest_path.c_str());
        archive_write_header(ext, entry);
        const void* buff;
        size_t size;
        int64_t offset;

        while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
            archive_write_data_block(ext, buff, size, offset);
        }

        archive_write_finish_entry(ext);
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
}

class Server {
public:
    Server() {}
    Server(utility::string_t url);

    void handle_request(http_request message);
    void initialize(const string_t& address);
    void shutdown();

private:
    http_listener m_listener;
    concurrency::streams::file_buffer<uint8_t> file_buffer() const;
};

Server::Server(utility::string_t url) : m_listener(url) {
    m_listener.support(methods::POST,  bind(&Server::handle_request, this,  placeholders::_1));
}

concurrency::streams::file_buffer<uint8_t> Server::file_buffer() const {
    return concurrency::streams::file_buffer<uint8_t>();
}
int d = 0;
void Server::handle_request(http_request message) {
    auto path = message.relative_uri().path();
    if (path == U("/polycube/v1")) {
        if (message.method() == methods::POST) {
            try {
                auto file_name_itr = message.headers().find("file_name");
                auto file_name = file_name_itr->second;
                auto file_path = "received_files/" + utility::conversions::to_utf8string(file_name);
                message.extract_vector().then([=]( vector<unsigned char> file_data) {
                    ofstream out(file_path,  ios::out |  ios::binary);
                    out.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());
                    out.close();
                    string extracted_dir = "extracted_files/";
                    extract(file_path, extracted_dir);

                    message.reply(status_codes::OK, "File received and extracted");
                }).wait();
            }
            catch (const http_exception& e) {
                cerr << "Error request handling: " << e.what() <<  endl;
                message.reply(status_codes::InternalError, "Internal server error");
            }
        } else {
            message.reply(status_codes::BadRequest, "NoT POST.");
        }
    } else {
        message.reply(status_codes::NotFound, "Invalid path");
    }
}


void Server::initialize(const string_t& address) {
    uri_builder uri(address);
    auto addr = uri.to_uri().to_string();
    m_listener = http_listener(addr);
    m_listener.support(methods::POST, bind(&Server::handle_request, this, placeholders::_1));
    try {
        m_listener
            .open()
            .then([&]() { cout << "Starting to listen at: " << addr << endl; })
            .wait();

        cout << "File extraction server is running" << endl;
    }
    catch (const exception& e) {
        cerr << "Error initializing the server: " << e.what() << endl;
    }
}

void Server::shutdown() {
    m_listener
        .close()
        .then([&]() { cout << "stop listening" << endl; })
        .wait();
}

int main() {
    utility::string_t address = U("http://localhost:8000");
    Server server;

    server.initialize(address);

    cout << "Press ENTER to exit" << endl;
    string line;
    getline(cin, line);

    server.shutdown();

    return 0;
}
