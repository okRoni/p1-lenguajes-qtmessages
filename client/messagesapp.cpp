#include "messagesapp.h"
#include "./ui_messagesapp.h"
#include <QMessageBox>

messagesApp::messagesApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::messagesApp)
{
    ui->setupUi(this);
    
    // instace chat
    chatInstance = new chat();
    chatInstance->setMessagesApp(this);  // Add this line
}

messagesApp::~messagesApp()
{
    delete ui;
}

void messagesApp::on_sendButton_clicked()
{
    QString message = ui->messageInput->toPlainText();
    QString contact = ui->contactText->toPlainText();

    if (message.isEmpty() || contact.isEmpty()) {
        QMessageBox::critical(this, "Error", "Debes ingresar un mensaje y un destinatario.");
        return;
    }

    ui->messageInput->clear();

    chatInstance->sendMessage(contact.toStdString(), message.toStdString());
}


void messagesApp::on_contactsList_itemClicked(QListWidgetItem *item)
{
    QString contact = item->text();
    ui->contactText->setPlainText(contact);
    ui->chatText->clear();
    ui->messageInput->clear();
    // disable contactText
    ui->contactText->setEnabled(false);

    chatInstance->loadContactMessages(contact.toStdString());
}


void messagesApp::on_newChatButton_clicked()
{
    ui->contactText->clear();
    ui->chatText->clear();
    ui->messageInput->clear();

    // enable contactText
    ui->contactText->setEnabled(true);
    ui->contactText->setPlaceholderText("Enter contact name");

    // un select contacts list
    ui->contactsList->setCurrentItem(nullptr);
}

