#include <iostream>
#include <string>
#include <ws2tcpip.h>

std::string convertToString(char* a, int size) 
{
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 

int main()
{
    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2,2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        std::cerr << "ERROR: Can't Initialize winsock! Closing program.\n";
        return 1;
    }
    
    // Create a socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == INVALID_SOCKET)
    {
        std::cerr << "ERROR: Can't create the socket! Closing program\n";
        return 1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(listener, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is for listenning
    std::cout << "Listener socket created. Waiting for connection...\n";
    listen(listener, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listener, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Client remote name
    char service[NI_MAXSERV];   // Service (port) the client is connected on

    ZeroMemory(host, NI_MAXHOST); // Same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << "\n";
    }
    else
    {
        //inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << "\n";
    }

    // Close listening socket
    closesocket(listener);

    // Loop: accept and echo message back to client
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
    std::cout << convertToString(buf, sizeof(buf)) << "\n";

    // Echo same message back
    send(clientSocket, buf, bytesReceived + 1, 0);

    // Close the socket
    closesocket(clientSocket);

    // Shutdown winsock
    WSACleanup();

    std::cout << "Closing program\n";
    return 0;
}