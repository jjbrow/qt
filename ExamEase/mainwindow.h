#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QLabel>
#include "databaseutils.h"
#include <QDatetime>
#include <QTableWidget>
#include <QMessageBox>
#include <QMenu>
#include "paper.h"
#include "addquestion.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTable();
    void operateButtons();
    void makeColumnNonEditable(QTableWidget *tableWidget, int column);
    void setupButtonsForPaper(QTableWidget *tableWidget, int row, const Paper &p, const QList<Paper> &list);
private:
    Ui::MainWindow *ui;
    DataBaseUtils db;
    QLabel * label ;
private slots:

};
#endif // MAINWINDOW_H
