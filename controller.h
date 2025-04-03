#include "controller.h"
// Add a new parameter “source” – if "sec", then use SEC EDGAR.

bool runAnalysis(const std::string& symbol, const std::string& source, std::vector<double>& prices, std::vector<int>& buySignals) {
    std::string response;
    if(source == "sec") {
        // Map ticker symbol to CIK (you might need a lookup table)
        // For demonstration, assume "AAPL" -> "0000320193"
        std::string cik = (symbol == "AAPL") ? "0000320193" : "";
        if(cik.empty()) return false;
        response = fetchDataFromSEC(cik);
        // You must then parse the SEC JSON format to extract price data.
        // (This may differ from Alpha Vantage – you might need a new parser.)
        // For now, you could reuse parseClosePrices if the JSON structure is similar.
        prices = parseClosePrices(response);
    } else {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=" + apiKey;
        response = fetchDataFromAPI(url);
        prices = parseClosePrices(response);
    }
    
    if(prices.size() < 50) {
        return false;
    }
    arma::mat features;
    arma::Row<size_t> labels;
    prepareTrainingData(prices, features, labels);

    mlpack::LogisticRegression<> model(features, labels);

    arma::mat latest(8, 1);
    int last = prices.size() - 2;
    for(int j = 0; j < 5; ++j)
        latest(j, 0) = prices[last - 4 + j];
    latest(5, 0) = calculateSMA(prices, 5, last);
    latest(6, 0) = calculateSMA(prices, 20, last);
    latest(7, 0) = prices[last + 1] - prices[last];

    int signal = predictNextDay(latest, model);
    buySignals.assign(prices.size(), 0);
    buySignals[prices.size()-2] = signal;
    return true;
}
