#include "viewer.h"
#include "controller.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

QT_CHARTS_USE_NAMESPACE

StockViewer::StockViewer(QWidget* parent) : QMainWindow(parent) {
    QWidget* central = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(central);

    QHBoxLayout* topBar = new QHBoxLayout();
    tickerInput = new QLineEdit();
    QPushButton* loadButton = new QPushButton("Load");
    statusLabel = new QLabel("Enter a ticker and click Load");
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);

    topBar->addWidget(new QLabel("Ticker:"));
    topBar->addWidget(tickerInput);
    topBar->addWidget(loadButton);

    layout->addLayout(topBar);
    layout->addWidget(statusLabel);
    layout->addWidget(chartView);
    setCentralWidget(central);

    connect(loadButton, &QPushButton::clicked, this, &StockViewer::loadTicker);
}

void StockViewer::loadTicker() {
    std::string symbol = tickerInput->text().toStdString();
    statusLabel->setText("Loading " + QString::fromStdString(symbol) + "...");

    std::vector<double> prices;
    std::vector<int> buySignals;

    bool ok = runAnalysis(symbol, prices, buySignals);
    if (!ok) {
        statusLabel->setText("Failed to fetch or process data for " + QString::fromStdString(symbol));
        return;
    }

    updateChart(prices, buySignals);
    statusLabel->setText("Loaded: " + QString::fromStdString(symbol));
}

void StockViewer::updateChart(const std::vector<double>& prices, const std::vector<int>& buySignals) {
    QLineSeries* series = new QLineSeries();
    QScatterSeries* buys = new QScatterSeries();
    buys->setColor(Qt::green);
    buys->setMarkerSize(10);

    for (size_t i = 0; i < prices.size(); ++i) {
        series->append(i, prices[i]);
        if (i < buySignals.size() && buySignals[i] == 1)
            buys->append(i, prices[i]);
    }

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->addSeries(buys);
    chart->createDefaultAxes();
    chart->setTitle("Stock Price + Buy Signal");

    chartView->setChart(chart);
}