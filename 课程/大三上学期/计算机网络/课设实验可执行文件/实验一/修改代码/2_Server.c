#include <stdio.h>
#include <Winsock2.h>

// 服务器端代码
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
        WSACleanup();  // 终止Ws2_32.dll 的使用
        return 0;
    }

    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);
    bind(sockSrv, (SOCKADDR *) &addrSrv, sizeof(SOCKADDR));
    listen(sockSrv, 5);
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    int flag = 0;
    char cl[] = "Client:";
    while (1) {
        SOCKET sockConn = accept(sockSrv, (SOCKADDR *) &addrClient, &len);
        char recvBuf[50];
        recv(sockConn, recvBuf, 50, 0);
        printf("%s", cl);
        printf("%s\n", recvBuf);
        if(strcmp(recvBuf, "Bye") == 0){
            send(sockConn, "See you next time!", strlen("See you next time!") + 1, 0);
            break;
        }

        char sendBuf[50];
        if (flag == 0) {
            sprintf(sendBuf, "Welcome %s to here!", inet_ntoa(addrClient.sin_addr));
            flag++;
        } else {
            // 输入发送的消息
            char c[50];
            gets(c);
            sprintf(sendBuf, c, inet_ntoa(addrClient.sin_addr));
        }
        send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

        closesocket(sockConn);
    }
    WSACleanup();  // 终止Ws2_32.dll 的使用
}