#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <time.h>
#include <netdb.h>

#define MAXLINE 4096

int main(int argc,char **argv) {
        int sockfd,n;
        struct sockaddr_in servaddr;
        char recvbuf[MAXLINE+1],sendbuf[MAXLINE+1];
        fd_set rset;
        //인자 갯수 확인
        if(argc!=4) {
                printf("usage: a.out <IPaddress> <Port> <ID>\n");
                exit(0);
        }
        //서버 소켓 생성
        if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) {
                printf("Socket Error\n");
                exit(0);
        }
        //sockaddr_in 구조체 초기화
        bzero(&servaddr,sizeof(servaddr));
        //servaddr 구조체 설정(IP, PORT 설정)
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=htons(atoi(argv[2]));
        servaddr.sin_addr.s_addr=inet_addr(argv[1]);
        //서버접속
        if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0) {
                printf("Connect Error\n");
                exit(0);
        }
        //[address <IPaddress:Port> is connected] 형태로 출력
        printf("[address %s:%s is connected]\n",argv[1],argv[2]);
        //ID를 서버에 알려줌
        write(sockfd,argv[3],strlen(argv[3]));
        while(1) {
                FD_ZERO(&rset);         //rset을 초기화
                FD_SET(0,&rset);        //rset에 0을 setting
                FD_SET(sockfd,&rset);   //rset에 sockfd를 setting
                if(select(sockfd+1,&rset,NULL,NULL,NULL)<0) {
                        printf("Select Error\n");
                        exit(0);
                }
                if(FD_ISSET(0,&rset)) {         //0이 요청될 때(키보드 입력시)
                        fgets(sendbuf,MAXLINE,stdin);
                        sendbuf[strlen(sendbuf)-1]=0x00;
                        write(sockfd,sendbuf,sizeof(sendbuf)-1);        //서버에 메시지를 보냄
                        if(strcmp(sendbuf,"/quit")==0) {        //'/quit'를 입력 받을 경우
                                exit(0);
                        }
                        else if(strcmp(sendbuf,"/list")==0) {   //'/list'를 입력 받을 경우
                                printf("[current user is ...]\n");
                        }
                }
                if(FD_ISSET(sockfd,&rset)) {    //sockfd가 요청될 때
                        bzero(&recvbuf,sizeof(recvbuf));
                        if((n=read(sockfd,recvbuf,MAXLINE))==0) {       //서버로 부터 메시지를 받음
                                printf("server terminated prematurely\n");
                                exit(0);
                        }
                        recvbuf[n]=0;
                        puts(recvbuf);
                }
        }
        return 0;
}