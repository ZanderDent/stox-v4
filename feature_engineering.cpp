#include "feature_engineering.h"

double calculateSMA(const std::vector<double>& prices, int period, int endIndex) {
    if (endIndex < period - 1) return -1.0;
    double sum = 0.0;
    for (int i = endIndex - period + 1; i <= endIndex; ++i)
        sum += prices[i];
    return sum / period;
}

void prepareTrainingData(const std::vector<double>& prices, arma::mat& features, arma::Row<size_t>& labels) {
    int n = prices.size();
    int samples = n - 21; // ensure there are enough data points for SMA20 and next-day label
    features.set_size(8, samples);
    labels.set_size(samples);

    for (int i = 20; i < n - 1; ++i) {
        int col = i - 20;
        // Last 5 prices: indices i-4 to i
        for (int j = 0; j < 5; ++j)
            features(j, col) = prices[i - 4 + j];
        features(5, col) = calculateSMA(prices, 5, i);
        features(6, col) = calculateSMA(prices, 20, i);
        features(7, col) = prices[i + 1] - prices[i];  // delta
        labels(col) = (prices[i + 1] > prices[i]) ? 1 : 0;
    }
}
