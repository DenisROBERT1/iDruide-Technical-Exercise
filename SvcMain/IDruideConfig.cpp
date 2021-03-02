// ----------------------------------------------------------------------------
// prog:          IDruideConfig.cpp
//
// Set iDruide Configuration 
// ----------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include <Iphlpapi.h>
#include <Assert.h>

#include "IDruideConfig.h"

#pragma comment(lib, "iphlpapi.lib")


// ----------------------------------------------------------------------------
// Configure the camera policy (enable/disable authorisation)
// TODO : using WMI
// ----------------------------------------------------------------------------

bool ConfigureCamera(bool bEnabled)
{
  HKEY hKey;
	TCHAR *szValue;
  DWORD dwSize;
  LONG Error;
	bool Ok = false;


	// Open registry key LOCAL MACHINE
  Error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\CapabilityAccessManager\\ConsentStore\\webcam"), 0,
                       KEY_READ | KEY_WRITE, &hKey);

  // Write "Value=Allow" or "Value=Deny"
  if (Error == ERROR_SUCCESS)
	{

		if (bEnabled) szValue = TEXT("Allow");
		else szValue = TEXT("Deny");
		dwSize = lstrlen(szValue) * sizeof(TCHAR);
		Error = RegSetValueEx(hKey, _T("Value"), 0, REG_SZ, (LPBYTE) (LPCTSTR) szValue, dwSize);
		if (Error == ERROR_SUCCESS)
		{
			Ok = true;
		}

		RegCloseKey(hKey);
  }

	return Ok;
}

// ----------------------------------------------------------------------------
// Configure the password policy (allow letter or digit only, length)
// ----------------------------------------------------------------------------

bool ConfigurePassword(bool bAllowLetters, bool bAllowDigits, int Length)
{
	return true;
}

// ----------------------------------------------------------------------------
// List OS and computer informations (Ex : MacAddress, OS, Antivirus, Firewall, Battery...)
// ----------------------------------------------------------------------------
// Ordinateur\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion


// ----------------------------------------------------------------------------
// Get MAC Adress
// Source :
// https://stackoverflow.com/questions/13646621/how-to-get-mac-address-in-windows-with-c
// ----------------------------------------------------------------------------
/*
int main(){ 
  char* pMac = getMAC();
  system("pause");
  free(pMac);
}
*/
char* getMAC()
{
  PIP_ADAPTER_INFO AdapterInfo;
  DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
  char *mac_addr = (char*)malloc(18);

  AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
  if (AdapterInfo == NULL)
	{
    printf("Error allocating memory needed to call GetAdaptersinfo\n");
    free(mac_addr);
    return NULL; // it is safe to call free(NULL)
  }

  // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
	{
    free(AdapterInfo);
    AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
    if (AdapterInfo == NULL) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return NULL;
    }
  }

  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
	{
    // Contains pointer to current adapter info
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    do
		{
      // technically should look at pAdapterInfo->AddressLength
      //   and not assume it is 6.
      sprintf_s(mac_addr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
        pAdapterInfo->Address[0], pAdapterInfo->Address[1],
        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
        pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
      printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
      // print them all, return the last one.
      // return mac_addr;

      printf("\n");
      pAdapterInfo = pAdapterInfo->Next;        
    } while(pAdapterInfo);                        
  }
  free(AdapterInfo);
  return mac_addr; // caller must free.
}

HANDLE CreateSrvNamedPipe()
{
  HANDLE hFile;
  hFile = CreateFile(TEXT("\\\\.\\pipe\\iDruide"), GENERIC_READ | GENERIC_WRITE,
                             0, NULL, CREATE_ALWAYS,
                             0, NULL);

	return hFile;
}

void DeleteSrvNamedPipe(HANDLE hPipe)
{
	CloseHandle(hPipe);
}
