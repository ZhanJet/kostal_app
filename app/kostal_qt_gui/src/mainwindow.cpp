#include <QtDebug>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


/** Window title*/
const std::string k_qtWindowTitle = "Kostal Flexiv System v1.0";

/** Topic Name Kostal*/
const std::string k_topicKostal = "topic_kostal_states";

/** Topic Name plan*/
const std::string k_topicPlan = "topic_plan_name";

/** Subscribe Node name*/
const std::string k_subNodeName = "qt_subscriber_new";

/** Publish Node name*/
const std::string k_pubNodeName = "qt_publisher_new";

/** ASCII Offset*/
const int k_asciiOffset = 48;

/** Robot Status label color*/
const QString k_robotStatusColor = "background-color: %1;"
    "color:%2;"
    "font-size: 20px;"
    "border: 1px solid #909090;"
    "border-radius: 5px;";

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, m_subNode(k_subNodeName)
, m_pubNode(k_pubNodeName)
, m_logNum(1)
{
    // set up ui first
    ui->setupUi(this);

    // create log
    g_logFile = new QFile(this);
    m_logpath = QCoreApplication::applicationDirPath() + "/LOG/";
    QDir dir(m_logpath);
    if (!dir.exists()) {
        dir.mkdir(m_logpath);
    }

    // init gui
    initGUI();

    // connect Slots
    connectSlots();

    // create callback function for subscription
    auto SubCallback = std::bind(&MainWindow::subCallback, this, &m_subMsg);

    // create subscription to start subscribing rdk states
    m_subscriber = m_subNode.CreateDefaultSubscription<
        kostal_gui_msgs::msg::KostalLeverPubSubType>(
        k_topicKostal, SubCallback, (void*)&m_subMsg);

    // create publisher to publish msg to rdk process
    m_publisher
        = m_pubNode
              .CreateDefaultPublisher<plan_msgs::msg::PlanExecutionPubSubType>(
                  k_topicPlan);
}

MainWindow::~MainWindow()
{
    delete ui;

    // close log
    if (g_logFile->isOpen()) {
        g_logFile->close();
    }
}

void MainWindow::initGUI()
{
    setWindowIcon(QIcon(":/images/images/logo_color_horizontal.png"));
    setWindowTitle(QString::fromStdString(k_qtWindowTitle));

    ui->saveLogButton->hide();
    ui->logTextEdit->document()->setMaximumBlockCount(100);
    ui->logTextEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

    // testjxy: test log
    QTimer* timer;
    timer = new QTimer();
//    timer->start(1000);

    connect(timer, &QTimer::timeout, this,
        &MainWindow::slotTestLog);
}

void MainWindow::connectSlots()
{
    connect(ui->robotResetAction, SIGNAL(triggered()), this, SLOT(slotResetRobot()));
}

void MainWindow::subCallback(kostal_gui_msgs::msg::KostalLever* subscribedMsg)
{
    // QT print subscribed tcp pose
    std::cout << "Tcp pose sub: ";
    auto tcpPose_array = subscribedMsg->tcp_pose();
    for (auto it = tcpPose_array.begin(); it != tcpPose_array.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // QT print subscribed flange pose
    std::cout << "Flange pose sub: ";
    auto flangePose_array = subscribedMsg->flange_pose();
    for (auto it = flangePose_array.begin(); it != flangePose_array.end();
         it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // update kostal data
    updateKostalLeverData(subscribedMsg);
}

void MainWindow::updateKostalLeverData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg)
{
    // update SPI
    updateSPIData(subscribedMsg);

    // update Robot
    updateRobotData(subscribedMsg);

    // update Testman
    updateTestmanData(subscribedMsg);

    // record log
    updateLogData(subscribedMsg);
}

void MainWindow::updateRobotData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    // node name
    ui->robotNodeNameLabel->setText(QString::fromStdString(subscribedMsg->node_name()));
    // work status
    int status = (int)(subscribedMsg->status()) - k_asciiOffset;
    if(status == RobotStatus::RS_INIT) {
        ui->robotStatusLabel->setText("INIT");  // init = yellow
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#FFFF00").arg("#285780"));
    } else if(status == RobotStatus::RS_IDLE) {
        ui->robotStatusLabel->setText("IDLE");  // idle = blue
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#0091FF").arg("#FFFFFF"));
        // when robot is IDLE, testman is STOP
        ui->testmanStatusLabel->setText("STOP");
    } else if (status == RobotStatus::RS_BUSY) {
        ui->robotStatusLabel->setText("BUSY");  // busy = green
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#00FF00").arg("#285780"));
        // when robot is BUSY, testman is START
        ui->testmanStatusLabel->setText("START");
    } else if (status == RobotStatus::RS_FAULT) {
        ui->robotStatusLabel->setText("FAULT"); // fault = red
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#FF0000").arg("#285780"));
    } else if (status == RobotStatus::RS_DONE)  {
        ui->robotStatusLabel->setText("DONE");  // done = white
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#FFFFFF").arg("#285780"));
    } else {                                    // other = grey
        ui->robotStatusLabel->setText("");
        ui->robotStatusLabel->setStyleSheet(k_robotStatusColor.arg("#6D6D6D").arg("#285780"));
    }

    // tcp pose
    auto tcpPoseArray = subscribedMsg->tcp_pose();
    ui->robotTCPXLabel->setText(QString::number(tcpPoseArray.at(0),'f',3));
    ui->robotTCPYLabel->setText(QString::number(tcpPoseArray.at(1),'f',3));
    ui->robotTCPZLabel->setText(QString::number(tcpPoseArray.at(2),'f',3));
    ui->robotTCPRXLabel->setText(QString::number(tcpPoseArray.at(3),'f',3));
    ui->robotTCPRYLabel->setText(QString::number(tcpPoseArray.at(4),'f',3));
    ui->robotTCPRZLabel->setText(QString::number(tcpPoseArray.at(5),'f',3));

    // flange pose
    auto flangePoseArray = subscribedMsg->flange_pose();
    ui->robotFlangeXLabel->setText(QString::number(flangePoseArray.at(0),'f',3));
    ui->robotFlangeYLabel->setText(QString::number(flangePoseArray.at(1),'f',3));
    ui->robotFlangeZLabel->setText(QString::number(flangePoseArray.at(2),'f',3));
    ui->robotFlangeRXLabel->setText(QString::number(flangePoseArray.at(3),'f',3));
    ui->robotFlangeRYLabel->setText(QString::number(flangePoseArray.at(4),'f',3));
    ui->robotFlangeRZLabel->setText(QString::number(flangePoseArray.at(5),'f',3));

    // sensor force
    auto sensorArray = subscribedMsg->raw_data();
    ui->robotSensorForceXLabel->setText(QString::number(sensorArray.at(0)));
    ui->robotSensorForceYLabel->setText(QString::number(sensorArray.at(1)));
    ui->robotSensorForceZLabel->setText(QString::number(sensorArray.at(2)));
    ui->robotSensorTorqueXLabel->setText(QString::number(sensorArray.at(3)));
    ui->robotSensorTorqueYLabel->setText(QString::number(sensorArray.at(4)));
    ui->robotSensorTorqueZLabel->setText(QString::number(sensorArray.at(5)));
}

void MainWindow::updateSPIData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    // spi configuration info
    ui->spiCPOLLabel->setText(QString::fromStdString(subscribedMsg->cpol()));
    ui->spiCPHALabel->setText(QString::fromStdString(subscribedMsg->cpha()));
    ui->spiLSBLabel->setText(QString::fromStdString(subscribedMsg->lsb()));
    ui->spiSELPLabel->setText(QString::fromStdString(subscribedMsg->selp()));

    // spi data or spi data recv flag
    //    ui->spiRecvDataLabel->setText(QString::fromStdString(subscribedMsg->recvFlag()));
}

void MainWindow::updateTestmanData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    // product type
    ui->testmanTypeLabel->setText(QString::fromStdString(subscribedMsg->task_name()));
    qDebug() << "task name = " << QString::fromStdString(subscribedMsg->task_name());
    // test status (start or stop)
    // ---> this status is set according to the robot work status

    // task type
    ui->testmanTaskTypeLabel->setText(QString::fromStdString(subscribedMsg->task_type()));

    // whether return to origin

    // whether recv undefined cmd

}

void MainWindow::updateLogData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    QString logData = QString::fromStdString(subscribedMsg->task_name());
    LogType type = (LogType)(subscribedMsg->status());
    updateLog(logData,type);
}

void MainWindow::updateLog(const QString &logData, const LogType& type)
{
    // save current cursor
    m_currentTxtCur = ui->logTextEdit->textCursor();

    QString color;
    QString strLog;
    if(type == LT_INFO) {
        color = ("#0091FF");
    }
    else if(type == LT_ERROR) {
        color = ("#E50000");
    }
    else if(type == LT_WARNING) {
        color = ("#F3A400");
    }

    strLog = QString("<font color = %1 size = \"4\"> %2 </font>").arg(color).arg(logData);
    ui->logTextEdit->append(strLog);
    ui->logTextEdit->setTextCursor(m_currentTxtCur);
    ++m_logNum;

    // save log in file
    saveLog(logData,type);
}

void MainWindow::saveLog(const QString &logData, const LogType &type)
{
    QDateTime time = QDateTime::currentDateTime();
    QString logFileName  = "log"+time.toString("yyyyMMdd")+".txt";
    QString logFilePath = m_logpath+logFileName;

    // check whole path name, avoid unused open file
    if (g_pathName != logFilePath) {
        g_pathName = logFilePath;

        // close old
        if (g_logFile->isOpen()) {
            g_logFile->close();
        } else {}

        // open
        g_logFile->setFileName(g_pathName);
        g_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    }

    // write log into file
    QTextStream out(g_logFile);
    switch(type)
    {
        case LT_INFO:
            out <<"[INFO]"<< logData << endl;
            break;
        case LT_ERROR:
            out <<"[ERROR]"<< logData << endl;
            break;
        case LT_WARNING:
            out <<"[WARNING]"<< logData << endl;
            break;
        default:
            break;
    }

    // when file size is over 10mb, create new log file for today
    if (g_logFile->size() >= 10*1024*1024) {
        QString logfileNameBackup = QObject::tr("log%1%2%3%4%5%6.txt").arg(time.date().year()).arg(time.date().month(),2,10,QChar('0'))
                                .arg(time.date().day(),2,10,QChar('0')).arg(time.time().hour(),2,10,QChar('0')).arg(time.time().minute(),2,10,QChar('0'))
                                .arg(time.time().second(),2,10,QChar('0'));

        qDebug() << "Rename = " << g_logFile->rename(m_logpath+logfileNameBackup);

        // rename a new log file
        g_pathName = logfileNameBackup;
    }
}

void MainWindow::slotTestLog()
{
    QString log = QString("This is a Flexiv test log : %1").arg(m_logNum);
    LogType type = (LogType)(m_logNum%LT_NUM);
    updateLog(log,type);
}

void MainWindow::slotResetRobot()
{
    qDebug() << "The start slot is triggered \n";
    m_pubMsg.plan_name( "plan1");
    m_pubMsg.stop_execution(false);
    m_pubMsg.start_execution(true);
    m_publisher->Publish((void*)&m_pubMsg);
}
