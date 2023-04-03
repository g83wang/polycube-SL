#include </home/g83wang/polycube-SL/server/Crow/include/crow/crow.h>
#include <iostream>
#include <fstream>
#include <archive.h>
#include <archive_entry.h>

bool extract_archive(const std::string &archive_path, const std::string &destination) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags = ARCHIVE_EXTRACT_TIME;
    int result;

    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_filter_gzip(a);
    result = archive_read_open_filename(a, archive_path.c_str(), 10240);

    if (result != ARCHIVE_OK) {
        return false;
    }

    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    while (true) {
        result = archive_read_next_header(a, &entry);
        if (result == ARCHIVE_EOF) {
            break;
        }

        if (result != ARCHIVE_OK) {
            return false;
        }

        result = archive_write_header(ext, entry);
        if (result != ARCHIVE_OK) {
            return false;
        }

        const void *buff;
        size_t size;
        int64_t offset;

        while (true) {
            result = archive_read_data_block(a, &buff, &size, &offset);
            if (result == ARCHIVE_EOF) {
                break;
            }

            if (result != ARCHIVE_OK) {
                return false;
            }

            result = archive_write_data_block(ext, buff, size, offset);
            if (result != ARCHIVE_OK) {
                return false;
            }
        }

        result = archive_write_finish_entry(ext);
        if (result != ARCHIVE_OK) {
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    return true;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/polycube/v1")
        .methods("POST"_method)
    ([](const crow::request &req) {
        auto it = req.files.find("file");
        if (it == req.files.end()) {
            return crow::response(400, "File not found");
        }

        const crow::multipart::file &file = it->second;
        std::string archive_path = "uploaded_archive.tar.gz";
        std::ofstream ofs(archive_path, std::ios::binary);
        ofs.write(file.data.data(), file.data.size());
        ofs.close();

        std::string destination = "extracted_files";
        if (!extract_archive(archive_path, destination)) {
            return crow::response(500, "Failed to extract archive");
        }

        std::remove(archive_path.c_str());
        return crow::response(200, "Archive extracted successfully");
    });

    app.port(9000).run();
}