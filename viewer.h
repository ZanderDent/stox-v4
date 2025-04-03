#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QChartView>
#include <QLabel>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <vector>
#include <string>

class StockViewer : public QMainWindow {
    Q_OBJECT

public:
    StockViewer(QWidget* parent = nullptr);

private slots:
    void loadTicker();  // handles button press

private:
    QLineEdit* tickerInput;
    QLabel* statusLabel;
    QChartView* chartView;

    void updateChart(const std::vector<double>& prices, const std::vector<int>& buySignals);
};

#endif // VIEWER_H
