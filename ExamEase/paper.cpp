#include "paper.h"

Paper::Paper()
    : m_id(-1), m_total(0)
{
}

int Paper::id() const
{
    return m_id;
}

void Paper::setId(int id)
{
    m_id = id;
}

QString Paper::name() const
{
    return m_name;
}

void Paper::setName(const QString &name)
{
    m_name = name;
}

int Paper::total() const
{
    return m_total;
}

void Paper::setTotal(int total)
{
    m_total = total;
}

QDateTime Paper::createDate() const
{
    return m_createDate;
}

void Paper::setCreateDate(const QDateTime &createDate)
{
    m_createDate = createDate;
}
