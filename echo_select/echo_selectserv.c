#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
  
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 3)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads); //reads에 serv_sock의 정보 등록
	fd_max=serv_sock; //생성된 serv_sock의 FD를 최대 FD로 등록

	while(1) {
		cpy_reads=reads; //reads 복사
		timeout.tv_sec=5;
		timeout.tv_usec=5000; //타임아웃용 구조체 초기화

		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
    //셀렉트 함수 호출, fd_max까지 감시하며 수신된 데이터를 감지한다. 타임아웃은 5초이다
    //초기 fd_max에는 서버 소켓만이 들어있다.
			break; //오류가 발생 시 탈출한다.
		if(fd_num==0) //변화가 일어나지 않으면 다시 호출한다.
			continue;

		for(i=0; i<fd_max+1; i++) { //fd_max까지 반복한다.
			if(FD_ISSET(i, &cpy_reads)) { //i FD가 1이면, 변화가 일어났음.
				if(i==serv_sock) { //서버 소켓에 수신된 데이터가 있음. connection request!
					adr_sz=sizeof(clnt_adr);
					clnt_sock=
						accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
            //연결대기큐에 대기중인 클라이언트 연결
					FD_SET(clnt_sock, &reads); //연결한 클라이언트 reads에 추가
					if(fd_max<clnt_sock) //소켓 추가되었으므로 fd_max 업데이트
						fd_max=clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else { //클라이언트 소켓에 수신된 데이터가 있음. read message!
					if(!(str_len = read(i, buf, BUF_SIZE))) { //EOF수신됨. close request!
						FD_CLR(i, &reads); //reads에서 해당 클라이언트 클리어
						close(i);
						printf("closed client: %d \n", i);
					}
					else
						write(i, buf, str_len); //클라이언트에게 읽어들인 메시지 다시 echo!
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}