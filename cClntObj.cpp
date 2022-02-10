#include "cClntObj.h"
#include "cServerSocket.h"

cClntObj::cClntObj(LPVOID _pServerSocket, SOCKET* _pSocket)
{
	cout << "[ Client Accepted ]" << endl;
	p_ServerSock = _pServerSocket;
	p_Clnt = nullptr;
	p_Clnt = _pSocket;
	b_ClntObjFlag = true;
	b_MessageFlag = false;

}

cClntObj::~cClntObj()
{
	cout << "** Delete Clnt Object" << endl;
}

//##################################################################
// ���� :	cClntObj ���� ( Receive, Send ������ ���� ) 
//##################################################################
void cClntObj::__ClntInit__()
{
	thread RecvThread = thread(_RecvDataFrom, this);
	thread SendThread = thread(_SendDataTo, this);

	RecvThread.join();
	SendThread.join();
	while (b_ClntObjFlag) { Sleep(1); }	
	_BreakConnection();
}

//##################################################################
//	���� :	p_Clnt�κ��� ���� �����͸� cBuffer�� ���� 
//##################################################################
void cClntObj::_RecvDataFrom(LPVOID lp)
{
	cClntObj* p = (cClntObj*)lp;
	while (p->b_ClntObjFlag)
	{
		try {
			recv(*p->p_Clnt, p->cBuffer, 1024, 0);
			if (strlen(p->cBuffer) > 0)
			{
				cout << "\n�� Message From Client : " << p->cBuffer << endl;
				if (strncmp(p->cBuffer, "exit", strlen(p->cBuffer)) == 0)
				{
					cout << "[ Break Connection ]" << endl;
					p->b_ClntObjFlag = false;
				}
				ZeroMemory(p->cBuffer, strlen(p->cBuffer));
			}
		}
		catch (exception e)
		{
			cout << WSAGetLastError() << endl;
		}
		Sleep(1);
	}
}

//##################################################################
//	���� :	�Է� ���� �����͸� p_Clnt�� ���� 
//##################################################################
void cClntObj::_SendDataTo(LPVOID lp)
{
	cClntObj* p = (cClntObj*)lp;
	while (p->b_ClntObjFlag)
	{
		cout << "\n�� Text your message : ";
		cin >> p->cBuffer;
	
		if (strncmp(p->cBuffer, "exit", strlen(p->cBuffer)) == 0)
		{
			cout << "[ Break Connection ]" << endl;
			p->b_ClntObjFlag = false;
		}
		else
		{
			send(*p->p_Clnt, p->cBuffer, strlen(p->cBuffer), 0);
		}
		ZeroMemory(p->cBuffer, strlen(p->cBuffer));
		Sleep(1);
	}
}

//##################################################################
//	���� :	Receive, Send���� exit ȣ�� ��, p_Clnt���� (cServerSocket)
//##################################################################
void cClntObj::_BreakConnection()
{
	cServerSocket* p = (cServerSocket*)p_ServerSock;
	p->_EraseClntSocket(p_Clnt);
}
