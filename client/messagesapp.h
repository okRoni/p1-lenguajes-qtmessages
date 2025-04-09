#ifndef MESSAGESAPP_H
#define MESSAGESAPP_H

#include <QWidget>
#include "chat.h"
#include <QListWidget>

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

    void on_contactsList_itemClicked(QListWidgetItem *item);

    void on_newChatButton_clicked();

private:
    Ui::messagesApp *ui;

public:
    Ui::messagesApp *getUi() { return ui; }  // Getter for ui
};
#endif // MESSAGESAPP_H
