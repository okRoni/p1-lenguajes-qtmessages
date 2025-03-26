#include "messagesapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    messagesApp w;
    w.show();
    return a.exec();
}
