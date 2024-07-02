#include "readinglogs.h"

ReadingLogs::ReadingLogs(QObject *parent):QObject(parent)
{

}

void ReadingLogs::reading(const QString &str){
    //当是启动时 实时读取数据
    if(messageFlag){
        //发射信号 数据显示到窗口
        emit messageReceived(str);
    }else{

        //if(messageCache.length()<messageNum){
        messageCache.enqueue(str);
        //}
        if(messageCache.count()> messageNum)
        {
            messageCache.removeAt(0);
        }

    }
}
