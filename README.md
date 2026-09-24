# C++ TCP Multi-Client Chat Application

A terminal-based multi-client chat application built using **C++ and TCP socket programming**. The application follows a client-server architecture where multiple clients can connect to a central server and communicate in real time.

## Features

- TCP-based client-server communication
- Multiple clients can connect simultaneously
- Username-based messaging
- Broadcast messages between connected clients
- `/users` command to view currently connected users
- Multithreaded client handling
- Thread-safe client management using mutexes
- Graceful client connection and disconnection handling

## Tech Stack

- C++
- TCP/IP
- Winsock2
- STL
- `std::thread`
- `std::mutex`

## Architecture

```text
                 TCP Connection
                       |
                       v
                +-------------+
                |   SERVER    |
                |   Port 8080 |
                +------+------+
                       |
              +--------+--------+
              |        |        |
              v        v        v
           Client   Client   Client
    he server listens for incoming TCP connections. Each connected client is handled by a separate thread. Messages received from a client are broadcast to the other connected clients.

Project Structure
cpp-tcp-chat/
│
├── client.cpp
├── server.cpp
├── README.md
├── LICENSE
└── .gitignore
Requirements
Windows
MinGW-w64 / GCC
VS Code
How to Run
1. Clone the repository
git clone https://github.com/ishitasalgotra/cpp-tcp-chat.git
cd cpp-tcp-chat
2. Compile the server
g++ server.cpp -o chatserver.exe -lws2_32
3. Compile the client
g++ client.cpp -o client.exe -lws2_32
4. Start the server
.\chatserver.exe

The server runs on port 8080.

5. Start a client

Open a new terminal:

.\client.exe

Enter a username when prompted.

To test multiple clients, open additional terminals and run:

.\client.exe

again.

Chat Commands
View Online Users

Inside a connected client, type:

/users

Example:

Online users:
- ishita
- rahul
Example
Server is running on port 8080...

Connected to server
Enter your username: ishita

rahul has joined the chat!

ishita: Hello Rahul!
rahul: Hey Ishita!
Key Concepts
TCP Socket Programming

The application uses TCP sockets to establish reliable communication between clients and the server.

Client-Server Architecture

The server listens on port 8080 and accepts incoming client connections. Clients connect to the server using the local loopback address.

Multithreading

A separate std::thread is created for each connected client, allowing multiple clients to communicate concurrently.

Mutex Synchronization

A std::mutex protects the shared client collection from simultaneous access by multiple client threads.

STL

The project uses C++ STL containers such as std::map to maintain connected clients and their usernames.

Learning Outcomes

This project demonstrates practical understanding of:

TCP/IP networking
Socket programming
Client-server architecture
Multithreading
Mutex-based synchronization
Concurrent programming
STL containers
Network I/O
Basic message framing
Future Improvements

Possible future improvements include:

Private messaging
Message timestamps
Chat history
User authentication
End-to-end encryption
Graphical user interface
License

This project is licensed under the MIT License.