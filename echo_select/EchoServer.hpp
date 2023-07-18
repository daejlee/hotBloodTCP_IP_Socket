#ifndef ECHOSERVER_HPP
# define ECHOSERVER_HPP
# define BUF_SIZE 1024
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <iostream>
# include <exception>
# include <cstring>
# include <unistd.h>

class EchoServer {
    public:
                            EchoServer();
        void                prepServer(char* argv[]);
        void                acceptClients();
                            ~EchoServer();
    private:
        int                 servSock, clntSock, rdMessageLen;
        char                message[BUF_SIZE];
        struct sockaddr_in  servAdr, clntAdr;
        struct timeval      timeout;
        fd_set              reads, cpyReads;
        socklen_t           adrSize;
};

#endif