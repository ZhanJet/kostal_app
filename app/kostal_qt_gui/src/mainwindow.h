#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT headers
#include <QMainWindow>
#include <QFile>
#include <QProcess>
#include <QThread>

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
    void initGUI();
private:
    Ui::MainWindow* ui;
    QProcess *m_process;
    QThread workerThread;
public slots:
    void slotUpdateRobotData(int status, QString task_name, QString task_type);
    void slotUpdateSpiData(int status, QString spi_cpol, QString spi_cpha, QString spi_lsb, QString spi_selp);
    void slotUpdateTestmanData(int status);
    void on_actionRestart_Robot_triggered();
    void slot_restart_robot();
};
#endif // MAINWINDOW_H
