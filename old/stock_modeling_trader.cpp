// stock_modeling_trader.cpp
// Qt6 + mlpack + Armadillo stock modeling with GUI signals

#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <armadillo>
#include <mlpack/core.hpp>
#include <mlpack/methods/logistic_regression/logistic_regression.hpp>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

using json = nlohmann::json;
using namespace nlohmann; // Added for json parsing
using namespace mlpack; // Changed from mlpack::regression

// Alpha Vantage API Key
const std::string apiKey = "9YG822AWWD14JNTT";

// Helper for curl
typedef std::vector<char> MemoryStruct;
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    MemoryStruct* mem = (MemoryStruct*)userp;
    char* data = (char*)contents;
    mem->insert(mem->end(), data, data + realsize);
    return realsize;
}

std::string fetchDataFromAPI(const std::string& url) {
    CURL* curl;
    CURLcode res;
    MemoryStruct buffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
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

double calculateSMA(const std::vector<double>& prices, int period, int endIndex) {
    if (endIndex < period - 1) return -1.0;
    double sum = 0.0;
    for (int i = endIndex - period + 1; i <= endIndex; ++i)
        sum += prices[i];
    return sum / period;
}

void prepareTrainingData(const std::vector<double>& prices, arma::mat& features, arma::Row<size_t>& labels) {
    int n = prices.size();
    int samples = n - 21;
    features.set_size(8, samples);
    labels.set_size(samples);

    for (int i = 20; i < n - 1; ++i) {
        for (int j = 0; j < 5; ++j)
            features(j, i - 20) = prices[i - 4 + j];
        features(5, i - 20) = calculateSMA(prices, 5, i);
        features(6, i - 20) = calculateSMA(prices, 20, i);
        features(7, i - 20) = prices[i + 1] - prices[i];
        labels(i - 20) = (prices[i + 1] > prices[i]) ? 1 : 0;
    }
}

int predictNextDay(const arma::mat& latest, mlpack::LogisticRegression<>& model) { // Updated
    arma::Row<size_t> pred;
    model.Classify(latest, pred);
    return pred[0];
}

class StockViewer : public QMainWindow {
public:
    StockViewer(const std::vector<double>& prices, const std::vector<int>& buySignals) {
        QLineSeries* series = new QLineSeries();
        QScatterSeries* buys = new QScatterSeries();
        buys->setColor(Qt::green);
        buys->setMarkerSize(10);

        for (int i = 0; i < prices.size(); ++i) {
            series->append(i, prices[i]);
            if (i < buySignals.size() && buySignals[i] == 1)
                buys->append(i, prices[i]);
        }

        QChart* chart = new QChart();
        chart->addSeries(series);
        chart->addSeries(buys);
        chart->createDefaultAxes();
        chart->setTitle("Stock Price + ML Buy Signal");

        QChartView* view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    }
};

int main(int argc, char* argv[]) {
    const std::string symbol = "AAPL";
    const std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=" + apiKey;
    std::string response = fetchDataFromAPI(url);
    std::vector<double> prices = parseClosePrices(response);

    if (prices.size() < 50) {
        std::cerr << "[ERROR] Not enough data.\n";
        return 1;
    }

    arma::mat features;
    arma::Row<size_t> labels;
    prepareTrainingData(prices, features, labels);

    mlpack::LogisticRegression<> model(features, labels); // Updated

    arma::mat latest(8, 1);
    int last = prices.size() - 2;
    for (int j = 0; j < 5; ++j)
        latest(j, 0) = prices[last - 4 + j];
    latest(5, 0) = calculateSMA(prices, 5, last);
    latest(6, 0) = calculateSMA(prices, 20, last);
    latest(7, 0) = prices[last + 1] - prices[last];

    int signal = predictNextDay(latest, model);

    std::vector<int> buySignals(prices.size(), 0);
    buySignals[prices.size() - 2] = signal;

    QApplication app(argc, argv);
    StockViewer viewer(prices, buySignals);
    viewer.resize(800, 600);
    viewer.show();

    return app.exec();
}