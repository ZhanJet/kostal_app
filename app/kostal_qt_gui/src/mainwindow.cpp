#include <QtDebug>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


/** Window title*/
const std::string k_qtWindowTitle = "Kostal Flexiv System v1.0";

const std::string k_topicKostal = "topic_kostal_states";

/** Subscribe Node name*/
const std::string k_subNodeName = "qt_subscriber_new";

/** ASCII Offset*/
const int k_asciiOffset = 48;

/** Robot Status label color*/
const QString k_statusColor = "background-color: %1;color:%2;font-size: 20px;border: 1px solid #909090;border-radius: 5px;";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_subNode(k_subNodeName)
{
    // set up ui first
    ui->setupUi(this);

    // init gui
    initGUI();

    // create callback function for subscription
    auto SubCallback = std::bind(&MainWindow::subCallback, this, &m_subMsg);

    // create subscription to start subscribing rdk states
    m_subscriber = m_subNode.CreateDefaultSubscription<kostal_gui_msgs::msg::KostalLeverPubSubType>(k_topicKostal, SubCallback, (void*)&m_subMsg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGUI()
{
    setWindowIcon(QIcon(":/images/images/logo_color_horizontal.png"));
    setWindowTitle(QString::fromStdString(k_qtWindowTitle));
}

void MainWindow::subCallback(kostal_gui_msgs::msg::KostalLever* subscribedMsg)
{
    // update Robot
    updateRobotData(subscribedMsg);

    // update SPI
    updateSpiData(subscribedMsg);

    // update Testman
    updateTestmanData(subscribedMsg);
}

void MainWindow::updateRobotData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->robot_status());
    qDebug() << "updateRobotData status" << status;
    switch (status) {
    case 105:
        ui->robot_status->setText(tr("105"));
        ui->robot_status->setStyleSheet(k_statusColor.arg("#6D6D6D").arg("#285780"));
        break;
    case 106:
        ui->robot_status->setText(tr("106"));
        ui->robot_status->setStyleSheet(k_statusColor.arg("#FF0000").arg("#285780"));
        break;
    case 107:
        ui->robot_status->setText(tr("107"));
        ui->robot_status->setStyleSheet(k_statusColor.arg("#00FF00").arg("#285780"));
        break;
    case 108:
        ui->robot_status->setText(tr("108"));
        ui->robot_status->setStyleSheet(k_statusColor.arg("#FFFF00").arg("#285780"));
        break;
    default:
        ui->robot_status->setText("");
        ui->robot_status->setStyleSheet(k_statusColor.arg("#FFFFFF").arg("#285780"));
        break;
    }

    ui->robot_product_type->setText(QString::fromStdString(subscribedMsg->task_name()));
    ui->robot_product_type->setFont(QFont("Helvetica", 12));
    ui->robot_task_type->setText(QString::fromStdString(subscribedMsg->task_type()));
}

void MainWindow::updateSpiData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->spi_status());
    qDebug() << "updateSpiData status" << status;
    switch (status) {
    case 109:
        ui->spi_status->setText(tr("109"));
        ui->spi_status->setStyleSheet(k_statusColor.arg("#6D6D6D").arg("#285780"));
        break;
    case 110:
        ui->spi_status->setText(tr("110"));
        ui->spi_status->setStyleSheet(k_statusColor.arg("#FF0000").arg("#285780"));
        break;
    case 111:
        ui->spi_status->setText(tr("111"));
        ui->spi_status->setStyleSheet(k_statusColor.arg("#00FF00").arg("#285780"));
        break;
    case 112:
        ui->spi_status->setText(tr("112"));
        ui->spi_status->setStyleSheet(k_statusColor.arg("#FFFF00").arg("#285780"));
        break;
    default:
        ui->spi_status->setText("");
        ui->spi_status->setStyleSheet(k_statusColor.arg("#FFFFFF").arg("#285780"));
        break;
    }

    // spi configuration info
    ui->spi_cpol->setText(QString::fromStdString(subscribedMsg->cpol()));
    ui->spi_cpha->setText(QString::fromStdString(subscribedMsg->cpha()));
    ui->spi_lsb->setText(QString::fromStdString(subscribedMsg->lsb()));
    ui->spi_selp->setText(QString::fromStdString(subscribedMsg->selp()));
}

void MainWindow::updateTestmanData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->testman_status());
    qDebug() << "updateTestmanData status" << status;
    switch (status) {
    case 101:
        ui->testman_status->setText(tr("101"));
        ui->testman_status->setStyleSheet(k_statusColor.arg("#6D6D6D").arg("#285780"));
        break;
    case 102:
        ui->testman_status->setText(tr("102"));
        ui->testman_status->setStyleSheet(k_statusColor.arg("#FF0000").arg("#285780"));
        break;
    case 103:
        ui->testman_status->setText(tr("103"));
        ui->testman_status->setStyleSheet(k_statusColor.arg("#00FF00").arg("#285780"));
        break;
    case 104:
        ui->testman_status->setText(tr("104"));
        ui->testman_status->setStyleSheet(k_statusColor.arg("#FFFF00").arg("#285780"));
        break;
    default:
        ui->testman_status->setText("");
        ui->testman_status->setStyleSheet(k_statusColor.arg("#FFFFFF").arg("#285780"));
        break;
    }
}
