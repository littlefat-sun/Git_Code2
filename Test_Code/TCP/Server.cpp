#include <WinSock2.h>
#include <stdio.h>

void main()
{
	//����Socket�İ汾��Ϣ����λ�ֽڱ�ʾ���汾����λ�ֽڱ�ʾ���汾��
	//����Socket�İ汾��Ϣ�������wsaData��
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//����һ���汾��Ϊ1.1��˫�ֽڰ汾��Ϣ
	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return;
	}

	//�жϸ��ֽں͵��ֽڰ汾���ǲ���1.1��
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			//�����Socket��İ󶨣��ͷ���Դ
			WSACleanup( );
			return; 
	}
	//����Socket��������TCP��ʽ

	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);
	//����Socket���ݽṹ��IP��ַ�������򣬶˿ں�
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);
	//��Socket�ϰ󶨷�������IP�Ͷ˿�
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//�����ͻ��˵����ӣ��������5���ͻ���
	listen(sockSrv,5);
	int len=sizeof(SOCKADDR);							 
	SOCKADDR_IN addrClient;
	while(1)
	{
		//ͨ��accept�����ȴ��ͻ��˵�����Connet�����γ��µ�Socket���������洢�ͻ��˵�IP��ַ�Ͷ˿�
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		//recv���տͻ��˵���Ϣ��ͨ���½���socket�������������recvBuf���棬���һ���ܽ���100���ֽ�
		char recvBuf[100];
		recv(sockConn,recvBuf,100,0);
		printf("%s\n",recvBuf);
		//sendBuf���͸��ͻ�����Ϣ��ͨ���½���socket��������������Ϣ�����sendBuf���棬���һ���ܷ���100���ֽ�
		char sendBuf[100];
		sprintf(sendBuf,"�����ͨ�ź�ϵͳ�Ĵ����о���ʵ��",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);

		closesocket(sockConn);
	}
}
