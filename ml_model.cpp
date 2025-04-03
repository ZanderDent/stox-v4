#include "ml_model.h"

int predictNextDay(const arma::mat& latest, mlpack::LogisticRegression<>& model) {
    arma::Row<size_t> pred;
    model.Classify(latest, pred);
    return pred[0];
}