#include "worker.h"
#include <QDebug>

const std::string k_topicKostal = "topic_kostal_states";
const std::string k_subNodeName = "qt_subscriber_new";

Worker::Worker(QObject *parent) : QObject(parent), m_subNode(k_subNodeName)
{
    // create callback function for subscription
    auto SubCallback = std::bind(&Worker::subCallback, this, &m_subMsg);

    // create subscription to start subscribing rdk states
    m_subscriber = m_subNode.CreateDefaultSubscription<kostal_gui_msgs::msg::KostalLeverPubSubType>(k_topicKostal, SubCallback, (void*)&m_subMsg);
}

void Worker::subCallback(kostal_gui_msgs::msg::KostalLever* subscribedMsg)
{
    if(subscribedMsg)
    {
        // update Robot
        updateRobotData(subscribedMsg);

        // update SPI
        updateSpiData(subscribedMsg);

        // update Testman
        updateTestmanData(subscribedMsg);
    }
    else
    {
        qDebug() << "wrong msg!";
    }
}

void Worker::updateRobotData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->robot_status());
    qDebug() << "updateRobotData status" << status;

    emit signalUpdateRobotData(status, QString::fromStdString(subscribedMsg->task_name()), QString::fromStdString(subscribedMsg->task_type()));
}

void Worker::updateSpiData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->spi_status());
    qDebug() << "updateSpiData status" << status;

    // spi configuration info
    QString spi_cpol = QString::fromStdString(subscribedMsg->cpol());
    QString spi_cpha = QString::fromStdString(subscribedMsg->cpha());
    QString spi_lsb = QString::fromStdString(subscribedMsg->lsb());
    QString spi_selp = QString::fromStdString(subscribedMsg->selp());

    emit signalUpdateSpiData(status, spi_cpol, spi_cpha, spi_lsb, spi_selp);
}

void Worker::updateTestmanData(const kostal_gui_msgs::msg::KostalLever *subscribedMsg)
{
    int status = (int)(subscribedMsg->testman_status());
    qDebug() << "updateTestmanData status" << status;

    emit signalUpdateTestmanData(status);
}
