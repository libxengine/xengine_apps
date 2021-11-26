# XEngine_Apps

XEngine V7 Version example

#### Engine Example
The example code of the engine contains the following examples, and the corresponding component module examples can be found through the following directory structure as needed  
please use vs2019 open it,and use x86 and debug mode.  

1. XEngine_AVCoder                  audio and video   
2. XEngine_Client                   client  
3. XEngine_Core                     service  
4. XEngine_DownLoad                 download  
5. XEngine_HelpComponents           help  
6. XEngine_Lib                      base library  
7. XEngine_NetHelp                  nethelp  
9. XEngine_RfcComponents            rfc protocol    
10. XEngine_StreamMedia             stream media  
11. XEngine_SystemSdk               system  

#### use
To use our code example. You may need to make some modifications. WINDOWS can be opened and compiled directly with VS2019, and LINUX can be compiled with commands  
Due to different environments, the directory structure of each user may be different. Therefore, the header file and the module directory are also different. Before you compile, you need to modify the loaded directory structure in our code according to your own directory structure to be correct Compile and use. Not only the loading directory of WINDOWS, but the compiling command of linux also needs to modify the directory address as needed.

## WINDOWS
To use it under WINDOWS, you need to install VS2019. You can compile and allow it directly, provided that your engine directory is configured, and you need to modify the engine directory of the loaded header file to your own directory.
## LINUX
To use under LINUX. You can use the command to compile, gdb debugging. The compiled command is in the code. You can directly copy the commented compilation command, open the terminal, switch to the current directory, and type directly to compile.

# 安装示例
在使用我们例子,你需要先按照引擎的README说明安装.    
如果你是VC编译,你需要在项目属性里面vc++ 目录中添加包含目录和库目录.  
然后修改代码,比如
头文件:#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h" 修改为: #include <XEngine_Include/XEngine_CommHdr.h>  
库文件:#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib") 修改为 #pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib") 
# Installation example
Before using our example, you need to install it first according to the engine's README instructions.
If you are compiling with VC, you need to add the include directory and library directory to the vc++ directory in the project properties.
Then modify the code, such as
Header file: #include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h" Modified to: #include <XEngine_Include/XEngine_CommHdr.h>
Library file: #pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib") Modified to #pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")