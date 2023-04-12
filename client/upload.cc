#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <sstream>
#include <archive.h>
#include <archive_entry.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;

int create_archive(const string& dir_path, const string& archive_path) {
    string cmd = "tar -czvf " + archive_path + " " + dir_path + " 1> /dev/null";
    //cout << cmd << endl;
    system(cmd.c_str());
    return 0;
}

//string get_file_name(const string& path) {
//    size_t pos = path.find_last_of("/\\");
//    return (pos == string::npos) ? path : path.substr(pos + 1);
//}

int upload_file(const string& target_url, const string& archive_path, const string& prog_name) {
    web::http::client::http_client client(target_url);
    web::http::http_request request(web::http::methods::POST);
    request.headers().add("file_name", prog_name);

    auto file_stream = concurrency::streams::file_stream<uint8_t>::open_istream(archive_path).get();
    request.set_body(file_stream.streambuf());

    try {
        auto response = client.request(request).get();
        if (response.status_code() == web::http::status_codes::OK) {
            std::cout << "File uploaded successfully" << std::endl;
        } else {
            std::cout << "Failed to upload the file. HTTP status: " << response.status_code() << std::endl;
        }
    } catch (const web::http::http_exception& e) {
        std::cerr << "Error while uploading the file: " << e.what() << std::endl;
    }

    file_stream.close().wait();

    return 0;
}


int main(int argc, char *argv[]) {
    string endpoint_url = "http://localhost:8000/polycube/v1";
    string dir_path = argv[1];
    string prog_name = argv[2];
    string archive_path = "./archive_directory/" + prog_name+ ".tar.gz";
    cout << archive_path << endl;
    create_archive(dir_path, archive_path);
    upload_file(endpoint_url, archive_path, prog_name);

    //remove(archive_path.c_str());

    return 0;
}


