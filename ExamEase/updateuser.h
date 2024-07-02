#ifndef UPDATEUSER_H
#define UPDATEUSER_H
#include"databaseutils.h"
#include <QWidget>

namespace Ui {
class updateUser;
}

class updateUser : public QWidget
{
    Q_OBJECT

public:
    explicit updateUser(QWidget *parent = nullptr);
    ~updateUser();

private:
    Ui::updateUser *ui;
};

#endif // UPDATEUSER_H
