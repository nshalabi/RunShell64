# Debugging Tips

## Debugging using Visual Studio 2019

If (for any reason) you have Visual Studio 2019 on the machine you are using to analyze the Shellcode, then you can utilize the local debugger to debug the Shellcode (**DO THIS IN A SAFE AND ISOLATED MALWARE ANALYSIS LAB**).

To carry out the following steps, install Visual Studio 2019 with MFC (installed with "Desktop development with C++" components). I will use the **"sc-x64-mba-ret.bin"** Shellcode file for testing (shown here loaded in IDA Pro).

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/0.PNG "RunShell64")

To debug the Shellcode, we will use the RunShell64 as the target for debugging, so download RunShell64 source code and open the project in Visual Studio 2019, then start debugging (`Debug -> Start Debugging or use F5`). When the application runs, load the Shellcode file **"sc-x64-mba-ret.bin"** and select the **"Set Breakpoint" option (very important)**, then hit the "Run" button and confirm running the Shellcode when prompt.

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/1.PNG "RunShell64")

Visual Studio will break at the code calling the Shellcode (actually, the function is just a pointer to the Shellcode content loaded in memory)

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/2.PNG "RunShell64")

To debug Shellcode, step into code (`Debug -> Step Into or use F11`), Visual Studio will prompt that it couldn't identify a function frame (as stated earlier, this is because the function we are calling is actually a pointer to an allocated memory region populated with Shellcode, so there is no function call, hence, there is no stack frame)

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/3.PNG "RunShell64")

Click on **"View Disassembly"** to land on the Shellcode as shown here (Compare it to IDA Pro listening shown earlier)

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/4.PNG "RunShell64")

You can now debug Shellcode in Visual Studio. By examining the IDA disassembly earlier, we know that the "MessageBoxA" API function name will be loaded in RDX register (instruction at **0x1BD2ACA0442** `lea rdx, [1BD2AACA05F4h])`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/5.PNG "RunShell64")

To demonstrate how to evaluate pointers, while still debugging, highlight the memory address in `lea rdx, [1BD2AACA05F4h]` instruction, then right-click and select **add to QuickWatch**

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/6.PNG "RunShell64")

To view the value the pointer is pointing to (Ansi string value), we need to instruct the debugger to re-interpret the pointer as a pointer to characters (char*) by typecasting the value. 
Adjust the **"Expression"** to typecast it to a *pointer to character (char\*)* : `((char*)1BD2AACA05F4h)`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/7.PNG "RunShell64")

Click on **"Reevaluate"**, if there is no error (for example, extra or missing parenthesis), then the debugger will reevaluate the value and display the API function name. As an extra step, click on "Add Watch" (so you don't have to keep reevaluating it).

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/8.PNG "RunShell64")

