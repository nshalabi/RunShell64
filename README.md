# RunShell64
## Small utility to help in x64 shellcode analysis and debugging

# Release Notes

-   The tool uses the API function [VirtualProtect](https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect) to adjust the protection of the shellcode memory buffer
-   Additional files handles can be loaded (documents needed by shellcode)-   There is an option to re-run preloaded shellcode or reallocate a new buffer with every run
-   In Version 2.0, I moved from Embarcadero C++Builder & VCL to VC++ 2019 & MFC
-   Two option to debug: Automatic breakpoint (INT3) + Manual breakpoint

# Future enhancements

-   Interception of API calls
-   Provide two options to load additional file handles (CreateFile vs fopen)
-   Adjust registers values

# Testing

-   The "Shellcode" folder contains compiled shellcode
-   The "sc-x64-mba-ret.bin" shellcode calls MessageBox API
-   The "sc-x64-mba-ret - Offset.bin" is the same as "sc-x64-mba-ret.bin" but includes a breakpoint (0xCC) at offset 0x0 and two additional bytes to test offset bypass
-   Shellcode files and their source are the work of [Didier Stevens](https://blog.didierstevens.com/2012/02/02/x64-windows-shellcode/)

To build from source, **VC++ 2019 and MFC** are needed

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/RunShell64.PNG "RunShell64")

# Debugging Tips

#### 1. [Debugging using Visual Studio 2019](https://github.com/nshalabi/RunShell64/blob/master/Extra/DEBUG_VS_2019.md)
#### 2. [Intercepting API calls in Shellcode using Frida](https://github.com/nshalabi/RunShell64/blob/master/Extra/INTERCEPT_API_FRIDA.md)