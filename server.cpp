#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <string>
#include <mutex>
#include <map>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

map<SOCKET, string> clients;
mutex client_mutex;

void broadcast(string message, SOCKET sender_socket) {
    lock_guard<mutex> lock(client_mutex);

    for (auto& client : clients) {
        if (client.first != sender_socket) {
            send(client.first, message.c_str(),
                 static_cast<int>(message.size()), 0);
        }
    }
}

void handle_client(SOCKET clientSocket) {

    char buffer[1024];
    string data;
    string username;

    // Receive username
    while (true) {

        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytes <= 0) {
            closesocket(clientSocket);
            return;
        }

        data.append(buffer, bytes);

        size_t pos = data.find('\n');

        if (pos != string::npos) {

            username = data.substr(0, pos);
            data.erase(0, pos + 1);

            break;
        }
    }

    // Add client
    {
        lock_guard<mutex> lock(client_mutex);
        clients[clientSocket] = username;
    }

    string joinMsg = username + " has joined the chat!\n";

    cout << joinMsg;

    broadcast(joinMsg, clientSocket);

    // Handle messages
    while (true) {

        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytes <= 0) {

            cout << username << " disconnected" << endl;

            {
                lock_guard<mutex> lock(client_mutex);
                clients.erase(clientSocket);
            }

            string leaveMsg = username + " has left the chat!\n";

            broadcast(leaveMsg, clientSocket);

            closesocket(clientSocket);

            break;
        }

        data.append(buffer, bytes);

        size_t pos;

        while ((pos = data.find('\n')) != string::npos) {

            string msg = data.substr(0, pos);

            if (msg.empty()) {
                data.erase(0, pos + 1);
                continue;
            }
            
            if (msg == "/users") {
    string userList = "Online users:\n";

    {
        lock_guard<mutex> lock(client_mutex);

        for (auto& client : clients) {
            userList += "- " + client.second + "\n";
        }
    }

    send(clientSocket, userList.c_str(),
         static_cast<int>(userList.size()), 0);

    data.erase(0, pos + 1);
    continue;
}

            string fullMsg = username + ": " + msg + "\n";

            cout << fullMsg;

            broadcast(fullMsg, clientSocket);

            data.erase(0, pos + 1);
        }
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
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET) {

        cout << "Socket creation failed" << endl;

        WSACleanup();

        return 1;
    }

    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(serverSocket,
             reinterpret_cast<sockaddr*>(&serverAddr),
             sizeof(serverAddr)) == SOCKET_ERROR) {

        cout << "Bind failed" << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    // Listen
    if (listen(serverSocket, 5) == SOCKET_ERROR) {

        cout << "Listen failed" << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    cout << "Server is running on port 8080..." << endl;

    // Accept clients
    while (true) {

        SOCKET clientSocket =
            accept(serverSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        thread t(handle_client, clientSocket);
        t.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}