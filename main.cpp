#include <iostream>
#include "tcpserver.h"

// Additional functions
std::string convertToString(char* a, int size) 
{
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Program constants 
const int PORT = 8080;

int main()
{
    std::cout << "Initializing tcp server ...\n";
    TcpServer server(PORT);
    if (server.Error == true)
    {
        server.Shutdown();
        return -1;
    }

    std::cout << "Starting the listener on port " << server.GetPort() << "\n";
    server.Listen();
    if (server.Error == true)
    {
        std::cerr << "Closing program.\n";
        server.Shutdown();
        return -1;
    }
    std::cout << "Listening...\n";

    // The thraed does not block at this point.
    // No incoming connection is available.
    SOCKET clientSocket;
    clientSocket = server.Accept();
    if (clientSocket != INVALID_SOCKET)
    {
        std::cout << "Connection succesful!\n";
    }
    else
    {
        std::cout << "ERROR: Connection failed!\n";
    }

    // Test using recv and send
    // Accept and echo message back to client
    char buf[4096];

    ZeroMemory(buf, 4096);

    // Wait for client to send data
    std::cout << "Waiting for the client to send data...\n";
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    std::cout << "Characters recieved: " << bytesReceived << "\n";
    if (bytesReceived == SOCKET_ERROR)
    {
        std::cerr << "Error in recv() function! Closing program\n";
    }
    
    if (bytesReceived == 0)
    {
        std::cerr << "Client disconnected!\n";
    }

    // Print the message received
    std::cout << trim(convertToString(buf, sizeof(buf))) << "\n";

    // Echo same message back
    send(clientSocket, buf, bytesReceived + 1, 0);

    return 0;
}