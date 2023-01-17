#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT headers
#include <QMainWindow>
#include <QTextCursor>
#include <QFile>

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

/** Robot work status */
enum RobotStatus
{
    RS_INIT,
    RS_IDLE,
    RS_BUSY,
    RS_FAULT,
    RS_DONE
};

/** Testman Task Type */
enum TestmanTaskType
{
    TTT_NORMAL = 1,
    TTT_BIAS = 2,
    TTT_DUMMY = 3,
    TTT_TOUCH = 4,
};

/** Log Type*/
enum LogType
{
    LT_INFO,
    LT_ERROR,
    LT_WARNING,

    LT_NUM = LT_WARNING - LT_INFO + 1
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    /**
     * @brief init GUI
     */
    void initGUI();

    /**
     * @brief connect Slots
     */
    void connectSlots();

    /**
     * @brief callback function for message subscription. This callback function
     * will print tcp pose and flange pose from subscribed messages
     * @param[in] subscribedMsg Subscribed messages
     */
    void subCallback(kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update KostalLever Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateKostalLeverData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update Robot Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateRobotData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update SPI Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateSPIData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update Testman Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateTestmanData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update Log Data
     * @param[in] subscribedMsg Subscribed messages
     */
    void updateLogData(const kostal_gui_msgs::msg::KostalLever* subscribedMsg);

    /**
     * @brief Update log
     * @param[in] list Data list that need update
     * @param[in] type Type of log
     */
    void updateLog(const QString& logData, const LogType& type);

    /**
     * @brief Save log in file
     * @param[in] list Data list that need update
     * @param[in] type Type of log
     */
    void saveLog(const QString& logData, const LogType& type);

signals:

private slots:
    /**
     * @brief Slot Reset Robot
     */
    void slotResetRobot();

    /**
     * @brief Slot test log function
     */
    void slotTestLog();

private:
    /** Ui*/
    Ui::MainWindow* ui;

    /** Subscribe Node*/
    flexiv::middleware2::FastRTPSNode m_subNode;

    /** Publish Node*/
    flexiv::middleware2::FastRTPSNode m_pubNode;

    /** Subscriber in FastDDS*/
    std::shared_ptr<flexiv::middleware2::FastRTPSSubscriber<
        kostal_gui_msgs::msg::KostalLeverPubSubType>>
        m_subscriber;

    /** Pulisher in FastDDS*/
    std::shared_ptr<flexiv::middleware2::FastRTPSPublisher<
        plan_msgs::msg::PlanExecutionPubSubType>>
        m_publisher;

    /** Subscribed messages*/
    kostal_gui_msgs::msg::KostalLever m_subMsg;

    /** Published messages*/
    plan_msgs::msg::PlanExecution m_pubMsg;

    /** Plan name*/
    QString m_planName;

    /** Flag*/
    bool m_isStartExec;

    /** Num of log*/
    int m_logNum;

    /** Current Text Cursor*/
    QTextCursor m_currentTxtCur;

    /** Log file*/
    QFile* g_logFile;

    /** Log file path*/
    QString m_logpath;

    /** Log file path*/
    QString g_pathName;
};
#endif // MAINWINDOW_H
