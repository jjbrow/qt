#ifndef SIMULATEDEXAM_H
#define SIMULATEDEXAM_H

#include <QDialog>
#include"question.h"
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include"question.h"
#include"databaseutils.h"
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
private:
    Ui::SimulatedExam *ui;
    QList<Question> list;
    //当前题下标
    int index;
};

#endif // SIMULATEDEXAM_H
