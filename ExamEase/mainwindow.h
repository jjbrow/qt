#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<readinglogs.h>
#include <QMainWindow>
#include<QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startDBusService();


private:
    Ui::MainWindow *ui;
    ReadingLogs *readingLogs;
    QLabel * label ;
private slots:
    void updateTextBrower(const QString &message);
    void updateMessageFlag();
};
#endif // MAINWINDOW_H
