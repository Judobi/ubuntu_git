#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>

#define PORT 1313

void error(char *msg){
    perror(msg);
    exit(1);
}

// #1
int main(int argc, char *argv[]){
    
    int sockfd, client_sockfd;
    socklen_t client_len;

    char buffer[256];

    struct sockaddr_in server_addr,client_addr; //adress
    int n;
    
    if(argc<2){
        fprintf(stderr, "ERROR, no port provide \n ");
        exit(1);
    }

    //#2 create new socket 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //STREAM형 TCP사용

    if(sockfd<0){
        error("ERROR, creating socket");
    }

    bzero((char*) &server_addr, sizeof(server_addr)); //서버구조체 초기화
    server_addr.sin_family = AF_INET; //domain, 주소체계
    server_addr.sin_addr.s_addr = INADDR_ANY; //내부아이피 할당
    server_addr.sin_port = htons(PORT); //포트번호 정렬\

    //#3 bind
    if(bind(sockfd,(struct sockaddr *) &server_addr, sizeof(server_addr))<0){
        error("ERROR on binding");
    }

    //#4 listen
    listen(sockfd,5); //대기, 보통 5사용.

    if(listen(sockfd,5)<0){
        error("ERROR on listening");
    }

    //#5 accept
    client_len = sizeof(client_addr); //client address 크기
    client_sockfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len); //성공시 0보다 큰 값 반환.
    if(client_sockfd <0){
        error("ERROR on accept");
    }

    //#6 read
    bzero(buffer,256);
    n = read(client_sockfd,buffer,255);
    if (n<0){
        error("ERROR reading from socket");
        printf("Here is the message :  %s\n",buffer);
    }

    //#7 write

    n= write(client_sockfd,"I got yout message",18);
    if(n<0){
        error("ERROR writing to socket");
    }

    close(sockfd);
    close(client_sockfd);
    return 0;
}