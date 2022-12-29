#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <Winsock2.h>

int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return 0;
    }
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        WSACleanup();
        return 0;
    }
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);
    connect(sockClient, (SOCKADDR *) &addrSrv, sizeof(SOCKADDR));
    send(sockClient, "hello", strlen("hello") + 1, 0);
    char recvBuf[50];
    recv(sockClient, recvBuf, 50, 0);
    printf("%s\n", recvBuf);
    closesocket(sockClient);
    WSACleanup();
}
