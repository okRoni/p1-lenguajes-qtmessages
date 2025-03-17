#ifndef MESSAGESAPP_H
#define MESSAGESAPP_H

#include <QWidget>

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

private:
    Ui::messagesApp *ui;
};
#endif // MESSAGESAPP_H
