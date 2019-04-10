#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <algorithm>

#pragma comment(lib,"ws2_32.lib")

int
main()
{

	WSAData wsaData;

	//打开网络库
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		WSACleanup();
		std::cerr << "网络库打开失败" << std::endl;
		return -1;
	}

	//校验版本
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		std::cerr << "版本号不对！失败。" << std::endl;
		return -1;
	}


	//创建SOCKET
	SOCKET _socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	if (_socket == INVALID_SOCKET)
	{
		std::cerr << "创建服务器SOCKET失败，错误码为：" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	//绑定地址与端口
	sockaddr_in sockad;

	sockad.sin_family =  AF_INET;
	sockad.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockad.sin_port = htons(12345);

	if (bind(_socket, (const sockaddr*)&sockad, sizeof(sockad)) == SOCKET_ERROR)
	{
		std::cerr << "绑定失败，错误码为：" << WSAGetLastError() << std::endl;
		closesocket(_socket);
		WSACleanup();
		return -1;
	}

	//开始监听
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "监听失败，错误码为：" << WSAGetLastError() << std::endl;
		closesocket(_socket);
		WSACleanup();
		return -1;
	}
	
	//创建客户端socket，这里的例子时只有一个服务器和一个客户端
	sockaddr_in clientMsg;
	int len = sizeof(clientMsg);
	SOCKET socketClient = accept(_socket, (sockaddr*)&clientMsg, &len);
	if (socketClient == INVALID_SOCKET)
	{
		std::cerr << "创建客户端SOCKET失败，错误码为：" << WSAGetLastError() << std::endl;
		closesocket(_socket);
		WSACleanup();
		return -1;
	}
	
	


	while (1)
	{

		int sum = 0;


		//与客户端收发消息
		//收
		char buf[1500] = { 0 };
		int res = 0;

		for (int i = 0; i != 2; ++i)
		{
			res = recv(socketClient, buf, sizeof(int), 0);
			if (res == SOCKET_ERROR)
			{
				//视具体错误码来处理
				std::cerr << "收到客户端消息失败，错误码为：" << WSAGetLastError() << std::endl;
			}
			else if (res == 0)
			{
				std::cout << "连接中断，服务器下线" << std::endl;
				system("pause");
				return -1;
			}		
			else
			{
				int x = *((int*)&buf);
				std::cout << x << std::endl;
				sum += x;
			}
		}

		char ss[4] = { 0 };
		for (int i = 0; i != sizeof(int); ++i)
			ss[i] = *((char*)&sum + i);

		//发送
		if (send(socketClient, ss,sizeof(ss), 0) == SOCKET_ERROR)
		{
			//视具体错误码来处理
			std::cerr << "发送消息给客户端失败，错误码为：" << WSAGetLastError() << std::endl;
		}
		else
			std::cout << "发送成功！" << std::endl;
	}




	//清理网络库
	closesocket(_socket);
	closesocket(socketClient);
	WSACleanup();

	system("pause");
	return 0;
}
