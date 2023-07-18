#include "EchoServer.hpp"

EchoServer::EchoServer() {}

EchoServer::~EchoServer() {
  close(serv_sock_);
  std::cout << "Server closed.\n";
}

/**
* @brief 에코셀렉트 서버 구동에 필요한 작업들을 수행합니다.
* @details 서버 소켓을 생성하고 서버 주소 구조체를 초기화합니다.
* bind()함수, listen()함수를 호출합니다. 연결요청 대기 큐의 크기는 15입니다.
* @param argv main문에 입력된 2차원 char 배열 argv입니다.
* @exception bind()함수, listen()함수에서 에러 발생 시 throw합니다.
*/
void  EchoServer::prepServ(char* argv[]) {
  serv_sock_ = socket(PF_INET, SOCK_STREAM, 0);
  std::memset(&serv_adr_, 0, sizeof(serv_adr_));
  serv_adr_.sin_family = AF_INET;
  serv_adr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr_.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock_, (struct sockaddr*)&serv_adr_, sizeof(serv_adr_) == -1))
    throw std::runtime_error("bind() ERROR");
  else if (listen(serv_sock_, 15) == -1)
    throw std::runtime_error("listen() ERROR");
}

/**
* @brief 클라이언트 연결 요청을 수락합니다.
* @details accept()함수를 호출하고, 연결 요청을 수락한 클라이언트 소켓을 reads에 등록합니다.
* 새롭게 늘어난 clnt_sock에 대하
* @param argv main문에 입력된 2차원 char 배열 argv입니다.
* @exception bind()함수, listen()함수에서 에러 발생 시 throw합니다.
*/
int  EchoServer::acceptClient(int fd_max, fd_set* reads_adr) {
  socklen_t           adr_size;
  struct sockaddr_in  clnt_adr;

  adr_size = sizeof(clnt_adr);
  if ((clnt_sock_ = accept(serv_sock_, (struct sockaddr*)&clnt_adr, &adr_size)) == -1)
    throw std::runtime_error("accept() ERROR");
  FD_SET(clnt_sock_, reads_adr);
  if (fd_max < clnt_sock_)
    fd_max = clnt_sock_;
  std::cout << "Connected client: " << clnt_sock_ << "\n";
  return fd_max;
}

void  EchoServer::receiveMessage(int& fd_i, fd_set* reads_adr) {
  int   rd_str_len;
  char  buf[BUF_SIZE];

  if (!(rd_str_len = read(fd_i, buf, BUF_SIZE))) {
    FD_CLR(fd_i, reads_adr);
    close(fd_i);
    std::cout << "Closed client: " << clnt_sock_ << "\n";
  }
  else
    write(fd_i, buf, rd_str_len);
}

void  EchoServer::launchServ() {
  int fd_max, sel_ret;
  struct timeval  timeout;
  fd_set  reads, r_reads;

  FD_ZERO(&reads);
  FD_SET(serv_sock_, &reads);
  fd_max = serv_sock_;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  while (1) {
    r_reads = reads;
    if ((sel_ret = select(fd_max + 1, &r_reads, 0, 0, &timeout) == -1))
      throw std::runtime_error("select() ERROR");
    else if (!sel_ret)
      continue;

    for (int fd_i = 0; fd_i < fd_max + 1; fd_i++) {
      if (FD_ISSET(fd_i, &r_reads)) {
        if (fd_i == serv_sock_) //accept request
          fd_max = acceptClient(fd_max, &reads);
        else //message received
          receiveMessage(fd_i, &reads);
      }
    }
  }
}

int main(int argc, char* argv[]) {
  EchoServer  S;
  try {
  S.prepServ(argv);
  S.launchServ();
  } catch (std::exception& e){ std::cerr << e.what(); }
  
  return 0;
}