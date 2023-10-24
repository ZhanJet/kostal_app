// QT associated headers
#include "mainwindowtest.h"
#include "./ui_mainwindowtest.h"
#include <QtDebug>
#include <QStatusBar>
#include <QLabel>


// const variables for window title
const std::string qtWindowTitle = "Kostal Flexiv System v1.0";

// const variables for topic name
const std::string topicKostal = "topic_kostal_states";
const std::string topicPlan = "topic_plan_name";

// create subscribed message
kostal_gui_msgs::msg::KostalLever sub_msg1;
plan_msgs::msg::PlanExecution pub_msg1;

/**
 * @brief callback function for message subscription. This callback function
 * will print tcp pose and flange pose from subscribed messages
 */
void MainWindowTest::subCallback(kostal_gui_msgs::msg::KostalLever* sub_msg1)
{
    // QT print subscribed tcp pose
    std::cout << "Tcp pose sub: ";
    auto tcpPose_array = sub_msg1->tcp_pose();
    for (auto it = tcpPose_array.begin(); it != tcpPose_array.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // QT print subscribed flange pose
    std::cout << "Flange pose sub: ";
    auto flangePose_array = sub_msg1->flange_pose();
    for (auto it = flangePose_array.begin(); it != flangePose_array.end();
         it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // QT set subscribed status light
    int status = (int)(sub_msg1->status()) - 48;
    setStatusLight(ui->label_system_status, status, 16);

    // QT show subscribed taskname & tasktype
    ui->label_task_name->setText(QString::fromStdString(sub_msg1->task_name()));
    ui->label_task_type->setText(QString::fromStdString(sub_msg1->task_type()));

    // QT show subscribed config data
    ui->label_spi_cpha->setText(QString::fromStdString(sub_msg1->cpha()));
    ui->label_spi_cpol->setText(QString::fromStdString(sub_msg1->cpol()));
    ui->label_spi_lsb->setText(QString::fromStdString(sub_msg1->lsb()));
    ui->label_spi_selp->setText(QString::fromStdString(sub_msg1->selp()));
}

// constructor
MainWindowTest::MainWindowTest(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindowTest)
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
        this, &MainWindowTest::signal_start, this, &MainWindowTest::slot_start_func);
    connect(this, &MainWindowTest::signal_stop, this, &MainWindowTest::slot_stop_func);
    connect(this, &MainWindowTest::signal_exit, this, &MainWindowTest::slot_exit_func);
    connect(this, &MainWindowTest::signal_run, this, &MainWindowTest::slot_run_func);

    // setup light to render status
    setStatusLight(ui->label_system_status, 0, 16);
    auto SubCallback = std::bind(&MainWindowTest::subCallback, this, &sub_msg1);

    // create subscription to start subscribing rdk states
    subscriber = sub_node.CreateDefaultSubscription<
        kostal_gui_msgs::msg::KostalLeverPubSubType>(
        topicKostal, SubCallback, (void*)&sub_msg1);

    // create publisher to publish plan name to rdk process
    publisher
        = pub_node
              .CreateDefaultPublisher<plan_msgs::msg::PlanExecutionPubSubType>(
                  topicPlan);
}

// deconstructor
MainWindowTest::~MainWindowTest()
{
    delete ui;
}

// start signal function
void MainWindowTest::on_pushButton_start_clicked()
{
    emit this->signal_start();
}

// stop signal function
void MainWindowTest::on_pushButton_stop_clicked()
{
    emit this->signal_stop();
}

// exit signal function
void MainWindowTest::on_pushButton_exit_clicked()
{
    emit this->signal_exit();
}

void MainWindowTest::on_pushButton_run_clicked()
{
    emit this->signal_run();
}

// start slot function
void MainWindowTest::slot_start_func()
{
    qDebug() << "The start slot is triggered \n";
    planName = "";
    stopExec = false;
    startExec = true;
    pub_msg1.plan_name(planName.toStdString());
    pub_msg1.stop_execution(stopExec);
    pub_msg1.start_execution(startExec);
    publisher->Publish((void*)&pub_msg1);
}

// stop slot function: just stop node
void MainWindowTest::slot_stop_func()
{
    planName = "";
    stopExec = true;
    startExec = false;
    pub_msg1.plan_name(planName.toStdString());
    pub_msg1.stop_execution(stopExec);
    pub_msg1.start_execution(startExec);
    publisher->Publish((void*)&pub_msg1);
}

// exit slot function: stop node and exit
void MainWindowTest::slot_exit_func()
{
    planName = "";
    stopExec = true;
    startExec = false;
    pub_msg1.plan_name(planName.toStdString());
    pub_msg1.stop_execution(stopExec);
    pub_msg1.start_execution(startExec);
    publisher->Publish((void*)&pub_msg1);
    sub_node.StopAll();
    QMainWindow::close();
}

// run slot function: type in plan name and run
void MainWindowTest::slot_run_func()
{
    planName = ui->lineEdit_plan_name->text();
    stopExec = false;
    startExec = true;
    pub_msg1.plan_name(planName.toStdString());
    pub_msg1.stop_execution(stopExec);
    pub_msg1.start_execution(startExec);
    publisher->Publish((void*)&pub_msg1);
}

/**
 * @brief create a light blub to show assigned status
 * @param[in] label label for this light blub
 * @param[in] color color for this light blub
 * @param[in] size size for this light blub
 */
void MainWindowTest::setStatusLight(QLabel* label, int color, int size)
{
    // setText should be set with null string
    label->setText("");
    QString min_width = QString("min-width: %1px;").arg(size);
    QString min_height = QString("min-height: %1px;").arg(size);
    QString max_width = QString("max-width: %1px;").arg(size);
    QString max_height = QString("max-height: %1px;").arg(size);
    QString border_radius = QString("border-radius: %1px;").arg(size / 2);
    QString border = QString("border:1px solid black;");
    QString background = "background-color:";

    switch (color) {
        case 0:
            // grey
            background += "rgb(190,190,190)";
            break;
        case 1:
            // red
            background += "rgb(255,0,0)";
            break;
        case 2:
            // green
            background += "rgb(0,255,0)";
            break;
        case 3:
            // yellow
            background += "rgb(255,255,0)";
            break;
        default:
            break;
    }
    const QString SheetStyle = min_width + min_height + max_width + max_height
                               + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

//restart robot
void MainWindow::slot_restart_robot(){
    on_actionRestart_Robot_triggered();
}
