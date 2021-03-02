@ECHO OFF

ECHO uninstall the service
SvcConfig disable Svc_iDruide
Pause

ECHO stop the service
SvcControl stop Svc_iDruide
Pause

ECHO delete the service
SvcConfig delete Svc_iDruide
Pause
