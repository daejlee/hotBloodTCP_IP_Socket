#ifndef ECHOSERVER_HPP
# define ECHOSERVER_HPP
# define BUF_SIZE 1024
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <sys/select.h>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <exception>

class EchoServer {
  public:
                        EchoServer();
                        ~EchoServer();
    void                prepServ(char* argv[]);
    void                launchServ();
  private:
    int                 acceptClient(int fd_max, fd_set* reads_adr);
    void                receiveMessage(int& fd_i, fd_set* reads_adr);
    int                 serv_sock_, clnt_sock_;
    struct sockaddr_in  serv_adr_;
};

#endif