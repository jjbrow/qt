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
#include <QAxObject>
#include "dateutils.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //表格初始化
    void initTable();
    //操作按钮绑定
    void operateButtons();
    //设置第几列不可编辑
    void makeColumnNonEditable(QTableWidget *tableWidget, int column);
    //设置列表按钮
    void setupButtonsForPaper(QTableWidget *tableWidget, int row, const Paper &p, const QList<Paper> &list);
    //将试卷导出到word
    void exportForWord(int id);
private:
    Ui::MainWindow *ui;
    DataBaseUtils db;
    QLabel * label ;
    QAxObject* wordApp;
private slots:

};
#endif // MAINWINDOW_H
