@ECHO OFF

SET "ACTION=%~1"
SET "WORKSPACE=%~2"
SET "PACKAGES_PATH=%WORKSPACE%\EDK2"
SET "PATH=%PATH%;%WORKSPACE%;%PACKAGES_PATH%"

REM Get Visual Studio version
IF "%~3" == "15.0" SET TAG_NAME=VS2017

REM Get build configuration
IF /I "%~4" == "RELEASE" (
	SET BUILD_TARGET=RELEASE
) ELSE (
	SET BUILD_TARGET=DEBUG
)

REM Prepare EDK2 build environment
CALL EDKSETUP.BAT --nt32
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
	REM Clean the Windows-Hosted UEFI emulator build path
	CALL BUILD cleanall --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform Nt32Pkg\Nt32Pkg.dsc
	IF NOT ERRORLEVEL 0 GOTO :END
	REM Clean the BrewBoot UEFI application build path
	CALL BUILD cleanall --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform BrewBoot\BrewBoot.dsc
	GOTO :END

:BUILD
	REM Build the Windows-Hosted UEFI emulator
	CALL BUILD all --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform Nt32Pkg\Nt32Pkg.dsc
	IF NOT ERRORLEVEL 0 GOTO :END
	REM Build the BrewBoot UEFI application
	CALL BUILD all --arch X64 --buildtarget %BUILD_TARGET% --tagname %TAG_NAME% --platform BrewBoot\BrewBoot.dsc
	IF NOT ERRORLEVEL 0 GOTO :END
	REM Copy the BrewBoot UEFI application and startup script to the emulator build path
	COPY /Y "%WORKSPACE%\Build\BrewBoot\%BUILD_TARGET%_%TAG_NAME%\X64\BrewBoot.efi" "%WORKSPACE%\Build\NT32X64\%BUILD_TARGET%_%TAG_NAME%\X64"
	COPY /Y "%WORKSPACE%\Build\BrewBoot\%BUILD_TARGET%_%TAG_NAME%\X64\startup.nsh" "%WORKSPACE%\Build\NT32X64\%BUILD_TARGET%_%TAG_NAME%\X64"
	GOTO :END

:END
