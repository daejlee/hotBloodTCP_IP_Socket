#ifndef ECHOSERVER_HPP
# define ECHOSERVER_HPP
# define BUF_SIZE 1024
# include <sys/socket.h>
# include <arpa/inet.h>
# include <iostream>
# include <exception>
# include <cstring>
# include <unistd.h>

class EchoServer {
    private:
        int                 servSock;
        int                 clntSock;
        char                message[BUF_SIZE];
        int                 rdMessageLen;
        struct sockaddr_in  servAdr;
        struct sockaddr_in  clntAdr;
        socklen_t           clntAdrSize;
    public:
                            EchoServer(int argc, const char* argv[]);
        void                acceptClients();
                            ~EchoServer();
};

#endif