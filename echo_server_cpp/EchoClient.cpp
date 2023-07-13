#include "EchoClient.hpp"

EchoClient::EchoClient(int argc, char* argv[]){
    if (argc != 3)
        throw std::invalid_argument("invalid arg count");
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw std::runtime_error("socket() error");
    
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));
}

void    EchoClient::conServ(){
    if (connect(sock, (struct sockaddr*)&servAdr, sizeof(servAdr)) == -1)
        throw std::runtime_error("connect() error");
    std::cout << "connected to server\n";

    while (1) {
        std::cout << "Input message(Q to quit): ";
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break ;
        write(sock, message, strlen(message));
        rdMessageLen = read(sock, message, BUF_SIZE - 1);
        message[rdMessageLen] = 0;
        std::cout << "message from server: " << message;
    }
    std::cout << "connection closed\n";
}

EchoClient::~EchoClient(){ close(sock); }

int main(int argc, char* argv[]){
    try {
        EchoClient  C(argc, argv);
        C.conServ();
    }
    catch(std::exception& e) { std::cout << e.what() << "\n"; }
    
    return 0;
}
