# RunShell64
## Small utility to help in x64 shellcode analysis and debugging

# Release Notes

-   The tool uses the API function [VirtualProtect](https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect) to adjust the protection of the shellcode memory buffer
-   Additional file handles can be loaded (documents needed by shellcode)
-   There is an option to re-run preloaded shellcode or reallocate a new buffer with every run

# Future enhancements

-   Interception of API calls
-   Provide two options to load additional file handles (CreateFile vs fopen)
-   Adjust registers values

# Testing

-   The "Shellcode" folder contains compiled shellcode
-   The "sc-x64-mba-ret.bin" shellcode calls MessageBox API
-   The "sc-x64-mba-ret - Offset.bin" is the same as "sc-x64-mba-ret.bin" but includes a breakpoint (0xCC) at offset 0x0 and two additional bytes to test offset bypass
-   Shellcode files and their source are the work of [Didier Stevens](https://blog.didierstevens.com/2012/02/02/x64-windows-shellcode/)

To build from source, **Embarcadero C++Builder** is needed, a community edition can be downloaded from [Embarcadero](https://www.embarcadero.com) website

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/RunShell64.PNG "RunShell64")