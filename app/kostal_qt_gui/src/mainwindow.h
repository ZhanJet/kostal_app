#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
// Pub-Sub associated headers
#include <flexiv/middleware2/fast_rtps_node.h>
#include "KostalLever.h"
#include "KostalLeverPubSubTypes.h"

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
    flexiv::middleware2::FastRTPSNode subNode
        = flexiv::middleware2::FastRTPSNode("my_subscriber");
    std::shared_ptr<flexiv::middleware2::FastRTPSSubscriber<
        kostal_gui_msgs::msg::KostalLeverPubSubType>>
        subscriber;
    QString planName;
};
#endif // MAINWINDOW_H
