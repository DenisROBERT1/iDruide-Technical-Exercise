@ECHO OFF

ECHO install the service
SvcMain install
Pause

ECHO start the service
SvcControl.exe start Svc_iDruide
Pause

ECHO update the service description
SvcConfig.exe describe Svc_iDruide
Pause

ECHO query the service configuration
SvcConfig.exe query Svc_iDruide
Pause

ECHO change the service DACL
SvcControl.exe dacl Svc_iDruide
Pause
