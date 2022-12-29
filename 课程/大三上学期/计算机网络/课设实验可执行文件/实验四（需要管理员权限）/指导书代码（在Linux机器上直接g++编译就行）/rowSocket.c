#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/in.h>

#define BUFFER_MAX 2048

int main(int argc, char *argv[]) {
    int sock, n_read, proto;
    char buffer[BUFFER_MAX];  // 设置最大缓冲区
    char *ethhead, *iphead, *tcphead,
            *udphead, *icmphead, *p;
    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0) {  // 初始化操作
        fprintf(stdout, "create socket error\n");
        exit(0);
    }
    while (1) {
        n_read = recvfrom(sock, buffer, 2048, 0, NULL, NULL);
        /*
        14 6(dest)+6(source)+2(type or length)  // dest是目标地址；source是起始位置
        +
        20 ip header
        +
        8 icmp,tcp or udp header
        = 42
        */
        if (n_read < 42) {
            fprintf(stdout, "Incomplete header, packet corrupt\n");  // 头信息不完整，报文出错
            continue;
        }
        ethhead = buffer;
        p = ethhead;
        int n = 0XFF;
        printf("MAC: %.2X:%02X:%02X:%02X:%02X:%02X==>"
               "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
               p[6] & n, p[7] & n, p[8] & n, p[9] & n, p[10] & n, p[11] & n,
               p[0] & n, p[1] & n, p[2] & n, p[3] & n, p[4] & n, p[5] & n);
        iphead = ethhead + 14;  // 这里好像是向后推进
        p = iphead + 12;  // 因真正的IP只是IP header的后8位？

        printf("IP: %d.%d.%d.%d => %d.%d.%d.%d\n",
               p[0] & 0XFF, p[1] & 0XFF, p[2] & 0XFF, p[3] & 0XFF,
               p[4] & 0XFF, p[5] & 0XFF, p[6] & 0XFF, p[7] & 0XFF);  //  & 0XFF的作用是位与
        proto = (iphead + 9)[0];  // 通过IP头的第一位确定协议
        p = iphead + 20;  // 到这里就只剩下icmp,tcp or udp header部分了
        printf("Protocol: ");
        switch (proto) {
            case IPPROTO_ICMP:  // 1
                printf("ICMP\n");
                break;
            case IPPROTO_IGMP:  // 2
                printf("IGMP\n");
                break;
            case IPPROTO_IPIP:  // IP为0，IPIP是隧道协议。在ip协议报文的基础上继续封装ip报文，基于tun设备实现，是一种点对点的通讯技术
                printf("IPIP\n");
                break;
            case IPPROTO_TCP :  // 6
            case IPPROTO_UDP :  // 17
                printf("%s,", proto == IPPROTO_TCP ? "TCP" : "UDP");
                printf("source port: %u,", (p[0] << 8) & 0XFF00 | p[1] & 0XFF);
                printf("dest port: %u\n", (p[2] << 8) & 0XFF00 | p[3] & 0XFF);
                break;
            case IPPROTO_RAW :  // 255
                printf("RAW\n");
                break;
            default:
                printf("Unkown, please query in include/linux/in.h\n");
        }
    }
}
