#pragma once

class TCltPipe
{
private:
protected:
	HANDLE hPipe;
public:
	TCltPipe(void);
	TCltPipe(LPCTSTR szPipeName);
	~TCltPipe(void);

	bool CreateSrvNamedPipe(LPCTSTR szPipeName);
	virtual bool SendCommand(LPCSTR szCommand, LPCSTR szReturn, int Length) = 0;
};

class TCltPipeIDruide: public TCltPipe
{
private:
protected:
public:
	TCltPipeIDruide();
	TCltPipeIDruide(LPCTSTR szPipeName);

	bool SendCommand(LPCSTR szCommand, LPCSTR szReturn, int Length);
};
