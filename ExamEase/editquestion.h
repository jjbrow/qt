#ifndef EDITQUESTION_H
#define EDITQUESTION_H

#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "paper.h"
#include "addquestion.h"
#include <QDebug>
#include "databaseutils.h"
#include<QPushButton>
namespace Ui {
class EditQuestion;
}

class EditQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit EditQuestion(QWidget *parent = nullptr);
    EditQuestion(int id);
    ~EditQuestion();

    //表格初始化
    void initTable();
    //设置第几列不可编辑
    void makeColumnNonEditable(QTableWidget *tableWidget, int column);
    //设置列表按钮
    void setupButtonsForPaper(QTableWidget *tableWidget, int row, const Question &p, const QList<Question> &list);
    //刷新表格
    void refreshTable();

private:
    Ui::EditQuestion *ui;
    // 试卷id
    int paper_id;
    DataBaseUtils db;
signals:
    void dialogClosed();
protected:
    void closeEvent(QCloseEvent *event) override; // 重写关闭事件
};

#endif // EDITQUESTION_H
