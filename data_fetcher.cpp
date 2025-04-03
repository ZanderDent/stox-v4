#include "data_fetcher.h"
#include <iostream>
#include <curl/curl.h>

// Define the API key here.
const std::string apiKey = "9YG822AWWD14JNTT";

typedef std::vector<char> MemoryStruct;
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    MemoryStruct* mem = static_cast<MemoryStruct*>(userp);
    char* data = static_cast<char*>(contents);
    mem->insert(mem->end(), data, data + realsize);
    return realsize;
}

std::string fetchDataFromAPI(const std::string& url) {
    CURL* curl = curl_easy_init();
    MemoryStruct buffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "[ERROR] CURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return std::string(buffer.begin(), buffer.end());
}

std::vector<double> parseClosePrices(const std::string& jsonResponse) {
    std::vector<double> closes;
    try {
        auto data = nlohmann::json::parse(jsonResponse);
        for (auto& [date, values] : data["Time Series (Daily)"].items()) {
            closes.push_back(std::stod(values["4. close"].get<std::string>()));
        }
        std::reverse(closes.begin(), closes.end());
    } catch (std::exception& e) {
        std::cerr << "[ERROR] JSON parse failed: " << e.what() << std::endl;
    }
    return closes;
}
