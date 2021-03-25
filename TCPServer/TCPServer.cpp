#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

FILE* f1, * f2;

int main(int argc, char** argv) {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	int port = atoi(argv[1]);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);
    printf("dang cho ket noi...\n");

	SOCKET client = accept(listener, NULL, NULL);
	printf("chap nhan ket noi \n");

	char buf[2048];
	char buf1[2048];

	FILE* f2 = fopen(argv[2], "r");
	FILE* f1 = fopen(argv[3], "wb");


	if (f2 == NULL) {
		printf("Error!");
		return 1;
	}
	fscanf(f2, "%[^\n]", buf1);
	send(client, buf1, strlen(buf1), 0);
	int ret;
	while (1) {
		ret = recv(client, buf, sizeof(buf), 0);

		if (ret <= 0) break;

		fwrite(buf, 1, ret, f1);
		if (ret < sizeof(buf))
			buf[ret] = 0;
		printf("Received: %s\n", buf);
	}
	fclose(f1);
	fclose(f2);
	system("pause");
	closesocket(client);
	closesocket(listener);
	WSACleanup();
}