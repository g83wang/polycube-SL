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

using namespace std;

int create_archive(const string& dir_path, const string& archive_path) {
    string cmd = "tar -czvf " + archive_path + " " + dir_path + " 1> /dev/null";
    //cout << cmd << endl;
    system(cmd.c_str());
    return 0;
}

int upload_file(const string& target_url, const string& archive_path) {
    CURL *curl = curl_easy_init();

    struct curl_httppost *firstitem = NULL;
    struct curl_httppost *lastitem = NULL;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "new_service");

    curl_formadd(&firstitem, &lastitem, CURLFORM_COPYNAME, "file", CURLFORM_FILENAME, archive_path.c_str(), CURLFORM_FILE, archive_path.c_str(), CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_URL, target_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, firstitem);
    int result = curl_easy_perform(curl);
    cout << result << endl;
    curl_easy_cleanup(curl);

    return 0;
}

int main(int argc, char *argv[]) {
    string endpoint_url = "http://localhost:9000/polycube/v1";
    string dir_path = argv[1];

    string archive_path = "/home/g83wang/polycube-SL/client/archive_directory/my_archive.tar.gz";
    create_archive(dir_path, archive_path);
    upload_file(endpoint_url, archive_path);

    remove(archive_path.c_str());

    return 0;
}


