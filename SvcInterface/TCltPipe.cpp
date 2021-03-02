// TestPipe.cpp�: d�finit le point d'entr�e pour l'application.
//

#include "stdafx.h"
#include "TCltPipe.h"

TCltPipe::TCltPipe(void)
{
}

TCltPipe::TCltPipe(LPCTSTR szPipeName)
{
	CreateSrvNamedPipe(szPipeName);
}

TCltPipe::~TCltPipe(void)
{
	CloseHandle(hPipe);
}


bool TCltPipe::CreateSrvNamedPipe(LPCTSTR szPipeName)
{
	int LastError;
	int i;

	for (i = 0; i < 10; i++) {

		hPipe = CreateFile(szPipeName, GENERIC_READ | GENERIC_WRITE,
															 0, NULL, OPEN_EXISTING,
															 0, NULL);

		if (hPipe != INVALID_HANDLE_VALUE) return true;

		LastError = GetLastError();

		WaitNamedPipe(szPipeName, 2000);

	}
	
	return true;
}

TCltPipeIDruide::TCltPipeIDruide()
{
}

TCltPipeIDruide::TCltPipeIDruide(LPCTSTR szPipeName): TCltPipe(szPipeName)
{
}

bool TCltPipeIDruide::SendCommand(LPCSTR szCommand, LPCSTR szReturn, int Length)
{
	WriteFile(hPipe, szCommand, strlen(szCommand), NULL, NULL);
	FlushFileBuffers(hPipe);
	memset((LPVOID) szReturn, 0, Length);
	if (!strcmp(szCommand, "INFO"))
	{
		ReadFile(hPipe, (LPVOID) szReturn, Length - 1, NULL, NULL);
	}

	return true;
}
