@ECHO OFF
REM Must be called from BuildBrewBoot.ps1
CALL EDKSETUP.BAT
CALL BUILD --arch %1 --buildtarget %2 --tagname %3 --platform %4