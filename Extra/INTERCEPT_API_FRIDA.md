# Debugging Tips

## Intercepting API calls in Shellcode using Frida

Any API calls made by Shellcode may be intercepted using hooking libraries, such MS Detours (https://github.com/microsoft/detours) or EasyHook (https://easyhook.github.io/) and many others, however, intercepting Win32 APIs using those libraries requires some modifications to be made to the C/C++ code in addition to knowing the hooked API functions arguments and return types beforehand.

To make these tasks simpler, I will use Frida (https://frida.re/), a dynamic instrumentation and interception toolkit. Frida will allow us to intercept the Win32 APIs without any modifications to the code, combined with RunShell64 notification, we can even intercept and reduce the noise generated from other parts of the code that we are not interested in.

To carry out the following steps, install Python 3.x and Frida (https://frida.re/docs/installation/). I will use the **"sc-x64-mba-ret.bin"** Shellcode file for testing (which calls MessageBoxA and LoadLibraryA API functions)

Open RunShell64.exe and load **"sc-x64-mba-ret.bin"** Shellcode file, hit the "Run" command and DON'T CONFIRM running the Shellcode yet. 
Then open a command prompt and attach Frida to the RunShell64 Process ID (-p) instructing the tool to include the MessageBoxA and LoadLibrary API functions (-i):

`frida-trace -p 2056 -i MessageBoxA -i LoadLibraryA`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/1.PNG "RunShell64")

Frida will generate **__handlers__** folder in the current directory with JS files (in sub-directories mapped to modules/dlls) to handle events related to function calling/leaving (onEnter/onLeave) for the  functions we have specified in our command

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/1.5.PNG "RunShell64")

Let us jump back to RunShell64 and hit Ok to run the Shellcode while monitoring the output from frida-trace

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/2.PNG "RunShell64")

Simple! Let us do something more useful, let us adjust the JS code in the **__handlers__** folder, following is the original file

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/3.PNG "RunShell64")

We will adjust the code in **LoadLibraryA.JS** to know which DLL is being requested by Shellcode, the following is the updated version

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/4.PNG "RunShell64")

Run the Shellcode again and let us check the output

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/5.PNG "RunShell64")

This is not what we wanted, our JS code needs to be modified to work with the first argument (LPCSTR lpLibFilname) as an ANSI string (MessageBoxA uses ANSI strings) using `Memory.readAnsiString(args[0])`

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/6.PNG "RunShell64")

Let us run the Shellcode again

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/7.PNG "RunShell64")

No need to modify any code or utilize any hooking libraries to intercept Win32 APIs. Let us examine one more case that might come handy during malware analysis in a lab environment, altering API function arguments. We now know that the Shellcode calls MessageBoxA API function (https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa), let us adjust the message box text by altering the arguments passed to the `onEnter` function in the **MessageBoxA.JS** file

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/8.PNG "RunShell64")

Run the Shellcode again

![RunShell64](https://github.com/nshalabi/RunShell64/blob/master/Art/FRIDA/9.PNG "RunShell64")

Frida is a powerful toolkit and supported on many platforms, including mobile platforms.

