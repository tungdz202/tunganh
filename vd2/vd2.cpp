#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

bool check_ipaddress(char* str) { // Hàm check chuỗi có phải địa chỉ IP không
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


int main(int argc, char** argv)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    char tmp[48];
    strcpy(tmp, argv[1]);
    if (check_ipaddress(tmp)) addr.sin_addr.s_addr = inet_addr(argv[1]);
    else {
        addrinfo* info;
        int result = getaddrinfo(argv[1], "http", NULL, &info);
        if (result != 0)
        {
            printf("Khong phan giai duoc ten mien");
            return 1;
        }
        memcpy(&addr, info->ai_addr, info->ai_addrlen);
        printf("Dia chi IP: %s", inet_ntoa(addr.sin_addr));
    }
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));

    if (ret == SOCKET_ERROR)

    {
        ret = WSAGetLastError();

        printf("Connect failed: %d\n", ret);

        return 1;
    }

    char buf[256];
    char hostbuf[256];
    int hostname;

    hostname = gethostname(hostbuf, sizeof(hostbuf));

    DWORD cchBuf;
    WCHAR* driveStrings;
    ULARGE_INTEGER freeSpace;

    //đọc thông tin ổ đĩa

    cchBuf = GetLogicalDriveStrings(0, NULL);

    driveStrings = (WCHAR*)malloc((cchBuf + 1) * sizeof(TCHAR));
    if (driveStrings == NULL)
    {
        return -1;
    }

    GetLogicalDriveStrings(cchBuf, driveStrings);
    char comp_name[256] = "Computer name: ";
    strcat(comp_name, hostbuf);
    send(client, comp_name, sizeof(comp_name), 0);

    while (*driveStrings)
    {
        char temp[256];
        char a[] = ": - ";
        char b[] = " GB free of ";
        GetDiskFreeSpaceEx(driveStrings, &freeSpace, NULL, NULL);
        int space = freeSpace.QuadPart / 1024 / 1024 / 1024;
        sprintf(temp, "%d", space);
        strcpy(buf, (char*)driveStrings);
        strcat(buf, a);
        strcat(buf, temp);
        strcat(buf, b);
        send(client, (char*)buf, sizeof(buf), 0);
        driveStrings += lstrlen(driveStrings) + 1;
    }

    closesocket(client);

    WSACleanup();

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
