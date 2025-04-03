#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Your Alpha Vantage API key (change as needed)
extern const std::string apiKey;

// Functions to fetch and parse API data
std::string fetchDataFromAPI(const std::string& url);
std::vector<double> parseClosePrices(const std::string& jsonResponse);

#endif // DATA_FETCHER_H
