#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")


int main(int argc, char* argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);
    printf("dang cho ket noi...\n");

    SOCKET client = accept(listener, NULL, NULL);
    printf("ket noi dang cho: %d\n", client);

    char buf[256];
    int ret;
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        if (ret < sizeof(buf)) { buf[ret] = 0; }
        printf("%s\n", buf);
    }

    closesocket(client);
    closesocket(listener);
    WSACleanup();

}