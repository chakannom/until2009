#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

#define LISTENQ 1024
#define MAXLINE 4096
#define MAXCONN 100     //최대 접속자

int main(int argc,char **argv) {
        int servfd,clinfd,clilen;
        int clients[MAXCONN],clients_cnt=0;
        struct sockaddr_in servaddr,clinaddr;
        int maxfdp1;
        fd_set rset;
        int i,j,n;
        char tmp[15];
        char recvbuf[MAXLINE+1],sendbuf[MAXLINE+1],clientsID[MAXCONN][255];
        //인자 갯수 확인
        if(argc!=2) {
                printf("usage: %s <Port>\n",argv[0]);
                exit(0);
        }
        //서버 소켓 생성
        if((servfd=socket(AF_INET,SOCK_STREAM,0))<0) {
                printf("Socket Error\n");
                exit(0);
        }
        //sockaddr_in 구조체 초기화
        bzero(&servaddr,sizeof(servaddr));
        bzero(&clinaddr,sizeof(clinaddr));
        //servaddr 구조체 설정(IP, PORT 설정)
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_port=htons(atoi(argv[1]));
        //소켓 Bind
        if(bind(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0) {
                printf("Bind Error\n");
                exit(0);
        }
        //소켓 Listen
        if(listen(servfd,LISTENQ)<0) {
                printf("Listen Error\n");
                exit(0);
        }
        //자신의 IPaddress 찾기
        char hostname[255]={0};
        struct hostent *hostinfo;
        if(gethostname(hostname,sizeof(hostname))==0) {
                if((hostinfo=gethostbyname(hostname))!=NULL) {
                        printf("[Server address is %s:%s]\n",(char*)inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list),argv[1]);
                }
        }

        while(1) {
                FD_ZERO(&rset);         //rest를 초기화
                FD_SET(0,&rset);        //rest에 0을 setting
                FD_SET(servfd,&rset);   //rset에 servfd를 setting
                maxfdp1=servfd+1;
                for(i=0;i<clients_cnt;i++) {
                        FD_SET(clients[i],&rset);       //rset에 clients를 setting
                        maxfdp1=max(maxfdp1-1,clients[i])+1;    //maxfdp1에 최대값 넣기
                }
                if(select(maxfdp1,&rset,NULL,NULL,NULL)<0) {    //select 설정
                        printf("Select Error\n");
                        exit(0);
                }
                if(FD_ISSET(0,&rset)) { //0이 요청될 때(키보드 입력)
                        fgets(tmp,15,stdin);
                        tmp[strlen(tmp)-1]=0x00;
                        if(strcmp(tmp,"/quit")==0) {    //'/quit' Options일 떄
                                exit(0);
                        }
                        else if(strcmp(tmp,"/list")==0) {       //'/list' Options일 때
                                printf("[current user is ...]\n");
                                for(j=0;j<clients_cnt;j++) {
                                        clilen=sizeof(clinaddr);
                                        if(getpeername(clients[j],(struct sockaddr*)&clinaddr,&clilen)<0) {     //클라이언트들의 정보를 받아옴
                                                printf("Getpeername Error\n");
                                                exit(0);
                                        }
                                        printf("[%s from %s:%d]\n",clientsID[j],(char*)inet_ntoa(clinaddr.sin_addr),ntohs(clinaddr.sin_port));
                                }
                        }
                }
                else if(FD_ISSET(servfd,&rset)) {       //servfd가 요청될 때
                        clilen=sizeof(clinaddr);
                        //Accpet(client 접속)
                        if((clinfd=accept(servfd,(struct sockaddr*)&clinaddr,&clilen))<0) {
                                printf("Accept Error\n");
                                exit(0);
                        }
                        //<ID> si connected from ~~ 출력부분
                        n=read(clinfd,clientsID[clients_cnt],255);
                        clientsID[clients_cnt][n]=0;
                        printf("%s is connected from %s\n",clientsID[clients_cnt],(char*)inet_ntoa(clinaddr.sin_addr));
                        //접속한 클라이언트들을 배열에 저장
                        if(clients_cnt<MAXCONN) {
                                clients[clients_cnt]=clinfd;    //새 클라이언트를 fd배열에 저장
                                clients_cnt++;
                        }
                        else {
                                printf("too many clients\n");
                                exit(0);
                        }
                        //접속되어 있는 클라이언트들에게 새로운 클라이언트가 접속한 것을 알림
                        //[<IP> is connected] 형태로 메시지를 보냄
                        sprintf(sendbuf,"[%s is connected]",clientsID[clients_cnt-1]);
                        for(i=0;i<clients_cnt;i++) {
                                if(clients[i]!=clinfd) {
                                        write(clients[i],sendbuf,sizeof(sendbuf)-1);
                                }
                        }
                }
                for(i=0;i<clients_cnt;i++) {
                        if(FD_ISSET(clients[i],&rset)) {        //clients에서 요청이 있으면
                                bzero(&recvbuf,sizeof(recvbuf));
                                if((n=read(clients[i],recvbuf,MAXLINE))>0) {    //메시지를 받아서
                                        if(strcmp(recvbuf,"/quit")==0)  {       //'/quit'메시지를 받았을 경우 연결을 끊는다.
                                                clilen=sizeof(clinaddr);
                                                if(getpeername(clients[i],(struct sockaddr*)&clinaddr,&clilen)<0) {
                                                        printf("Getpeername Error\n");
                                                        exit(0);
                                                }
                                                close(clients[i]);
                                                printf("%s is leaved at %s\n",clientsID[i],(char*)inet_ntoa(clinaddr.sin_addr));
                                                sprintf(sendbuf,"[%s is leaved]",clientsID[i]);
                                                clients_cnt--;
                                                for(j=i;j<clients_cnt;j++) {
                                                        clients[j]=clients[j+1];
                                                        strcpy(clientsID[j],clientsID[j+1]);
                                                }
                                                for(j=0;j<clients_cnt;j++) {
                                                        write(clients[j],sendbuf,sizeof(sendbuf)-1);
                                                }
                                                continue;
                                        }
                                        else if(strcmp(recvbuf,"/list")==0) {   //'/list'메시지를 받았을 경우
                                                for(j=0;j<clients_cnt;j++) {
                                                        clilen=sizeof(clinaddr);
                                                        if(getpeername(clients[j],(struct sockaddr*)&clinaddr,&clilen)<0) {
                                                                printf("Getpeername Error\n");
                                                                exit(0);
                                                        }
                                                        sprintf(sendbuf,"[%s from %s:%d]",clientsID[j],(char*)inet_ntoa(clinaddr.sin_addr),ntohs(clinaddr.sin_port));
                                                        write(clients[i],sendbuf,sizeof(sendbuf)-1);
                                                }
                                                continue;
                                        }
                                        else if(strstr(recvbuf,"/msg")-recvbuf==0) {    //'/msg'메시지를 받았을 경우
                                                sscanf(recvbuf,"%s %s",tmp,tmp);
                                                for(j=0;j<clients_cnt;j++) {
                                                        if(strcmp(clientsID[j],tmp)==0) {
                                                                sprintf(sendbuf,"[msg from %s] %s",clientsID[i],&recvbuf[strlen(tmp)+6]);
                                                                write(clients[j],sendbuf,sizeof(sendbuf)-1);
                                                                break;
                                                        }
                                                }
                                                continue;
                                        }
                                        else {
                                                sprintf(sendbuf,"[%s] %s",clientsID[i],recvbuf);
                                                for(j=0;j<clients_cnt;j++) {
                                                        //보낸 클라이언트를 제외한 클라이언트들에세 메시지를 보냄
                                                        if(clients[j]!=clients[i]) {
                                                                write(clients[j],sendbuf,sizeof(sendbuf)-1);
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
        return 0;
}
