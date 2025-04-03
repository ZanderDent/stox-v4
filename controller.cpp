#include "controller.h"
#include <mlpack/methods/logistic_regression/logistic_regression.hpp>

bool runAnalysis(const std::string& symbol, std::vector<double>& prices, std::vector<int>& buySignals) {
    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=" + apiKey;
    std::string response = fetchDataFromAPI(url);
    prices = parseClosePrices(response);

    if (prices.empty() || prices.size() < 50 || response.find("Error Message") != std::string::npos) {
        return false;
    }

    arma::mat features;
    arma::Row<size_t> labels;
    prepareTrainingData(prices, features, labels);

    mlpack::LogisticRegression<> model(features, labels);

    arma::mat latest(8, 1);
    int last = prices.size() - 2;
    for (int j = 0; j < 5; ++j)
        latest(j, 0) = prices[last - 4 + j];
    latest(5, 0) = calculateSMA(prices, 5, last);
    latest(6, 0) = calculateSMA(prices, 20, last);
    latest(7, 0) = prices[last + 1] - prices[last];

    int signal = predictNextDay(latest, model);

    buySignals.assign(prices.size(), 0); // Single assignment instead of resize
    buySignals[prices.size() - 2] = signal;

    return true;
}