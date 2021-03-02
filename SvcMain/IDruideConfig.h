// ----------------------------------------------------------------------------
// prog:          IDruideConfig.h
//
// Set iDruide Configuration 
// ----------------------------------------------------------------------------


bool ConfigureCamera(bool bEnabled);
bool ConfigurePassword(bool bAllowLetters, bool bAllowDigits, int Lenght);
char* getOS();
char* getMAC();

HANDLE CreateSrvNamedPipe();
void DeleteSrvNamedPipe(HANDLE hPipe);

