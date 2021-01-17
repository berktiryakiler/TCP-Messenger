#include "server.h"
#include "welcome.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    welcome w;
    w.setWindowTitle("TCP Messenger");
    w.show();

    return a.exec();
}
