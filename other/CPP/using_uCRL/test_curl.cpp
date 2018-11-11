#include<iostream>
#include<vector>
#include<string>
#include<cassert>
#include "unistd.h"

#include<curl/curl.h>

#include "picojson.h"

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
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responceData);

    CURLcode res = curl_easy_perform(curl);
    sleep(5);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        return 1;
    }
    if(responceData.length() == 0) {
        cerr << "respnceData is null, maybe timeout" << endl;
        cerr << "responceData: " << responceData << endl;
        return 0;
    }

    // debug 
    cerr << responceData.length() << endl;

    picojson::value json_val;
    string err_check = picojson::parse(json_val, responceData);
    if (!err_check.empty()) {
        cerr << "json parse error: " << err_check << endl;
        return 1;
    }

    picojson::object& obj = json_val.get<picojson::object>();
    picojson::array& events = obj["events"].get<picojson::array>();
    string title, started_at;
    for(const auto& event : events) {
        title = event.get<picojson::object>().at("title").get<string>();
        started_at = event.get<picojson::object>().at("started_at").get<string>();
    }
    cout << title << endl;
    cout << started_at << endl;

    return 0;
}
