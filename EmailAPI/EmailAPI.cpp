#include "head.h"


SOCKET Connect_Server(char *Smtp_Address,int Port)
{
	WSADATA wsadata;
	int ret = 0;
	SOCKET Email_Connect;
	SOCKADDR_IN Saddr;
	struct hostent *pHostent;
	ret = WSAStartup(MAKEWORD(2, 2),&wsadata);
	if (ret != 0)
	{
		return 0;
	}

	if (HIBYTE(wsadata.wVersion) != 2 || LOBYTE(wsadata.wVersion) != 2)
	{
		printf("Socket Version Error Failed to initialize\n");
		WSACleanup();
		return 1;
	}

	Email_Connect = socket(AF_INET, SOCK_STREAM, 0);
	pHostent = gethostbyname(Smtp_Address);

	Saddr.sin_addr.S_un.S_addr = *((unsigned long*)pHostent->h_addr_list[0]);
	Saddr.sin_family = AF_INET;
	Saddr.sin_port = htons(Port);

	connect(Email_Connect, (SOCKADDR*)&Saddr, sizeof(SOCKADDR));
	return Email_Connect;
}

void Uninstalls(SOCKET Email_Connect)
{
	closesocket(Email_Connect);
	WSACleanup();
}

void Send_Message(SOCKET Eamil_Connect,char *Message)
{
	printf("\nsend:%s\n", Message);
	send(Eamil_Connect, Message, strlen(Message), 0);
}

void Receive_Message(SOCKET Email_Connect)
{
	char TEXT[BUFSIZ];
	strnset(TEXT, '\0', BUFSIZ);
	recv(Email_Connect, TEXT, BUFSIZ, 0);
	printf("\nrecv:%s\n", TEXT);
}

int Base64_Encode(const char *Sourcedata, char *Base64)
{
	int i = 0, j = 0;
	unsigned char trans_index = 0;    
	const int datalength = strlen((const char*)Sourcedata);
	for (; i < datalength; i += 3) {
		trans_index = ((Sourcedata[i] >> 2) & 0x3f);
		Base64[j++] = Base64_Coding[(int)trans_index];
		trans_index = ((Sourcedata[i] << 4) & 0x30);
		if (i + 1 < datalength) {
			trans_index |= ((Sourcedata[i + 1] >> 4) & 0x0f);
			Base64[j++] = Base64_Coding[(int)trans_index];
		}
		else {
			Base64[j++] = Base64_Coding[(int)trans_index];

			Base64[j++] = Base64_Coding[64];

			Base64[j++] = Base64_Coding[64];

			break;   
		}

		trans_index = ((Sourcedata[i + 1] << 2) & 0x3c);
		if (i + 2 < datalength) { 
			trans_index |= ((Sourcedata[i + 2] >> 6) & 0x03);
			Base64[j++] = Base64_Coding[(int)trans_index];

			trans_index = Sourcedata[i + 2] & 0x3f;
			Base64[j++] = Base64_Coding[(int)trans_index];
		}
		else {
			Base64[j++] = Base64_Coding[(int)trans_index];
			Base64[j++] = Base64_Coding[64];
			break;
		}
	}

	Base64[j] = '\0';

	return 0;
}

void Sun(const char Send_Email[BUFSIZ], const char Send_Passworld[BUFSIZ], char Receive_Email[BUFSIZ], char Title[BUFSIZ], char Content[BUFSIZ])
{
	char SendBuf[BUFSIZ], RecvBuf[BUFSIZ],Base64[128];
	char *Email_Address = "smtp.qq.com";
	int Port = 25;
	SOCKET Email_Connect;
	Email_Connect = Connect_Server(Email_Address, Port);

	//接收SMTP服务的消息查看是否连接成功
	Receive_Message(Email_Connect);
	//向服务器打招呼
	Send_Message(Email_Connect, "helo qq\r\n");
	//接收服务器发来的数据
	Receive_Message(Email_Connect);
	//告诉服务器需要登陆
	Send_Message(Email_Connect, "auth login\r\n");
	//接收服务器发来的数据
	Receive_Message(Email_Connect);
	
	//接下来就是登录邮箱
	const char *Email_Account = Send_Email;
	const char *Email_Password = Send_Passworld;
	//然后发送您的账号和密码通过Base64重组编码发送给SMTP服务器

	Base64_Encode(Email_Account, Base64);
	strcpy(SendBuf, Base64);
	strcat(SendBuf, "\r\n\0");
	Send_Message(Email_Connect, SendBuf);
	Receive_Message(Email_Connect);

	Base64_Encode(Email_Password, Base64);
	strcpy(SendBuf, Base64);
	strcat(SendBuf, "\r\n\0");
	Send_Message(Email_Connect, SendBuf);
	Receive_Message(Email_Connect);

	//告诉服务器发送人是谁
	strcpy(SendBuf, "mail from:<");

	strcat(SendBuf, Send_Email);
	strcat(SendBuf, ">");
	strcat(SendBuf, "\r\n\0");
	Send_Message(Email_Connect, SendBuf);
	Receive_Message(Email_Connect);
	//现在要告诉服务器收件人是谁
	strcpy(SendBuf, "rcpt to:<");
	strcat(SendBuf, Receive_Email);
	strcat(SendBuf, ">");
	strcat(SendBuf, "\r\n\0");
	Send_Message(Email_Connect, SendBuf);
	Receive_Message(Email_Connect);
	//发送邮件内容
	Send_Message(Email_Connect, "data\r\n");
	Receive_Message(Email_Connect);
	strcpy(SendBuf, "From:");
	strcat(SendBuf, Send_Email);
	strcat(SendBuf, "\n");
	strcat(SendBuf, "To:");
	strcat(SendBuf, Receive_Email);
	strcat(SendBuf, "\n");
	//发送标题和内容给服务器
	strcat(SendBuf, "subject:");
	strcat(SendBuf, Title);
	strcat(SendBuf, "\r\n\r\n");
	strcat(SendBuf, Content);
	strcat(SendBuf, "\r\n.\r\n\0");
	Send_Message(Email_Connect, SendBuf);
	Receive_Message(Email_Connect);

	Uninstalls(Email_Connect);
	printf("mail is already send to:%s\n", Receive_Email);
	system("pause");
}
