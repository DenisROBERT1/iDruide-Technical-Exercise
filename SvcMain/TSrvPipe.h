// ----------------------------------------------------------------------------
// prog:          TSrvPipe.h
//
// Named pipe communication between user interface and the service (server side) 
// ----------------------------------------------------------------------------

#pragma once

bool Trace(const void *Buf, int Count);

class TSrvPipe
{
private:
protected:
	HANDLE hPipe;
public:
	TSrvPipe(void);
	TSrvPipe(LPCTSTR szPipeName);
	~TSrvPipe(void);

	bool CreateSrvNamedPipe(LPCTSTR szPipeName);
	bool ProcessSrvNamedPipe();
	virtual bool ProcessCommand(LPCSTR szCommand) = 0;
};

class TSrvPipeIDruide: public TSrvPipe
{
private:
protected:
public:
	TSrvPipeIDruide(LPCTSTR szPipeName);

	virtual bool ProcessCommand(LPCSTR szCommand);
};
