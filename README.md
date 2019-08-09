# BrewOS
A 64-bit hobby operating system for the x86_64 architecture.

## Minimum Requirements
*You may be able to get away with using earlier versions of these requirements but they are likely to be untested.*
* Windows 10+ (64-bit)
* Visual Studio 2017+
* Python 3.7+
* NASM 2.14+
* EDK2 UDK2018+

## Getting Started
1. GIT the latest source
	* `git clone https://github.com/coffeeunderrun/BrewOS-64.git --recurse-submodules`
2. Install Python
	* Make sure the Python installation path is included in the PATH environment variable.
	* **NOTE:** The PYTHON_HOME environment variable may need to be set for earlier versions of EDK2.
3. Install NASM
	* Add a new environment variable called NASM_PREFIX and point it to the NASM installation path.
	* **NOTE:** The NASM_PREFIX value must end with a backslash.
4. Open the BrewOS solution in Visual Studio.
5. Build the Emulator project.
6. Build the BrewBoot project.
7. Build the Kernel project.
8. Debug *(F5 or Ctrl+F5)* using **Local Windows Debugger** to run the Windows-Hosted UEFI emulator.

## Resources
[EDK2](https://github.com/tianocore/tianocore.github.io/wiki/Windows-systems) Windows Build

## Authors
* **Jamie** - [coffeeunderrun](https://github.com/coffeeunderrun)

## License
This project is licensed under the GNU General Public License - see the [LICENSE](LICENSE) file for details
