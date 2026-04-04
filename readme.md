# ZDoom dev build recreation
intended date: December 9th, 2005

## Requirements
* [NASM 0.98.39](https://sourceforge.net/projects/nasm/files/Win32%20binaries/0.98.39/nasm-0.98.39-win32.zip/download)
* [FMOD 3.75 Programmers API](https://web.archive.org/web/20061016195047/http://www.fmod.org/files/fmodapi375win.zip)
* [DirectX SDK (December 2005)](https://archive.org/details/dxsdk_dec2005)
* Microsoft Visual C++ .NET 2003
 
## Environment setup
1. Extract NASM and FMOD into a location (remember where they are!). 2. Install both Visual C++ .NET 2003 and the DirectX SDK.
3. Open up Visual C++, and go to Tools > Options. Scroll down and click "Projects", and then "VC++ Directories". Make sure the Platform is Win32 and the "Show directories for" is set to "Executable files". Add the NASM root directory.
4. After doing that, go to "Show directories for" and select "Include files". Add both the DirectX SDK (DXSDKLOCATION/Include) and FMOD (FMODLOCATION/api/inc). 
5. After doing that, go to "Show directories for" and select "Library files". Add both the DirectX SDK (DXSDKLOCATION/Lib/x86) and FMOD (FMODLOCATION/api/lib). 
 
## Building ZDoom
Before compiling, go to Build > Configuration Manager, and change "Active Solution Configuration" from "Debug" to "Release". Afterwards, go to Build > Build Solution. You might get a fair few warnings but should compile the following:
<table style="border-collapse: collapse; width: 100%; height: 72px; color: #ffffff" border="0">
<tbody>
<tr style="color: #FFD800;" "height: 18px;">
<td style="width: 20%; height: 18px;" align="center">C:\zdoom.exe</td>
<td style="width: 20%; height: 18px;" align="left">ZDoom executable</td>
</tr>
<tr style="color: #FFD800;" "height: 18px;">
<td style="width: 20%; height: 18px;" align="center">C:\zdoom.wad</td>
<td style="width: 20%; height: 18px;" align="left">Supporting ZDoom data</td>
</tr>
</table>