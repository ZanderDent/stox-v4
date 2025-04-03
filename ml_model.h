#ifndef ML_MODEL_H
#define ML_MODEL_H

#include <armadillo>
#include <mlpack/methods/logistic_regression/logistic_regression.hpp>

int predictNextDay(const arma::mat& latest, mlpack::LogisticRegression<>& model);

#endif // ML_MODEL_H