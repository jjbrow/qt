#include "dateutils.h"

DateUtils::DateUtils()
{

}

QString DateUtils::getCurrentDateTimeString() {
    // 获取当前日期和时间
    QDateTime current = QDateTime::currentDateTime();

    // 格式化为字符串（例如 "yyyy-MM-dd HH:mm:ss"）
    QString dateTimeString = current.toString("yyyyMMddHHmmss");

    return dateTimeString;
}
