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
