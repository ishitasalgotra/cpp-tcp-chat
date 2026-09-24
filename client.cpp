#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receiveMessages(SOCKET sock) {

    char buffer[1024];
    string data;

    while (true) {

        int bytes = recv(sock, buffer, sizeof(buffer), 0);

        if (bytes <= 0) {

            cout << "Disconnected from server" << endl;
            break;
        }

        data.append(buffer, bytes);

        size_t pos;

        while ((pos = data.find('\n')) != string::npos) {

            string msg = data.substr(0, pos);

            if (!msg.empty()) {
                cout << msg << endl;
            }

            data.erase(0, pos + 1);
        }
    }
}

void sendMessages(SOCKET sock) {

    while (true) {

        string message;

        getline(cin, message);

        message += "\n";

        send(sock,
             message.c_str(),
             static_cast<int>(message.size()),
             0);
    }
}

int main() {

    // Initialize Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        cout << "WSAStartup failed" << endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket =
        socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET) {

        cout << "Socket creation failed" << endl;

        WSACleanup();

        return 1;
    }

    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddr.sin_addr
    );

    // Connect to server
    if (connect(
            clientSocket,
            reinterpret_cast<sockaddr*>(&serverAddr),
            sizeof(serverAddr)
        ) == SOCKET_ERROR) {

        cout << "Connection failed" << endl;

        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    cout << "Connected to server" << endl;

    // Username
    string username;

    cout << "Enter your username: ";

    getline(cin, username);

    username += "\n";

    send(
        clientSocket,
        username.c_str(),
        static_cast<int>(username.size()),
        0
    );

    // Start receiving and sending threads
    thread receiveThread(receiveMessages, clientSocket);
    thread sendThread(sendMessages, clientSocket);

    receiveThread.join();

    sendThread.detach();

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}