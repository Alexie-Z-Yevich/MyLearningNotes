#include <stdio.h>
#include <Winsock2.h>

// 客户端代码
int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);  // 服务初始化操作
    if (err != 0) {
        return 0;
    }
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        WSACleanup();  // 终止Ws2_32.dll 的使用
        return 0;
    }

    int flag = 0;
    char se[] = "Server:";

    while (1) {
        SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);  // 初始化套接字接口地址结构
        SOCKADDR_IN addrSrv;
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  // 设置通信IP地址
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(6000);  // 通信端口号
        connect(sockClient, (SOCKADDR *) &addrSrv, sizeof(SOCKADDR));  //建立连接
        if (flag == 0) {
            send(sockClient, "hello", strlen("hello") + 1, 0);
            flag++;
        } else {
            // 输入发送的消息
            char c[50];
            gets(c);
            send(sockClient, c, strlen(c) + 1, 0);
        }

        char recvBuf[50];
        recv(sockClient, recvBuf, 50, 0);
        printf("%s", se);
        printf("%s\n", recvBuf);
        if(strcmp(recvBuf, "See you next time!") == 0) {
            send(sockClient, "Bye", strlen("Bye") + 1, 0);
            break;
        }
        closesocket(sockClient);
    }
    WSACleanup();  // 终止Ws2_32.dll 的使用
}
