#ifndef ECHOCLIENT_HPP
# define ECHOCLIENT_HPP
# define BUF_SIZE 1024
# include <sys/socket.h>
# include <arpa/inet.h>
# include <iostream>
# include <exception>
# include <cstring>
# include <unistd.h>

class EchoClient
{
    private:
        int                 sock;
        char                message[BUF_SIZE];
        struct sockaddr_in  servAdr;
    public:
                            EchoClient(int argc, char* argv[]);
        void                conServ();
                            ~EchoClient();
};

#endif