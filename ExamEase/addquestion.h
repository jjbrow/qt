#ifndef ADDQUESTION_H
#define ADDQUESTION_H

#include <QDialog>
#include <QDebug>
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
private:
    Ui::AddQuestion *ui;
};

#endif // ADDQUESTION_H
