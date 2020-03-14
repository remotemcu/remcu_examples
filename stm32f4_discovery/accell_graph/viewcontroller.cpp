#include "viewcontroller.h"
#include "chart.h"

#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QtWidgets>
#include <QDialog>
#include <QDataStream>

static QLineEdit *hostLineEdit;
static QLineEdit *portLineEdit;

ViewController::ViewController()
{
    QMainWindow * window = new QMainWindow;

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *ControlLayout = new QVBoxLayout;

    QLabel *hostLabel = new QLabel("Server OpenOCD/GDB:");
    hostLineEdit = new QLineEdit("127.0.0.1");

    QLabel *portLabel = new QLabel("Port server:");
    portLineEdit = new QLineEdit("6666");

    QPushButton *connect_button = new QPushButton("Connect");

    ControlLayout->addWidget(hostLabel);
    ControlLayout->addWidget(hostLineEdit);
    ControlLayout->addWidget(portLabel);
    ControlLayout->addWidget(portLineEdit);
    //ControlLayout->addWidget();
    ControlLayout->addWidget(connect_button);

    ControlLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    mainLayout->addLayout(ControlLayout);

    Chart *chart = new Chart;
    chart->setTitle("Acceleration Sensor Data");
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView * chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);

    mainLayout->setStretchFactor(ControlLayout,1);
    mainLayout->setStretchFactor(chartView,6);

    window->setCentralWidget(new QWidget);
    window->centralWidget()->setLayout(mainLayout);
    window->resize(1000, 500);
    window->show();

    connect(connect_button, &QPushButton::clicked,
            this, &ViewController::connect_click);
    connect(this, &ViewController::dataChanged,
            chart, &Chart::printData);

}


void ViewController::connect_click(){
    QString sHost = hostLineEdit->text();
    QString sPort = portLineEdit->text();

    bool ok = false;
    uint16_t port = sPort.toUShort(&ok, 10);

    if(ok == false){
        qWarning() << "failed convert port";
        return;
    }

    if(sHost.isEmpty()){
        qWarning() << "host name is empty";
        return;
    }

    qDebug() << "get parameter: " << sHost << ":" << port;

    remcu_thread_options_t options = {sHost, port};

    emit start(options);
}
