//receiver.cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#define MCASTADDR "233.0.0.2" //本例使用的多播组地址。
#define MCASTPORT 5150 //绑定的本地端口号。
#define BUFSIZE 1024 //接收数据缓冲大小。
#pragma comment(lib, "ws2_32")

int main(int argc, char **argv) {
    WSADATA wsd;
    struct sockaddr_in local, remote, from;
    SOCKET sock, sockM;
    TCHAR recvbuf[BUFSIZE];
/*struct ip_mreq mcast; // Winsock1.0 */
    int len = sizeof(struct sockaddr_in);
    int ret;
//初始化 WinSock2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        printf("WSAStartup() failed\n");
        return -1;
    }
/*
创建一个 SOCK_DGRAM 类型的 SOCKET
其中,WSA_FLAG_MULTIPOINT_C_LEAF 表示 IP 多播在控制面层上属于
"无根"类型;
WSA_FLAG_MULTIPOINT_D_LEAF 表示 IP 多播在数据面层上属于"无根",
有关控制面层和
数据面层有关概念请参阅 MSDN 说明。
*/
    if ((sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0,
                          WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF |
                          WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        printf("socket failed with:%d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
//将 sock 绑定到本机某端口上。
    local.sin_family = AF_INET;
    local.sin_port = htons(MCASTPORT);
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *) &local, sizeof(local)) == SOCKET_ERROR) {
        printf("bind failed with:%d \n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }
//加入多播组
    remote.sin_family = AF_INET;
    remote.sin_port = htons(MCASTPORT);
    remote.sin_addr.s_addr = inet_addr(MCASTADDR);
/* Winsock1.0 */
/*
mcast.imr_multiaddr.s_addr = inet_addr(MCASTADDR);
mcast.imr_interface.s_addr = INADDR_ANY;
if( setsockopt(sockM,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&mcast,
 sizeof(mcast)) == SOCKET_ERROR)
{
 printf("setsockopt(IP_ADD_MEMBERSHIP) failed:%d\n",WSAGetLastError());
 closesocket(sockM);
 WSACleanup();
 return -1;
 }
*/
/* Winsock2.0*/
    if ((sockM = WSAJoinLeaf(sock, (SOCKADDR *) &remote, sizeof(remote),
                             NULL, NULL, NULL, NULL,
                             JL_BOTH)) == INVALID_SOCKET) {
        printf("WSAJoinLeaf() failed:%d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    printf("Wait For The Class Start\n");
    int flag = 0;

//接收多播数据，当接收到的数据为"QUIT"时退出。
    while (1) {
        if ((ret = recvfrom(sock, recvbuf, BUFSIZE, 0,
                            (struct sockaddr *) &from, &len)) == SOCKET_ERROR) {
            printf("recvfrom failed with:%d\n", WSAGetLastError());
            closesocket(sockM);
            closesocket(sock);
            WSACleanup();
            return -1;
        }
        recvbuf[ret] = '\0';
        if(flag == 0){
            if (strcmp(recvbuf, "START") == 0){
                flag++;
                recvbuf[ret] = '\0';
                printf("Start Class!\n");
                continue;
            }else {
                continue;
            }
        }
        if (strcmp(recvbuf, "QUIT") == 0) break;
        else {
            printf("The Teacher Says :' %s ' FROM <%s> \n", recvbuf, inet_ntoa(from.sin_addr));
        }
    }
    printf("Class Finished! FROM <%s> \n", inet_ntoa(from.sin_addr));
    closesocket(sockM);
    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}
