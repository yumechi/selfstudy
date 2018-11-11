#include<iostream>
#include<vector>
#include<string>
#include<curl/curl.h>

using namespace std;

size_t onReceive(char* ptr, size_t size, size_t nmemb, string* stream) {
    const size_t sizes = size * nmemb;
    stream->append(ptr, sizes);
    return sizes;
}

int main(int argc, const char* argv[]) {
    CURL *curl = curl_easy_init();
    if(curl == nullptr) {
        cerr << "curl init error" << endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    string responceData;

    // setup
    curl_easy_setopt(curl, CURLOPT_URL, "https://supporterzcolab.com/api/v1/event/?event_id=586");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responceData);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        return 1;
    }
 
    cout << responceData << endl;

    return 0;
}