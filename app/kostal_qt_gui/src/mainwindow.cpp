// QT associated headers
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "statuslight.h"
#include <QtDebug>
#include <QStatusBar>
#include <QLabel>

// const variables for window title and topic name
const std::string qtWindowTitle = "Kostal Flexiv System v1.0";
const std::string topicKostal = "topic_kostal";
// create subscribed message
kostal_gui_msgs::msg::KostalLever sub_msg;

/**
 * @brief callback function for message subscription. This callback function
 * will print tcp pose and flange pose from subscribed messages
 */
void subCallback()
{
    std::cout << "Tcp pose sub: ";
    auto tcpPose_array = sub_msg.tcp_pose();
    for (auto it = tcpPose_array.begin(); it != tcpPose_array.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Flange pose sub: ";
    auto flangePose_array = sub_msg.flange_pose();
    for (auto it = flangePose_array.begin(); it != flangePose_array.end();
         it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// constructor
MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    // set up ui first
    ui->setupUi(this);
    ui->lineEdit_plan_name->setPlaceholderText("Please type in plan ...");
    ui->lineEdit_plan_name->setClearButtonEnabled(true);

    // setup window title
    setWindowTitle(QString::fromStdString(qtWindowTitle));
    // add staus bar to hint status
    QStatusBar* statusbar = new QStatusBar();
    setStatusBar(statusbar);
    QLabel* statuslabel = new QLabel("Hint", this);
    statusbar->addWidget(statuslabel);
    // connect signal & slot functions
    connect(
        this, &MainWindow::signal_start, this, &MainWindow::slot_start_func);
    connect(this, &MainWindow::signal_stop, this, &MainWindow::slot_stop_func);
    connect(this, &MainWindow::signal_exit, this, &MainWindow::slot_exit_func);
    connect(this, &MainWindow::signal_run, this, &MainWindow::slot_run_func);
    // setup light to render status
    setStatusLight(ui->label_system_status, 1, 16);
    // create subscription to start subscription in the constructor
    subscriber = subNode.CreateDefaultSubscription<
        kostal_gui_msgs::msg::KostalLeverPubSubType>(
        topicKostal, &subCallback, (void*)&sub_msg);
}

// deconstructor
MainWindow::~MainWindow()
{
    delete ui;
}

// start signal function
void MainWindow::on_pushButton_start_clicked()
{
    emit this->signal_start();
}

// stop signal function
void MainWindow::on_pushButton_stop_clicked()
{
    emit this->signal_stop();
}

// exit signal function
void MainWindow::on_pushButton_exit_clicked()
{
    emit this->signal_exit();
}

void MainWindow::on_pushButton_run_clicked()
{
    emit this->signal_run();
}

// start slot function
void MainWindow::slot_start_func()
{
    // Empty for now
    qDebug() << "The start slot is triggered \n";
}

// stop slot function
void MainWindow::slot_stop_func()
{
    qDebug() << "The stop slot is triggered \n";
    subNode.StopAll();
}

// exit slot function
void MainWindow::slot_exit_func()
{
    qDebug() << "The exit slot is triggered \n";
    subNode.StopAll();
    QMainWindow::close();
}

// run slot function
void MainWindow::slot_run_func()
{
    qDebug() << "The run slot is triggered \n";
    planName = ui->lineEdit_plan_name->text();
    qDebug() << planName;
}