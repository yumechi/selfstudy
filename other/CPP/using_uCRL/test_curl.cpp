#include<iostream>
#include<vector>
#include<string>
#include<cassert>
#include "unistd.h"
#include<curl/curl.h>

#include "picojson.h"

size_t onReceive(char* ptr, size_t size, size_t nmemb, std::string* stream) {
    const size_t sizes = size * nmemb;
    stream->append(ptr, sizes);
    return sizes;
}

std::string get_responce() {
    CURL *curl = curl_easy_init();
    if(curl == nullptr) {
        std::cerr << "curl init error" << std::endl;
        curl_easy_cleanup(curl);
        exit(1);
    }

    std::string ret;
    // setup
    curl_easy_setopt(curl, CURLOPT_URL, "https://supporterzcolab.com/api/v1/event/?event_id=586");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);

    CURLcode res = curl_easy_perform(curl);
    // using sleep time, Succeed
    sleep(5);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    if(ret.length() == 0) {
        std::cerr << "respnceData is null, maybe timeout" << std::endl;
        std::cerr << "responceData: " << ret << std::endl;
        exit(1);
    }
    return ret;
}

int main(int argc, const char* argv[]) {
    // debug 
    std::string responce = get_responce();
#if DEBUG
    std::cerr << responce.length() << std::endl;
#endif
    picojson::value json_val;
    std::string err_check = picojson::parse(json_val, responce);
    if (!err_check.empty()) {
        std::cerr << "json parse error: " << err_check << std::endl;
        return 1;
    }

    picojson::object& obj = json_val.get<picojson::object>();
    picojson::array& events = obj["events"].get<picojson::array>();
    std::string title, started_at;
    for(const auto& event : events) {
        title = event.get<picojson::object>().at("title").get<std::string>();
        started_at = event.get<picojson::object>().at("started_at").get<std::string>();
    }
    std::cout << title << std::endl;
    std::cout << started_at << std::endl;

    return 0;
}
