#include <iostream>
#include <WinSock2.h>
#include <ctime>

#pragma comment(lib,"ws2_32.lib")
//#pragma warning(suppress : 4996) //doesn't work.
#pragma warning(disable : 4996)
using namespace std;

void printCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    cout << "现在时间是："
         << 1900 + ltm->tm_year << "年"
         << 1 + ltm->tm_mon << "月"
         << ltm->tm_mday << "日 "
         << ltm->tm_hour << "时"
         << ltm->tm_min << "分"
         << ltm->tm_sec << "秒" << endl;
}

int main(int argv, char* argc[])
{
    //初始化
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发送消息
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));         //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(8888);
    connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));

    //接收服务器消息
    char szBuffer[MAXBYTE] = { 0 };
    recv(clntSock, szBuffer, MAXBYTE, NULL);

    //输出接收到的数据
    cout << "服务端：" << szBuffer << endl;
    printCurrentTime(); // 输出当前时间

    //向服务端发送消息
    const char* str = "hello server\r\n";
    send(clntSock, str, strlen(str) + 2 * sizeof(char), NULL);

    //关闭套接字
    closesocket(clntSock);

    //终止dll
    WSACleanup();

    system("pause");

    return 0;
}
