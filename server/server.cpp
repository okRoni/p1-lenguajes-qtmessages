#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

std::map<std::string, int> activeClientSockets; // map of currently connected clients
std::mutex active_clients_mutex;
const int PORT = 12345;

void checkForSaveFile() {
    std::ifstream file("messages.txt");
    if (!file.good()) {
        std::ofstream createFile("messages.txt");
        createFile.close();
    }
}

void checkForUsersFile() {
    std::ifstream file("users.txt");
    if (!file.good()) {
        std::ofstream createFile("users.txt");
        createFile.close();
    }
}

bool isUserRegistered(const std::string& username) {
    std::ifstream file("users.txt");
    if (!file.good()) {
        std::cerr << "Failed to open users file for reading.\n";
        return false;
    }
    
    std::string line;
    bool found = false;
    while (std::getline(file, line)) {
        if (line == username) {
            found = true;
            break;
        }
    }
    file.close();
    return found;
}

void registerUser(const std::string& username) {
    std::ofstream file("users.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open users file for writing.\n";
        return;
    }
    file << username << std::endl;
    file.close();
}

// save messages by writing in file (data base)
int saveMessage(const std::string& unprocessedMessage) {
    std::string fileName = "messages.txt";
    std::ofstream file;
    
    file.open(fileName, std::ios::app); // append mode not application!!!?!?!??!?!
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return -1;
    }
    
    file << unprocessedMessage << std::endl;
    
    file.close();
    return 0;
}

// get all messages to a certain destinatary
std::vector<std::string> getMessages(const std::string& destinatary) {
    std::ifstream file("messages.txt");
    std::vector<std::string> messages;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: messages.txt" << std::endl;
        return messages;
    }

    while (std::getline(file, line)) {
        if (line.find(destinatary) != std::string::npos) {
            messages.push_back(line);
        }
    }

    file.close();
    return messages;
}

int sendOnlineMessage(const std::string& sender, const std::string& recipient, const std::string& content) {
    // check if sender and recipient are valid
    if (activeClientSockets.count(sender) == 0) {
        std::cerr << "Sender not found: " << sender << "\n";
        return -1;
    }
    if (activeClientSockets.count(recipient) == 0) {
        std::cerr << "Recipient not found: " << recipient << "\n";
        return -1;
    }

    // send message
    int senderSocket = activeClientSockets[sender];
    int recipientSocket = activeClientSockets[recipient];
    std::lock_guard<std::mutex> lock(active_clients_mutex);
    send(recipientSocket, content.c_str(), content.size(), 0);

    return 0;
}

// send a set of message to a certain destinatary
int sendDueMessages(const std::string& destinatary, const std::vector<std::string>& messages) {
    if (activeClientSockets.count(destinatary) == 0) {
        std::cerr << "Destinatary not found: " << destinatary << "\n";
        return -1;
    }

    int socket = activeClientSockets[destinatary];
    std::lock_guard<std::mutex> lock(active_clients_mutex);
    
    for (const auto& message : messages) {
        // find separators
        size_t contactSeparator = message.find("->"); // separator bwtwn sender and recipient
        size_t messageSeparator = message.find(":"); // separator bwtwn contacts and message

        std::string sender = message.substr(0, contactSeparator);
        std::string recipient = message.substr(contactSeparator + 2, messageSeparator - contactSeparator - 2);
        std::string content = message.substr(messageSeparator + 1);

        // clear white spaces after the message
        content.erase(content.find_last_not_of(" \n\r\t") + 1);

        std::string processedMessage;
        if (sender == destinatary) {
            processedMessage = "me|" + recipient  + "->" + content + "\n";
        } else {
            processedMessage = sender + "->" + content + "\n";
        }

        send(socket, processedMessage.c_str(), processedMessage.size(), 0);
    }

    return 0;
}

int processMessage(const std::string& message) {
    size_t contactSeparator = message.find("->"); // separator bwtwn sender and recipient
    size_t messageSeparator = message.find(":"); // separator bwtwn contacts and message
    // ommit invalid messages
    if (contactSeparator == std::string::npos || messageSeparator == std::string::npos) {
        std::cerr << "Invalid message format: " << message << "\n";
        return -1;
    }
    std::string sender = message.substr(0, contactSeparator);
    std::string recipient = message.substr(contactSeparator + 2, messageSeparator - contactSeparator - 2); 
    std::string content = message.substr(messageSeparator + 1);
    std::string proccesedMessage = sender + "->" + content;
    
    // save message
    if (isUserRegistered(recipient) == false) {
        std::cerr << "Recipient not registered: " << recipient << ", unable to save message.\n";
        return -1;
    }
    if (saveMessage(message) == -1) {
        std::cerr << "Failed to save message: " << proccesedMessage << "\n";
        return -1;
    }

    std::cout << "\nMessage of " << sender << " to " << recipient << ":\n" << content << "\n";

    sendOnlineMessage(sender, recipient, proccesedMessage);
    return 0;
}

void handleClient(int client_socket) {
    std::cout << "Client connected.\n";
    char buffer[1024] = {0};
    // recieve username (first thing the client sends)
    read(client_socket, buffer, 1024);
    std::string username(buffer);
    std::cout << "Username: " << username << "\n";
    { // register client
        std::lock_guard<std::mutex> lock(active_clients_mutex);
        activeClientSockets[username] = client_socket;
        std::cout << username << " connected.\n";

        // check if user is already saved in file
        if (!isUserRegistered(username)) {
            std::cout << username << " registered for the first time.\n";
            registerUser(username);
        } else {
            std::cout << username << " was already registered.\n";
        }
    }

    // put client up to date with messages
    std::vector<std::string> messages = getMessages(username);
    std::cout << "Sending " << messages.size() << " messages to " << username << "\n";
    sendDueMessages(username, messages);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) break;  // desconected client
        
        // process message
        std::string message(buffer);
        if (processMessage(message) == -1) {
            std::cerr << "Error processing message: " << message << "\n";
            continue;
        }
    }

    { // delete client of currently connected clients map
        std::lock_guard<std::mutex> lock(active_clients_mutex);
        activeClientSockets.erase(username);
        std::cout << username << " disconnected.\n";
    }
    close(client_socket);
}

int main() {
    checkForSaveFile();
    checkForUsersFile();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Add this: Allow address reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Listening on port " << PORT << "...\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        std::thread(handleClient, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
