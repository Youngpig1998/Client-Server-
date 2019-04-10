#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")

int
main()
{


	//打开网络库
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		WSACleanup();
		std::cerr << "网络库打开失败！" << std::endl;
		return -1;
	}

	//校验版本
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		std::cerr << "版本号不正确！" << std::endl;
		return -1;
	}

	//创建服务器端的SOCKET
	SOCKET _socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));//初始化的是服务器的SOCKET
	if (_socket == INVALID_SOCKET)
	{
		WSACleanup();
		std::cerr << "服务器SOCKET创建失败！" << std::endl;
		return -1;
	}

	//链接服务器
	sockaddr_in serverMsg;
	serverMsg.sin_family = AF_INET;
	serverMsg.sin_port = htons(12345);
	serverMsg.sin_addr.S_un.S_addr  = inet_addr("127.0.0.1");

	if (connect(_socket, (const sockaddr*)&serverMsg, sizeof(serverMsg)) == SOCKET_ERROR)
	{
		std::cerr << "链接服务器失败，错误码为：" << WSAGetLastError() << std::endl;
		closesocket(_socket);
		WSACleanup();
		return -1;
	}
	



	while (1)
	{
		//处理数据
		int x, y;
		int z = 0;
		std::cout << "输入两个数：";
		std::cin >> x;
   		std::cin >> y;
		char xx[8] = { 0 };
		for (int i = 0; i != sizeof(int); ++i)
		{
			xx[i] = *((char*)&x + z);
			++z;
		}
		z = 0;
		for (int i = sizeof(int); i != sizeof(int) + sizeof(int); ++i)
		{
			xx[i] = *((char*)&y + z);
			++z;
		}

		//发送
		if (send(_socket, xx,sizeof(xx), 0) == SOCKET_ERROR)
		{
			//视具体错误码来处理
			std::cerr << "发送消息给服务器失败，错误码为：" << WSAGetLastError() << std::endl;
		}
		else
		{
			std::cout << "发送成功！" << std::endl;
		}
		
		//std::string str;
		//std::cin >> str;
		////发送
		//if (send(_socket,str.c_str() ,str.size()+1, 0) == SOCKET_ERROR)
		//{
		//	//视具体错误码来处理
		//	std::cerr << "发送消息给服务器失败，错误码为：" << WSAGetLastError() << std::endl;
		//}
		//else
		//{
		//	std::cout << "发送成功！" << std::endl;
		//}

		//收
		char buf[1500] = { 0 };
		int res = recv(_socket, buf, sizeof(int), 0);

		if (res == SOCKET_ERROR)
		{
			//视具体错误码来处理
			std::cerr << "收到服务器消息失败，错误码为：" << WSAGetLastError() << std::endl;
		}
		else if (res == 0)
		{
			std::cout << "连接中断，服务器下线" << std::endl;
			system("pause");
			return -1;
		}
		else
			std::cout << "计算结果为：" << *((int*)&buf) << std::endl;
	}


	

	//清理网络库
	closesocket(_socket);
	WSACleanup();

	system("pause");
	return 0;
}
