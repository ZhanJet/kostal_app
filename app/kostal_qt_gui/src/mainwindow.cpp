#include <QtDebug>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QThread>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "worker.h"

/** Window title*/
const std::string k_qtWindowTitle = "Kostal Flexiv System v1.0";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // set up ui first
    ui->setupUi(this);

    QProcess *killProcess = new QProcess(this);
    killProcess->start("sh /home/kostal/Programs/Kostal/flexiv_app_guis/app/kostal_qt_gui/kill_rdk.sh");
    killProcess->waitForStarted();

    QThread::msleep(1500);

    QProcess *runProcess = new QProcess(this);
    runProcess->start("/home/kostal/Programs/Kostal/flexiv_app_guis/build/app/kostal_rdk_client/kostal_rdk_client");
    runProcess->waitForStarted();

    // init gui
    initGUI();

    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &Worker::signalUpdateRobotData, this, &MainWindow::slotUpdateRobotData);
    connect(worker, &Worker::signalUpdateSpiData, this, &MainWindow::slotUpdateSpiData);
    connect(worker, &Worker::signalUpdateTestmanData, this, &MainWindow::slotUpdateTestmanData);
    workerThread.start();
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    delete ui;
}

void MainWindow::initGUI()
{
    setWindowIcon(QIcon(":/images/images/logo_color_horizontal.png"));
    setWindowTitle(QString::fromStdString(k_qtWindowTitle));
}

void MainWindow::slotUpdateRobotData(int status, QString task_name, QString task_type)
{
    switch (status) {
    case 105:
        ui->robot_status->setText(tr("105"));
        ui->robot_status->setStyleSheet("QLabel{background-color:#6D6D6D;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 106:
        ui->robot_status->setText(tr("106"));
        ui->robot_status->setStyleSheet("QLabel{background-color:#FF0000;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 107:
        ui->robot_status->setText(tr("107"));
        ui->robot_status->setStyleSheet("QLabel{background-color:#00FF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 108:
        ui->robot_status->setText(tr("108"));
        ui->robot_status->setStyleSheet("QLabel{background-color:#FFFF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    default:
        ui->robot_status->setText("");
        ui->robot_status->setStyleSheet("QLabel{background-color:#FFFFFF;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    }
    ui->robot_product_type->setText(task_name);
    ui->robot_product_type->setFont(QFont("Helvetica", 12));
    ui->robot_task_type->setText(task_type);

}

void MainWindow::slotUpdateSpiData(int status, QString spi_cpol, QString spi_cpha, QString spi_lsb, QString spi_selp)
{
    switch (status) {
    case 109:
        ui->spi_status->setText(tr("109"));
        ui->spi_status->setStyleSheet("QLabel{background-color:#6D6D6D;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 110:
        ui->spi_status->setText(tr("110"));
        ui->spi_status->setStyleSheet("QLabel{background-color:#FF0000;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 111:
        ui->spi_status->setText(tr("111"));
        ui->spi_status->setStyleSheet("QLabel{background-color:#00FF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 112:
        ui->spi_status->setText(tr("112"));
        ui->spi_status->setStyleSheet("QLabel{background-color:#FFFF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    default:
        ui->spi_status->setText("");
        ui->spi_status->setStyleSheet("QLabel{background-color:#FFFFFF;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    }

    ui->spi_cpol->setText(spi_cpol);
    ui->spi_cpha->setText(spi_cpha);
    ui->spi_lsb->setText(spi_lsb);
    ui->spi_selp->setText(spi_selp);
}

void MainWindow::slotUpdateTestmanData(int status)
{
    switch (status) {
    case 101:
        ui->testman_status->setText(tr("101"));
        ui->testman_status->setStyleSheet("QLabel{background-color:#6D6D6D;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 102:
        ui->testman_status->setText(tr("102"));
        ui->testman_status->setStyleSheet("QLabel{background-color:#FF0000;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 103:
        ui->testman_status->setText(tr("103"));
        ui->testman_status->setStyleSheet("QLabel{background-color:#00FF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    case 104:
        ui->testman_status->setText(tr("104"));
        ui->testman_status->setStyleSheet("QLabel{background-color:#FFFF00;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    default:
        ui->testman_status->setText("");
        ui->testman_status->setStyleSheet("QLabel{background-color:#FFFFFF;color:#285780;font-size:20px;border:1px solid #909090;border-radius:5px;}");
        break;
    }
}

void MainWindow::on_actionRestart_Robot_triggered()
{
    QProcess *killProcess = new QProcess(this);
    killProcess->start("sh /home/kostal/Programs/Kostal/flexiv_app_guis/app/kostal_qt_gui/kill_rdk.sh");
    killProcess->waitForStarted();

    QThread::msleep(1500);

    QProcess *runProcess = new QProcess(this);
    runProcess->start("sudo /home/kostal/Programs/Kostal/flexiv_app_guis/build/app/kostal_rdk_client/kostal_rdk_client");
    runProcess->waitForStarted();
}

//restart robot
void MainWindow::slot_restart_robot(){
    on_actionRestart_Robot_triggered();
}
