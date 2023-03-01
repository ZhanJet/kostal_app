#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT headers
#include <QMainWindow>
#include <QFile>
#include <QProcess>

// Pub-Sub associated headers
#include <flexiv/middleware2/fast_rtps_node.h>

// message headers for KostalLever.idl
#include "KostalLever.h"
#include "KostalLeverPubSubTypes.h"

// message headers for PlanExecution.idl
#include "PlanExecution.h"
#include "PlanExecutionPubSubTypes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/** Testman Task Type */
enum TestmanTaskType
{
    TTT_NORMAL = 1,
    TTT_BIAS = 2,
    TTT_DUMMY = 3,
    TTT_TOUCH = 4,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionRestart_Robot_triggered();

private:
    /**
     * @brief init GUI
     */
    void initGUI();

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
    /** Ui*/
    Ui::MainWindow* ui;
    QProcess *m_process;

    /** Subscribe Node*/
    flexiv::middleware2::FastRTPSNode m_subNode;

    /** Subscriber in FastDDS*/
    std::shared_ptr<flexiv::middleware2::FastRTPSSubscriber<kostal_gui_msgs::msg::KostalLeverPubSubType>> m_subscriber;

    /** Subscribed messages*/
    kostal_gui_msgs::msg::KostalLever m_subMsg;
};
#endif // MAINWINDOW_H
