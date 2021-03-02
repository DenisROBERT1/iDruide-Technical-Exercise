// TSrvPipe.cpp : définit la communication entre l'application et le service.
//

#include <windows.h>
#include "TSrvPipe.h"


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

  hPipe = CreateNamedPipe(
			szPipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
			1, 1024, 1024, 0, NULL);

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
	if (!lstrcmpA(szCommand, "INFO")) {
		WriteFile(hPipe, "Bla bla bla\nBla.", 16, NULL, NULL);
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
		MessageBoxA(NULL, szCommand, "$$$", MB_OK);
	}

	return false;
}
