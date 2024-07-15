#ifndef ADDQUESTION_H
#define ADDQUESTION_H

#include <QDialog>
#include <QDebug>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStyledItemDelegate>
#include"question.h"
#include"databaseutils.h"
#include <QMessageBox>
namespace Ui {
class AddQuestion;
}

class AddQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit AddQuestion(QWidget *parent = nullptr);
    AddQuestion(int id);
    ~AddQuestion();
    // 试卷id
    int paper_id;
    //设置样式
    void customizeStyle();
    //清除选中状态
    void clearSelections();
    //按钮操作绑定
    void buttonOperate();
private:
    Ui::AddQuestion *ui;
    QButtonGroup *buttonGroup;
    DataBaseUtils db;
};

#endif // ADDQUESTION_H
