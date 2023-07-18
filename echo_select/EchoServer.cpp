#include "EchoServer.hpp"

EchoServer::EchoServer() {}

void    EchoServer::prepServer(char* argv[]) {
    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == -1)
        throw std::runtime_error("socket() error");
    
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if (bind(servSock, (struct sockaddr*)&servAdr, sizeof(servAdr)) == -1)
        throw std::runtime_error("bind() error");
    else if (listen(servSock, 5) == -1)
        throw std::runtime_error("listen() error");
    clntAdrSize = sizeof(clntAdr);
}

void    EchoServer::acceptClients() {
    for (int i = 0; i < 5; i++){
        clntSock = accept(servSock, (struct sockaddr*)&clntAdr, &clntAdrSize);
        if (clntSock == -1)
            throw std::runtime_error("accept() error");
        std::cout << "Connected client" << i + 1 << "\n";
        while (rdMessageLen = read(clntSock, message, BUF_SIZE))
            write(clntSock, message, rdMessageLen);
        close(clntSock);
    }
}

EchoServer::~EchoServer() {
    close(servSock);
    std::cout << "Server closed\n";
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2)
            throw std::invalid_argument("invalid arg count");
        EchoServer  S;
        S.prepServer(argv);
        S.acceptClients();
        }
    catch(std::exception& e) { std::cout << e.what() << "\n"; }

    return 0;
}
