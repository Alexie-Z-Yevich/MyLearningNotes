#include "iostream"
#include "winsock2.h"
#include "windows.h"
#include "string"
#include "unistd.h"
#include "sstream"

using namespace std;

string username, password, message, info, subject;

string to_addr;

int err;

int main()
{
    char buff[1024];
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 1);

    err = WSAStartup(wVersionRequested, &wsaData);

    SOCKADDR_IN addrServer;
    HOSTENT *pHostent;
    SOCKET sockClient;
    cout << "��ӭ�����ʼ�ϵͳ������������ѡ��ķ��񣺣�1�������ʼ�����    ��2���鿴�ʼ�����\n";
    int num;
    cin >> num;
    if(num == 1)
    {
        sockClient = socket(AF_INET, SOCK_STREAM, 0);

        pHostent = gethostbyname("smtp.qq.com");

        addrServer.sin_addr.S_un.S_addr = *((DWORD *) pHostent->h_addr_list[0]);
        addrServer.sin_family = AF_INET;
        addrServer.sin_port = htons(25);

        err = connect(sockClient, (SOCKADDR *) &addrServer, sizeof(SOCKADDR));
        buff[recv(sockClient, buff, 500, 0)] = '\0';
        message = "ehlo qq.com\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "auth login\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        username = "________________________\r\n"; //base64���ܵ��û���(xxxx@qq.com)
        send(sockClient, username.c_str(), username.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        password = "________________________\r\n"; //base64���ܵ�����
        send(sockClient, password.c_str(), password.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        cout << "������Ŀ�����䣺";
        cin >> to_addr;

        message = "MAIL FROM:________________________ \r\nRCPT TO:<";
        message.append(to_addr);
        message.append("> \r\n");
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "DATA\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "From: ________________________\r\nTo: " + to_addr + "\r\nsubject:";
        cout << "�������ʼ����⣺";
        cin >> subject;
        message.append(subject);
        message.append("\r\n\r\n");
        cout << "�������ʼ����ģ�";
        cin >> info;
        message.append(info);
        message.append("\r\n.\r\n");
        send(sockClient, message.c_str(), message.length(), 0);//�����ʼ�����
        message = "QUIT\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';
        cout << "���ͳɹ���" << endl;
    }
    else
    {
        sockClient = socket(AF_INET, SOCK_STREAM, 0);
        pHostent = gethostbyname("pop.qq.com");
        int port = 110;
        addrServer.sin_addr.S_un.S_addr = *((DWORD *) pHostent->h_addr_list[0]);
        addrServer.sin_family = AF_INET;
        addrServer.sin_port = htons(port);
        err = connect(sockClient, (SOCKADDR *) &addrServer, sizeof(SOCKADDR));
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "user ________________________\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "pass ________________________\r\n"; //��Ȩ�룬û���ܵ�
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "stat\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 500, 0)] = '\0';

        message = "list\r\n";
        send(sockClient, message.c_str(), message.length(), 0);
        buff[recv(sockClient, buff, 50000, 0)] = '\0';
        std::cout << "Client : send list \nServer :"
                  << buff << std::endl;
        while(true)
        {
            int n;
            std::cout << "�鿴���ʼ���ţ�" << std::endl;
            cin >> n;
            message = "retr " + to_string(n) + "\r\n";

            send(sockClient, message.c_str(), message.length(), 0); //����״̬
            std::cout << "Client : send retr (...) \n";

            buff[recv(sockClient, buff, 50000, 0)] = '\0';   //���շ���ֵ
            std::cout << "Server ��" << buff << std::endl;
        }

    }

}
