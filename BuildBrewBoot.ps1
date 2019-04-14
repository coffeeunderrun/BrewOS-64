Param
(
	[string] $nasmVersion = "2.14.02",
	[string] $nasmUrl = ("https://www.nasm.us/pub/nasm/releasebuilds/{0}/win64/nasm-{0}-win64.zip" -f $nasmVersion),
	[string] $pythonVersion = "3.7.3",
	[string] $pythonUrl = ("https://www.python.org/ftp/python/{0}/python-{0}.exe" -f $pythonVersion),
	[string] $workspacePath = $PSScriptRoot,
	[string] $toolPath = [System.IO.Path]::Combine($workspacePath, "Tools"),
	[string] $packagesPath = [System.IO.Path]::Combine($workspacePath, "EDK2"),
	[string] $toolChain = "VS2017",
	[bool] $releaseBuild = $false
)

# Verify running as administrator
$isElevated = [bool]([System.Security.Principal.WindowsIdentity]::GetCurrent().Groups -match "S-1-5-32-544")
if($isElevated -eq $false)
{
	Write-Output "Please run this script as administrator."
	exit
}

# Make sure EDK2 is available
$edkSetupPath = [System.IO.Path]::Combine($packagesPath, "edksetup.bat")
if(!(Test-Path -Path $edkSetupPath))
{
	Write-Error "Unable to find EDK2 setup. Specify a PackagesPath or use GIT to pull the EDK2 submodule. (git submodule update --init --force --recursive)" -Category NotInstalled
	exit
}

if(!(Test-Path -Path $toolPath))
{
	New-Item -ItemType Directory -Path $toolPath
}

# Install NASM if it isn't already
$nasmPath = [System.IO.Path]::Combine($toolPath, ("nasm-{0}" -f $nasmVersion))
if(!(Test-Path -Path $nasmPath))
{
	$nasmFile = [System.IO.Path]::GetFileName($nasmUrl)
	$nasmArchive = [System.IO.Path]::Combine($toolPath, $nasmFile)

	# Dowload archive if it doesn't exist
	if(!(Test-Path -Path $nasmArchive))
	{
		Write-Host "Downloading $nasmFile..."
		Start-BitsTransfer -Source $nasmUrl -Destination $nasmArchive
	}

	# Extract NASM from archive
	Write-Host "Extracting $nasmFile..."
	Expand-Archive $nasmArchive -DestinationPath $toolPath
}

# Install Python if it isn't already
if((&{python -V} 2>&1) -is [System.Management.Automation.ErrorRecord])
{
	$pythonFile = [System.IO.Path]::GetFileName($pythonUrl)
	$pythonInstaller = [System.IO.Path]::Combine($toolPath, $pythonFile)

	# Download installer if it doesn't exist
	if(!(Test-Path -Path $pythonInstaller))
	{
		Write-Host "Downloading $pythonFile..."
		Start-BitsTransfer -Source $pythonUrl -Destination $pythonInstaller
	}

	# Install Python
	Write-Host "Installing $pythonFile..."
	& $pythonInstaller /quiet InstallAllUsers=1 PrependPath=1 Include_test=0

	# Refresh environment path after install
	$env:Path = "{0};{1}" -f
	[System.Environment]::GetEnvironmentVariable("Path", "Machine"),
	[System.Environment]::GetEnvironmentVariable("Path", "User")

	# Make sure Python successfully installed
	if((&{python -V} 2>&1) -is [System.Management.Automation.ErrorRecord])
	{
		Write-Error "Unable to find Python installation." -Category NotInstalled
		exit
	}
}

# Set environment variables
[System.Environment]::SetEnvironmentVariable("WORKSPACE", $workspacePath)
[System.Environment]::SetEnvironmentVariable("PACKAGES_PATH", $packagesPath)
[System.Environment]::SetEnvironmentVariable("NASM_PREFIX", "$nasmPath\")
$env:Path = "{0};{1};{2};{3}" -f $env:Path, $workspacePath, $packagesPath, $nasmPath

# Build EDK2 BaseTools
$baseToolsPath = [System.IO.Path]::Combine($packagesPath, "BaseTools\Bin\Win32")
if(!(Test-Path -Path $baseToolsPath))
{
	& "$edkSetupPath" "--nt32", "Rebuild"
}

# Build BrewBoot UEFI Application
$target = if($releaseBuild -eq $true) { "RELEASE" } else { "DEBUG" }
& BuildBrewBoot.bat "X64", "$target", "$toolChain", "OvmfPkg\OvmfPkgX64.dsc"
& BuildBrewBoot.bat "X64", "$target", "$toolChain", "BrewBoot\BrewBoot.dsc"