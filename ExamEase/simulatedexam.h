#ifndef SIMULATEDEXAM_H
#define SIMULATEDEXAM_H

#include <QDialog>
#include"question.h"
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include"question.h"
#include"databaseutils.h"
#include <QButtonGroup>
#include <QTimer>
#include <QMessageBox>
namespace Ui {
class SimulatedExam;
}

class SimulatedExam : public QDialog
{
    Q_OBJECT

public:
    explicit SimulatedExam(QWidget *parent = nullptr);
    SimulatedExam(QList<Question> l);
    ~SimulatedExam();
    //根据下标显示题目
    void viewQuestion(int index);
    //绑定按钮
    void buttonOperate();
    //清除选中状态
    void clearSelections();
    //倒计时
    void timeOut();
    //提交
    void summit();
    int getPaperId();
    void setPaperId(int id);
private:
    Ui::SimulatedExam *ui;
    QList<Question> list;
    //当前题下标
    int index;
    QButtonGroup *buttonGroup;
    bool clearFlag = false;
    //分数
    int score = 0;
    QTimer timer;
    int timeRemaining = 90 * 60; // 倒计时时间，单位为秒
    int paper_id;
    DataBaseUtils db;
protected:
    void closeEvent(QCloseEvent *event) override; // 重写关闭事件
signals:
    void dialogClosed();
};

#endif // SIMULATEDEXAM_H
