
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

//// Hàm check chuỗi có phải địa chỉ IP không
bool check_ipaddress(char* str) {
	int test = 0;
	char* temp;
	if (str == NULL) return false;
	temp = strtok(str, ".");
	if (temp == NULL) return false;
	while (temp) {
		for (int i = 0; i < strlen(temp); i++) {
			if (!isdigit(temp[i])) return false;
		}
		int num = atoi(temp);
		if (num < 0 || num>255) return false;
		temp = strtok(NULL, ".");
		test++;
	}
	if (test != 4) return false;
	return true;
}

int main(int argc, char** argv) {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
    addr.sin_family = AF_INET;

	char tmp[40];
	strcpy(tmp, argv[1]);
	if (check_ipaddress(tmp)) {
		addr.sin_addr.s_addr = inet_addr(argv[1]);
	}

	else {
		addrinfo* info;
		int result = getaddrinfo(argv[1], "http", NULL, &info);
		if (result != 0)
		{
			printf("khong phan giai duoc ten mien!");
			return 1;
		}
		memcpy(&addr, info->ai_addr, info->ai_addrlen);
	}

	addr.sin_port = htons(atoi(argv[2]));
	system("pause");

	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR) {
		ret = WSAGetLastError();
		printf("Connect failed: %d\n", ret);
		return 1;
	}

	char buf[256];

	ret = recv(client, buf, strlen(buf), 0);
	buf[ret] = 0;
	printf("%s\n", buf);
	while (1) {
		printf("Enter string: \n");
		gets_s(buf, sizeof(buf));
		send(client, buf, strlen(buf), 0);

		if (strcmp(buf, "exit") == 0) {
			break;
		}
	}

	closesocket(client);
	WSACleanup();
}