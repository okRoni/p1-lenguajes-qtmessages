#include "messagesapp.h"
#include "./ui_messagesapp.h"

messagesApp::messagesApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::messagesApp)
{
    ui->setupUi(this);
}

messagesApp::~messagesApp()
{
    delete ui;
}

void messagesApp::on_sendButton_clicked()
{
    QString message = ui->messageInput->toPlainText();
    ui->chatText->append(message.prepend("\n**message:** "));
    ui->messageInput->clear();
}

