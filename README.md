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

##Debugging using Visual Studio 2019

If (for any reason) you have Visual Studio 2019 on the machine you are using to analyze the Shellcode, then you can utilize the local debugger to debug the Shellcode (same strict rules should apply regarding the safe and isolated environment for malware analysis).

I will use the "sc-x64-mba-ret.bin" for testing and Visual Studio 2019 with MFC. This is the Shellcode loaded in IDA Pro

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/0.PNG "RunShell64")

Download the RunShell64 and open the project in Visual Studio 2019, run (Debug -> Start Debugging or use F5), load the Shellcode file "sc-x64-mba-ret.bin", select the "Set Breakpoint" option and run the Shellcode, then confirm running the Shellcode prompt.

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/1.PNG "RunShell64")

Visual Studio will break at the code calling the Shellcode, actually, the function is a pointer to the Shellcode loaded in memory

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/2.PNG "RunShell64")

To debug Shellcode, step into code (Debug -> Step Into or use F11), Visual Studio will prompt that it couldn't identify a function frame (as stated earlier, this is because the function we are calling is actually a pointer to an allocated memory region populated with Shellcode, so there is no function call, hence, no stack frame)

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/3.PNG "RunShell64")

Click on "View Disassembly" to land on the Shellcode as shown here (Compare it to IDA Pro showed previously)

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/4.PNG "RunShell64")

You can now debug Shellcode, despite the lack of convenience provided by other standalone debuggers.
Examining IDA Disassembly, we know that the "MessageBoxA" API function name will be loaded in RDX register (instruction at 0x1BD2ACA0442 `lea rdx, [1BD2AACA05F4h])`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/5.PNG "RunShell64")

To demonstrate how to evaluate memory addresses, while still debugging, select the memory address in `lea rdx, [1BD2AACA05F4h]` instruction, right-click and select add to QuickWatch...

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/6.PNG "RunShell64")

To view the value the pointer is pointing to (string value), we need to direct the debugger to interpret the pointer as a pointer to characters (char*) by typecasting the value. 
Adjust the "Expression" to typecast it to a character pointer (char*) : `((char*)1BD2AACA05F4h)`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/7.PNG "RunShell64")

Click on "Reevaluate", if no error (for example, extra or missing parenthesis), click on "Add Watch" (not needed, but in case this pointer changes during the run, then you don't need to keep reevaluating it).

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/8.PNG "RunShell64")

