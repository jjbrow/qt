#ifndef ADDQUESTION_H
#define ADDQUESTION_H

#include <QDialog>
#include <QDebug>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStyledItemDelegate>
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
private:
    Ui::AddQuestion *ui;
    QButtonGroup *buttonGroup;
};

#endif // ADDQUESTION_H
