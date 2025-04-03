#ifndef FEATURE_ENGINEERING_H
#define FEATURE_ENGINEERING_H

#include <vector>
#include <armadillo>

// Calculate Simple Moving Average (SMA)
double calculateSMA(const std::vector<double>& prices, int period, int endIndex);

// Prepare features and labels for ML training
void prepareTrainingData(const std::vector<double>& prices, arma::mat& features, arma::Row<size_t>& labels);

#endif // FEATURE_ENGINEERING_H
