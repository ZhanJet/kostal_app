#ifndef WORKER_H
#define WORKER_H

#include <QObject>

// Pub-Sub associated headers
#include <flexiv/middleware2/fast_rtps_node.h>

// message headers for KostalLever.idl
#include "KostalLever.h"
#include "KostalLeverPubSubTypes.h"

// message headers for PlanExecution.idl
#include "PlanExecution.h"
#include "PlanExecutionPubSubTypes.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject* parent = nullptr);
    /**
     * @brief callback function for message subscription. This callback function
     * will print tcp pose and flange pose from subscribed messages
     * @param[in] subscribedMsg Subscribed messages
     */
    void subCallback(kostal_gui_msgs::msg::KostalLever* subscribedMsg);
    /**
     * @brief Update Robot Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateRobotData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update SPI Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateSpiData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update Testman Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateTestmanData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);
private:
    /** Subscribe Node*/
    flexiv::middleware2::FastRTPSNode m_subNode;

    /** Subscriber in FastDDS*/
    std::shared_ptr<flexiv::middleware2::FastRTPSSubscriber<kostal_gui_msgs::msg::KostalLeverPubSubType>> m_subscriber;

    /** Subscribed messages*/
    kostal_gui_msgs::msg::KostalLever m_subMsg;
signals:
    void signalUpdateRobotData(int, QString, QString);
    void signalUpdateSpiData(int, QString, QString, QString, QString);
    void signalUpdateTestmanData(int);
};

#endif // WORKER_H
