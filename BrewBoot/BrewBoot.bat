@ECHO OFF

SET "ACTION=%~1"
SET "WORKSPACE=%~2"
SET "PACKAGES_PATH=%WORKSPACE%\EDK2"
SET "PATH=%PATH%;%WORKSPACE%;%PACKAGES_PATH%"
SET "OUTPUT_PATH=%~3"

REM Get Visual Studio version
IF "%~4" == "15.0" SET TAG_NAME=VS2017

REM Get build configuration
IF /I "%~5" == "RELEASE" (
	SET BUILD_TARGET=RELEASE
) ELSE (
	SET BUILD_TARGET=DEBUG
)

REM Prepare EDK2 build environment
IF EXIST "%PACKAGES_PATH%\BaseTools\Bin\Win32\" (
	CALL EDKSETUP.BAT %TAG_NAME%
) ELSE (
	CALL EDKSETUP.BAT %TAG_NAME% rebuild
)
IF NOT ERRORLEVEL 0 GOTO :END

REM Check for CLEAN action
IF /I "%ACTION%" == "CLEAN" (
	PUSHD "%WORKSPACE%"
	CALL :CLEAN
	POPD
	GOTO :END
)

REM Check for BUILD action
IF /I "%ACTION%" == "BUILD" (
	PUSHD "%WORKSPACE%"
	CALL :BUILD
	POPD
	GOTO :END
)

REM Check for REBUILD action
IF /I "%ACTION%" == "REBUILD" (
	PUSHD "%WORKSPACE%"
	CALL :CLEAN
	CALL :BUILD
	POPD
	GOTO :END
)

REM Unsupported action
GOTO :END

:CLEAN
	CALL BUILD cleanall --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform BrewBoot\BrewBoot.dsc
	GOTO :END

:BUILD
	CALL BUILD all --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform BrewBoot\BrewBoot.dsc --define OUTPUT_PATH="%OUTPUT_PATH%"
	ROBOCOPY "%WORKSPACE%\Build\BrewBoot\%BUILD_TARGET%_%TAG_NAME%\X64\BrewBoot\BrewBoot\OUTPUT\." "%OUTPUT_PATH%\." BrewBoot.efi BrewBoot.pdb
	GOTO :END

:END
