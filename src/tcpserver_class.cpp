#include "tcpserver.h"

TcpServer::TcpServer(int port)
{
    // Initialize default values
    this->Error = false;
    this->_Port = port;

    // Initialize winsock
    if ((WSAStartup(MAKEWORD(2,2), &this->_WSAData)) != 0)
    {
        printf("ERROR: Failed to Initialize WSA Winsock.\n");
        this->Error = true;
    }
    else
    {
        struct addrinfo *result = NULL, hints;

        ZeroMemory(&hints, sizeof (hints));

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Dumb work around to obatin const char * for a dumb func
        std::string sPort = std::to_string(this->_Port); // sPort is supposed to be the port number
        char const *pchar = sPort.c_str();

        // The dumb func that resolve the local address and port to be used by the server
        if((getaddrinfo(NULL, pchar, &hints, &result)) != 0 )
        {
            printf("ERROR: Failed to get address info.\n");
            this->Error = true;
        }
        else
        {
            // Create a socket
            this->_Server = INVALID_SOCKET;
            this->_Server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (this->_Server == INVALID_SOCKET)
            {
                printf("Error at socket(): %ld\n", WSAGetLastError());
                freeaddrinfo(result);
                this->Error = true;
            }
            // Bind the ip address and port to a socket
            else
            {
                if((bind(this->_Server, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR)
                {
                    printf("Bind failed with error: %d\n", WSAGetLastError());
                    this->CloseServerSocket();
                    freeaddrinfo(result);
                    this->Error = true;
                }

                // this->_Addr.sin_family = AF_INET;
                // this->_Addr.sin_port = htons(this->_Port);
                // this->_Addr.sin_addr.S_un.S_addr = INADDR_ANY;
                // int bindError = bind(this->_Server, (sockaddr*)&this->_Addr, sizeof(this->_Addr));
                // if ( bindError == SOCKET_ERROR)
                //     Error = true;
            }
        }
    }
}

void TcpServer::Listen()
{
    // Places the socket in a state in which it is listening for an incoming connection.
    if (listen(this->_Server, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        this->CloseServerSocket();
        this->Error = true;
    }
}

// Return UINT_PTR or -1 if error ocurred
SOCKET TcpServer::Accept()
{
    return accept(this->_Server, NULL, NULL);
}

std::string GetClientInfo(const sockaddr_in *client)
{
    char host[NI_MAXHOST];      // Client remote name
    char service[NI_MAXSERV];   // Service (port) the client is connected on

    ZeroMemory(host, NI_MAXHOST); // Same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if(getnameinfo((sockaddr*)client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::string hostStr(host);
        std::string serviceStr(service);
        std::string info = hostStr + " connected on port " + serviceStr;
        return info;
    }
    else
    {
        std::string hostStr(host);
        std::string info = hostStr + " connected on port " + std::to_string(ntohs((*client).sin_port));
        return info;
        // inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
    }
}

int TcpServer::GetPort()
{
    return this->_Port;
}

int TcpServer::CloseServerSocket()
{
    return closesocket(this->_Server);
}

void TcpServer::Shutdown()
{
    this->~TcpServer();
}

TcpServer::~TcpServer()
{
    WSACleanup();
}