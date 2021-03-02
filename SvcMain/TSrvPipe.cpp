// TSrvPipe.cpp : définit la communication entre l'application et le service.
//

#include <windows.h>
#include "TSrvPipe.h"
#include "IDruideConfig.h"


TSrvPipe::TSrvPipe(void)
{
}

TSrvPipe::TSrvPipe(LPCTSTR szPipeName)
{
	CreateSrvNamedPipe(szPipeName);
}

bool TSrvPipe::CreateSrvNamedPipe(LPCTSTR szPipeName)
{
	int LastError;
	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;


	memset(&sd, 0, sizeof(SECURITY_DESCRIPTOR));
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, true, NULL, false);
	sa.nLength = sizeof(SECURITY_DESCRIPTOR);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

  hPipe = CreateNamedPipe(
			szPipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
			1, 1024, 1024, 0, &sa);

	LastError = GetLastError();

	ConnectNamedPipe(hPipe, NULL);

	return true;
}

bool TSrvPipe::ProcessSrvNamedPipe()
{
	DWORD dw;
	char szBuffer[1024];


	memset(szBuffer, 0, sizeof(szBuffer));
	ReadFile(hPipe, szBuffer, 1023, &dw, NULL);
	if (dw != 0)
	{
		ProcessCommand(szBuffer);
	}

	return true;
}

TSrvPipe::~TSrvPipe(void)
{
	DisconnectNamedPipe(hPipe);

	CloseHandle(hPipe);
}

TSrvPipeIDruide::TSrvPipeIDruide(LPCTSTR szPipeName): TSrvPipe(szPipeName)
{
}

bool TSrvPipeIDruide::ProcessCommand(LPCSTR szCommand)
{
	char szReturn[1024];
  char* pMac = getMAC();


	if (!lstrcmpA(szCommand, "INFO")) {
		strcpy_s(szReturn, sizeof(szReturn) - strlen(szReturn), "OS : ");
		strcat_s(szReturn, sizeof(szReturn) - strlen(szReturn), getOS());
		strcat_s(szReturn, sizeof(szReturn) - strlen(szReturn), "\n");

		strcat_s(szReturn, sizeof(szReturn) - strlen(szReturn), "MAC Adress : ");
		pMac = getMAC();
		strcat_s(szReturn, sizeof(szReturn) - strlen(szReturn), pMac);
		free(pMac);
		strcat_s(szReturn, sizeof(szReturn) - strlen(szReturn), "\n");

		WriteFile(hPipe, szReturn, strlen(szReturn), NULL, NULL);
		FlushFileBuffers(hPipe);
		return true;
	}
	else if (!lstrcmpA(szCommand, "RESTART")) {
		WinExec("shutdown /r /t 60", SW_HIDE);
	}
	else if (!lstrcmpA(szCommand, "END")) {
		DisconnectNamedPipe(hPipe);
		ConnectNamedPipe(hPipe, NULL);
		return true;
	}
	else if (!memcmp(szCommand, "PARAM", 5)) {
		// szCommand = "PARAM C LL A"
		// C = Camera (0 = Not allowed, 1 = Allowed)
		// LL = Length password
		// A = Allow letters (1 = Allow leters, 2 = Digits only)
		ConfigureCamera(szCommand[6] == '1');
	}

	return false;
}

