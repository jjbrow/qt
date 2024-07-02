
#include"login.h"
#include <QApplication>
#include <QDBusConnection>
#include<QDBusError>
#include<readinglogs.h>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    login l;
    l.show();

    // 获取屏幕的尺寸
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

    // 获取窗口的尺寸
    int windowWidth = l.width();
    int windowHeight = l.height();

    // 计算窗口显示在屏幕中间时的位置
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // 移动窗口到屏幕中间
    l.move(x, y);
    return a.exec();
}
