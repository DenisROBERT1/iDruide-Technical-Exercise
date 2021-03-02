#pragma once

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
