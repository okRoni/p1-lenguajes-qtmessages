#ifndef MESSAGESAPP_H
#define MESSAGESAPP_H

#include <QWidget>
#include "chat.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class messagesApp;
}
QT_END_NAMESPACE

class messagesApp : public QWidget
{
    Q_OBJECT

public:
    messagesApp(QWidget *parent = nullptr);
    ~messagesApp();
    chat *chatInstance;

private slots:
    void on_sendButton_clicked();

private:
    Ui::messagesApp *ui;
};
#endif // MESSAGESAPP_H
