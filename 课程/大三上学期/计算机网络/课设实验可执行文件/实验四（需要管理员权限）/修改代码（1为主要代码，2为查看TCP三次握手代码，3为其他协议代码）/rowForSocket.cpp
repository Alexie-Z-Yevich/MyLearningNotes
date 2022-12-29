/*
1,为了使网卡接收所有经过它的封包，要将其设为混杂模式；
2,创建原始套接字，便于实现；
3,将原始套接字绑定到一个明确的本地地址；
4,向套接字发送SIO_RCVALL控制命令，让它接收所有的IP包。
*/

//#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include "windows.h"
#include <ws2ipdef.h>
#include <mstcpip.h>             //SIO_RCVALL
#include <process.h>
//#pragma warning(disable:4996)
//#pragma comment(lib,"ws2_32.lib")

typedef struct _IPHeader {
    u_char VIHL;           // 版本和首部长度 各占4bit
    u_char ToS;            // 服务类型
    u_short TotalLen;      // 总长度
    u_short ID;            // 标识号
    u_short Frag_Flags;    // 片偏移量
    u_char TTL;            // 生存时间
    u_char Protocol;       // 协议
    u_short Checksum;      // 首部校验和
    struct in_addr SrcIP;  // 源IP地址
    struct in_addr DestIP; // 目的地址
} *PIPHDR;


/*TCP头*/
typedef struct _tcpheader {
    USHORT sourcePort;        // 来源端口
    USHORT destinationPort;   // 目标端口
    ULONG sequenceNumber;     // 32位序列号
    ULONG acknowledgeNumber;  // 32位确认号

    //下面2个 共占16位
    UCHAR dataOffset;         //4位首部长度/6位保留字
    UCHAR flags;              //6位标志位

    USHORT windows;           //16位窗口大小
    USHORT checksum;          //16位校验和
    USHORT urgentPointer;     //16位紧急数据偏移量

} TCP_HDR, *PTCP_HDR;

//解析IP封包
void DecodeIPPacket(char *pData) {
    PIPHDR pIphdr = (PIPHDR) pData;
    char sourceIP[32] = {0}, destIP[32] = {0};
    printf("---------------------\n");
    //从IP头中取出源IP地址和目的IP地址
    strcpy(sourceIP, inet_ntoa(pIphdr->SrcIP));
    strcpy(destIP, inet_ntoa(pIphdr->DestIP));
    printf("%s -> %s \n", sourceIP, destIP);
    //IP头长度，版本号和首部长度各占4位
    int nHeaderLen = (pIphdr->VIHL & 0xf) * sizeof(ULONG);
    printf("Protocol: ");
    PTCP_HDR pTcphdr = (PTCP_HDR) (pData + nHeaderLen);
    switch (pIphdr->Protocol) {
        case IPPROTO_ICMP:  // 1
            printf("ICMP\n");
            break;
        case IPPROTO_IGMP:  // 2
            printf("IGMP\n");
            break;
        case IPPROTO_IP:  // 0
            printf("IP\n");
            break;
        case IPPROTO_TCP :  // 6
            printf("TCP\n");
            printf("Port: %d -> %d \n", ntohs(pTcphdr->sourcePort), ntohs(pTcphdr->destinationPort));
            switch (ntohs(pTcphdr->destinationPort)) {
                case 80:
                    printf_s("Send request to 80 port. Details: %s\n", pData + sizeof(TCP_HDR));
                    break;
                case 21:
                    printf_s("Send request to 21 port. Details: %s\n", pData + sizeof(TCP_HDR));
                    break;
                case 8080:
                    printf_s("Send request to 8080 port. Details: %s\n", pData + sizeof(TCP_HDR));
                    break;
            }
            break;
        case IPPROTO_UDP :  // 17
            printf("UDP\n");
            printf("Port: %d -> %d \n", ntohs(pTcphdr->sourcePort), ntohs(pTcphdr->destinationPort));
            break;
        case IPPROTO_RAW :  // 255
            printf("RAW\n");
            break;
        default:
            printf("Unkown IPPRPTP, please check the Protocol in Winsock2\n");
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //raw socket只能由root建立
    SOCKET s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (s == INVALID_SOCKET) {
        printf("Please Use The Administrator Account To Run\n");
        return -1;
    }

    //获取本地IP地址
    char szHostName[56];
    SOCKADDR_IN sin;
    struct hostent *pHost;

    gethostname(szHostName, 56);
    if ((pHost = gethostbyname((char *) szHostName)) == NULL) {
        printf("gethostbyname Failed\n");
        return -1;
    }
    sin.sin_family = AF_INET;
    sin.sin_port = htons(0);

    for (ULONG i = 0; pHost->h_addr_list[i] + pHost->h_length < pHost->h_name;) {  //对于多IP地址匹配的问题
        printf("My IP[%d]: %s\n", ++i, inet_ntoa(*((in_addr *) pHost->h_addr_list[i])));
    }
    memcpy_s(&sin.sin_addr.S_un.S_addr, 4U, pHost->h_addr_list[1], pHost->h_length);
    printf("Start To Bind IP: %s\n", inet_ntoa(sin.sin_addr));

    if (bind(s, (sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        printf("Bind Failed\n");
        return -1;
    }
    DWORD dwValue = 1;
    if (ioctlsocket(s, SIO_RCVALL, &dwValue) != 0) {  // 设置SIO_RCVALL
        printf("SIO_RCVALL Failed\n");
        return -1;
    }
    char buff[1024];
    int nRet;
    while (1) {  // 开始接收封包
        nRet = recv(s, buff, 1024, 0);
        if (nRet > 0) {
            DecodeIPPacket(buff);  // 解析IP封包
        }
    }
    system("pause");
    closesocket(s);
    WSACleanup();
}
