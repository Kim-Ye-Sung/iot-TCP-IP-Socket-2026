#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len;
    char buf[BUF_SIZE];

    char webpage[] =
        "HTTP/1.1 200 OK\r\n"
        "Server: Linux Web Server\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\r\n"
        "<html><head><title> My Web Page </title>\r\n"
        "<style>body {background-color: #FFFF00} </style></head>\r\n"
        "<body><center><h1>Hello world!!</h1><br>\r\n"
        "<img src=\"Pigs.jpg\"></center></body></html>\r\n";

    if(argc != 2)   // 프로그램 실행시 포트번호를 입력하지 않으면 
    {
      printf("Usage : %s <port>\n", argv[0]);   // 포트번호도 입력하라고 출력
      exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);    // 소켓 생성

    if(serv_sock == -1)     // 소켓 생성 실패하면 
    {
      error_handling("socket() error");  // 에러코드 출력
    }
    
    int option = 1;     
    // 프로그램을 종료했는데도 전에 사용하던 포트가 운영체제에 남아있을때를 대비해서 다시 그 포트를 사용할수 있게끔 설정
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); 

    memset(&serv_adr, 0, sizeof(serv_adr));     // 서버 주소 구조체 설정
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)   // 앞서 만들어두었던 서버 소켓에 IP와 포트번호를 부여
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)      // 서버 소켓을 클라이언트가 접속할수있도록 대기상태로 설정
        error_handling("listen() error");

    while(1) 
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);   // 접속한 클라이언트를 소켓에 저장

        if(clnt_sock == -1)  // 클라이언트 접속을 받아들이는데 실패하면 반복문 초기화
            continue;

        memset(buf, 0, sizeof(buf));    // 이전에 저장되었던 클라이언트 요청 지우기
        str_len = read(clnt_sock, buf, BUF_SIZE - 1);   // 클라이언트가 보낸 요청 읽기

        if(str_len <= 0)  // 클라이언트가 보낸 요청이 없거나, 읽기에 실패했거나, 클라이언트의 접속이 끊겼다면 처리를 끝내가 다음 클라이언트 기다리기
        {
            close(clnt_sock);   
            continue;
        }

        if(strncmp(buf, "GET /Pigs.jpg", 13) == 0)  // 브라우저가 보낸 요청(buf) 안에 사진을 요청한 문자열이 있으면 
        {
            FILE *fp;

            char img_header[] =     // 응답헤더 선언
                "HTTP/1.1 200 OK\r\n"
                "Server: Linux Web Server\r\n"
                "Content-Type: image/jpeg\r\n"
                "\r\n";

            fp = fopen("Pigs.jpg", "rb");   // 사진을 읽기모드로 열기

            if(fp == NULL)  // 사진이 없으면 없다고 클라이언트한테 응답
            {
                char not_found[] =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "\r\n"
                    "<html><body><h1>404 Not Found</h1></body></html>";

                write(clnt_sock, not_found, strlen(not_found));
            }
            else         // 사진이 있으면 
            {
                write(clnt_sock, img_header, strlen(img_header));   // 사진이 있다고 응답하기

                while(1)
                {
                    str_len = fread(buf, 1, BUF_SIZE, fp); // 사진에서 최대 BUF_SIZE(1024바이트) 크기만큼 나눠서 읽기

                    if(str_len <= 0)  // 사진의 끝까지 반복한다.
                        break;

                    write(clnt_sock, buf, str_len);   // 나눠서 읽은 크기만큼 클라이언트한테 보내기
                }

                fclose(fp);
            }
        }
        else    // 요청한게 사진이 아니라면 만들어두었던 webpage HTML 전송
        {
            write(clnt_sock, webpage, strlen(webpage));
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

