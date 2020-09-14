#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <ws2tcpip.h>

// This class includes the windows socket 2 header file
// You can use closesocket(SOCKET s) function to close any opened socket

class TcpServer
{
    public:
        TcpServer(int port);
        ~TcpServer();
        void Listen();
        SOCKET Accept();
        std::string GetClientInfo(const sockaddr_in *client);
        int GetPort();
        int CloseServerSocket();
        void Shutdown();
        bool Error;
    private:
        int _Port;
        WSADATA _WSAData;
        SOCKET _Server;
        SOCKADDR_IN _Addr;
};

#endif // TCPSERVER_H