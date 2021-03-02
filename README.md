# iDruide-Technical-Exercise
iDruide technical exercise

This document intends to iDruide applicants for technical role as C/C++ Windows engineer.
As a C++ engineer, you must create a Windows service that run automatically at the startup.
This service must be able to :
- Configure the camera policy (enable/disable authorisation) using WMI
- Configure the password policy (allow letter or digit only, length)
- List OS and computer informations (Ex : MacAddress, OS, Antivirus, Firewall, Battery...)
- Reboot the device
The Windows service must communicate with a systray application running in the user
session.
More info about MDM Bridge WMI Provider:
https://docs.microsoft.com/en-gb/windows/win32/dmwmibridgeprov/mdm-bridge-wmiprovider-portal?redirectedfrom=MSDN
Please provide us a brief documenta4on to know how to install and execute the project.
The code must be delivered in a git repo.
Bonus:
• Package the application in an MSI installer.
