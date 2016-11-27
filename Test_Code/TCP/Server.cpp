#include <WinSock2.h>
#include <stdio.h>

void main()
{
	//请求Socket的版本信息（高位字节表示副版本，低位字节表示主版本）
	//返回Socket的版本信息，存放在wsaData里
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//生成一个版本号为1.1的双字节版本信息
	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return;
	}

	//判断高字节和低字节版本号是不是1.1？
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			//解除与Socket库的绑定，释放资源
			WSACleanup( );
			return; 
	}
	//定义Socket描述符，TCP方式

	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);
	//定义Socket数据结构，IP地址，网际域，端口号
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);
	//在Socket上绑定服务器的IP和端口
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//监听客户端的链接，最大允许5个客户端
	listen(sockSrv,5);
	int len=sizeof(SOCKADDR);							 
	SOCKADDR_IN addrClient;
	while(1)
	{
		//通过accept函数等待客户端的链接Connet，并形成新的Socket描述符，存储客户端的IP地址和端口
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		//recv接收客户端的消息，通过新建的socket描述符，存放在recvBuf里面，最大一次能接收100个字节
		char recvBuf[100];
		recv(sockConn,recvBuf,100,0);
		printf("%s\n",recvBuf);
		//sendBuf发送给客户端消息，通过新建的socket描述符，发送信息存放在sendBuf里面，最大一次能发送100个字节
		char sendBuf[100];
		sprintf(sendBuf,"轨道交通信号系统的创新研究与实践",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);

		closesocket(sockConn);
	}
}
