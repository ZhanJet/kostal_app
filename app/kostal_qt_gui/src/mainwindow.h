#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT headers
#include <QMainWindow>
#include <QLineEdit>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void signal_start();
    void signal_stop();
    void signal_exit();
    void signal_run();

private slots:
    void on_pushButton_start_clicked();
    void slot_start_func();

    void on_pushButton_stop_clicked();
    void slot_stop_func();

    void on_pushButton_exit_clicked();
    void slot_exit_func();

    void on_pushButton_run_clicked();
    void slot_run_func();

private:
    Ui::MainWindow* ui;

    flexiv::middleware2::FastRTPSNode sub_node
        = flexiv::middleware2::FastRTPSNode("qt_subscriber");

    flexiv::middleware2::FastRTPSNode pub_node
        = flexiv::middleware2::FastRTPSNode("qt_publisher");

    std::shared_ptr<flexiv::middleware2::FastRTPSSubscriber<
        kostal_gui_msgs::msg::KostalLeverPubSubType>>
        subscriber;

    std::shared_ptr<flexiv::middleware2::FastRTPSPublisher<
        plan_msgs::msg::PlanExecutionPubSubType>>
        publisher;

    QString planName = "";

    bool startExec = false;

    bool stopExec = false;
};
#endif // MAINWINDOW_H
