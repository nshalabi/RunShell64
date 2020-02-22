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

To build from source, **Embarcadero C++Builder** is needed, a community edition can be downloaded from [Embarcadero](https://www.embarcadero.com) website

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/RunShell64.PNG "RunShell64")