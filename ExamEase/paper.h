#ifndef PAPER_H
#define PAPER_H

#include <QString>
#include <QDateTime>

class Paper
{
public:
    Paper();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    int total() const;
    void setTotal(int total);

    QDateTime createDate() const;
    void setCreateDate(const QDateTime &createDate);


private:
    int m_id;
    QString m_name;
    int m_total;
    QDateTime m_createDate;
};

#endif // PAPER_H
