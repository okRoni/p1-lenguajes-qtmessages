#include "messagesapp.h"
#include "connection.h"

#include <QApplication>
#include <QInputDialog>
#include <QString>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    messagesApp w;

    // solicitar nombre de usuario
    bool ok;
    QString username = QInputDialog::getText(nullptr, "Login",
                                             "Ingresa tu nombre de usuario:",
                                             QLineEdit::Normal, "", &ok);
    if (!ok || username.isEmpty()) {
        QMessageBox::critical(nullptr, "Error", "Debes ingresar un nombre de usuario.");
        return 1;
    }

    connection net;
    net.start(username.toStdString());

    w.show();
    return a.exec();
}
