// QT associated headers
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtDebug>
#include <QStatusBar>
#include <QLabel>

// wrapped headers
#include "statuslight.h"

// const variables for window title
const std::string qtWindowTitle = "Kostal Flexiv System v1.0";

// const variables for topic name
const std::string topicKostal = "topic_kostal_states";
const std::string topicPlan = "topic_plan_name";

// create subscribed message
kostal_gui_msgs::msg::KostalLever sub_msg;
plan_msgs::msg::PlanExecution pub_msg;

/**
 * @brief callback function for message subscription. This callback function
 * will print tcp pose and flange pose from subscribed messages
 */
void MainWindow::subCallback(kostal_gui_msgs::msg::KostalLever* sub_msg)
{
    // QT print subscribed tcp pose
    std::cout << "Tcp pose sub: ";
    auto tcpPose_array = sub_msg->tcp_pose();
    for (auto it = tcpPose_array.begin(); it != tcpPose_array.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // QT print subscribed flange pose
    std::cout << "Flange pose sub: ";
    auto flangePose_array = sub_msg->flange_pose();
    for (auto it = flangePose_array.begin(); it != flangePose_array.end();
         it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // QT set subscribed status light
    int status = (int)(sub_msg->status()) - 48;
    setStatusLight(ui->label_system_status, status, 16);

    // QT show subscribed taskname & tasktype
    ui->label_task_name->setText(QString::fromStdString(sub_msg->task_name()));
    ui->label_task_type->setText(QString::fromStdString(sub_msg->task_type()));

    // QT show subscribed config data
    ui->label_spi_cpha->setText(QString::fromStdString(sub_msg->cpha()));
    ui->label_spi_cpol->setText(QString::fromStdString(sub_msg->cpol()));
    ui->label_spi_lsb->setText(QString::fromStdString(sub_msg->lsb()));
    ui->label_spi_selp->setText(QString::fromStdString(sub_msg->selp()));
}

// constructor
MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    // set up ui first
    ui->setupUi(this);
    ui->lineEdit_plan_name->setPlaceholderText("Please type in plan name ...");
    ui->lineEdit_plan_name->setClearButtonEnabled(true);

    // setup window title
    setWindowTitle(QString::fromStdString(qtWindowTitle));

    // add hint function of the floating cursor
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
    setStatusLight(ui->label_system_status, 0, 16);
    auto SubCallback = std::bind(&MainWindow::subCallback, this, &sub_msg);

    // create subscription to start subscribing rdk states
    subscriber = sub_node.CreateDefaultSubscription<
        kostal_gui_msgs::msg::KostalLeverPubSubType>(
        topicKostal, SubCallback, (void*)&sub_msg);

    // create publisher to publish plan name to rdk process
    publisher
        = pub_node
              .CreateDefaultPublisher<plan_msgs::msg::PlanExecutionPubSubType>(
                  topicPlan);
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
    qDebug() << "The start slot is triggered \n";
    planName = "";
    stopExec = false;
    startExec = true;
    pub_msg.plan_name(planName.toStdString());
    pub_msg.stop_execution(stopExec);
    pub_msg.start_execution(startExec);
    publisher->Publish((void*)&pub_msg);
}

// stop slot function: just stop node
void MainWindow::slot_stop_func()
{
    planName = "";
    stopExec = true;
    startExec = false;
    pub_msg.plan_name(planName.toStdString());
    pub_msg.stop_execution(stopExec);
    pub_msg.start_execution(startExec);
    publisher->Publish((void*)&pub_msg);
}

// exit slot function: stop node and exit
void MainWindow::slot_exit_func()
{
    planName = "";
    stopExec = true;
    startExec = false;
    pub_msg.plan_name(planName.toStdString());
    pub_msg.stop_execution(stopExec);
    pub_msg.start_execution(startExec);
    publisher->Publish((void*)&pub_msg);
    sub_node.StopAll();
    QMainWindow::close();
}

// run slot function: type in plan name and run
void MainWindow::slot_run_func()
{
    planName = ui->lineEdit_plan_name->text();
    stopExec = false;
    startExec = true;
    pub_msg.plan_name(planName.toStdString());
    pub_msg.stop_execution(stopExec);
    pub_msg.start_execution(startExec);
    publisher->Publish((void*)&pub_msg);
}