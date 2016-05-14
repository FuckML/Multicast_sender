#pragma comment (lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h> // for IP_MULTICAST_TTL option
#define TTL 64

using namespace std;

void err_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
void main() {
  WSADATA wsaData;
  SOCKET sendSock;
  SOCKADDR_IN mulAddr;
  int timelive = TTL;
  FILE *fp;
  char buf[BUFSIZ];

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    err_handling("wsastartup err");
  }

  sendSock = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&mulAddr, 0, sizeof(mulAddr));
  mulAddr.sin_family = AF_INET;
  mulAddr.sin_addr.S_un.S_addr = inet_addr("224.1.1.1");
  mulAddr.sin_port = htons(9000);

  setsockopt(sendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)timelive, sizeof(timelive)); // 옵션으로 소켓의 TTL정보 저장(반드시 해야함)
  if ((fp = fopen("news.txt", "r")) == NULL) {
    err_handling("fopen err");
  }
  while (!feof(fp)) {
    fgets(buf, BUFSIZ, fp);
    fputs(buf, stdout);
    sendto(sendSock, buf, BUFSIZ, 0, (SOCKADDR*)&mulAddr, sizeof(mulAddr)); // 설정한 ADDR로 UDP기반 데이터 보냄
    Sleep(500);
  }
  closesocket(sendSock);
  WSACleanup();
}
