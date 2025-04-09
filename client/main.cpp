#include "messagesapp.h"
#include "connection.h"

#include <QApplication>
#include <QInputDialog>
#include <QString>
#include <QMessageBox>
#include <fstream>

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

    connection& net = connection::getConnection();
    net.registerUser(username.toStdString());

    // check local save file
    std::ifstream file("messages.txt");
    if (!file.good()) {
        std::ofstream createFile("messages.txt");
        createFile.close();
    }
    // clean local message file
    std::ofstream clearFile("messages.txt", std::ios::trunc);
    file.close();
    

    w.show();
    int result = a.exec();

    connection::destroyConnection();  // Clean up the singleton instance
    return result;
}
