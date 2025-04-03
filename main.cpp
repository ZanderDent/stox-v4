#include <iostream>
#include <vector>
#include "data_fetcher.h"
#include "feature_engineering.h"
#include "ml_model.h"
#include "viewer.h"
#include "controller.h"
#include <QApplication>
#include <armadillo>
#include <mlpack/methods/logistic_regression/logistic_regression.hpp>

int main(int argc, char *argv[]) {
    std::string symbol = "AAPL";
    std::string source = "alpha";  // default to Alpha Vantage

    // Simple command-line parsing:
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg == "--symbol" && i+1 < argc) {
            symbol = argv[++i];
        } else if(arg == "--source" && i+1 < argc) {
            source = argv[++i];
        }
    }

    // Here you could choose between a GUI or backtest mode.
    QApplication app(argc, argv);
    StockViewer viewer;  // In a full version, pass symbol/source to viewer or controller.
    // (Viewer could later call runAnalysis and update the chart.)
    viewer.resize(900, 700);
    viewer.show();
    return app.exec();
}
