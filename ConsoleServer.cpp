#include <iostream>
#include <WinSock2.h>
#include <ctime>

#pragma comment(lib,"ws2_32.lib")        //加载ws2_32.dll
using namespace std;
//#pragma warning(suppress : 4996) //doesn't work.
#pragma warning(disable : 4996)

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
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));     //每个字节用0填充
    sockAddr.sin_family = PF_INET;              //使用ipv4
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(8888);            //端口
    bind(servSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);

    //接收客户端消息
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)& clntAddr, &nSize);

    //向客户端发送消息
    const char* str = "hello client\r\n";
    send(clntSock, str, strlen(str) + 2 * sizeof(char), NULL);
    //printCurrentTime(); // 输出当前时间

    char szBuffer[MAXBYTE] = { 0 };
    recv(clntSock, szBuffer, MAXBYTE, NULL);
    cout << "客户端：" << szBuffer << endl;
    printCurrentTime(); // 输出当前时间

    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);

    //终止dll使用
    WSACleanup();

    system("pause");
    return 0;
}
