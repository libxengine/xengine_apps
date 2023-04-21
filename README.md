# XEngine_Apps

xengine v8 例子代码

# 引擎例子
引擎的例子代码包含下面的例子,可以根据需要通过下面的目录结构找到相应的组件模块例子.  
请使用vs2022打开,并且使用 x86和debug模式

1. XEngine_AVCodec                  音视频组件  
2. XEngine_Client                   客户端组件  
3. XEngine_Core                     核心服务组件  
5. XEngine_HelpComponents           帮助组件  
6. XEngine_Lib                      基础组件  
7. XEngine_NetHelp                  网络组件  
9. XEngine_RfcComponents            协议标准组件  
10. XEngine_StreamMedia             流媒体组件  
11. XEngine_SystemSdk               系统组件  

# 使用说明
要使用我们的代码例子.你可能需要做一部分修改.WINDOWS可以直接用VS打开并且编译,LINUX或者MACOS可以使用命令编译  
由于环境不同,每个使用者的目录结构可能也不同.所以,头文件和模块目录也不一样.你在编译前,需要根据自己的目录结构修改我们代码中的加载的目录结构,才可以正确编译使用.不光是WINDOWS的加载目录,linux和MACOS的编译命令也需要根据需要修改目录地址.  

## WINDOWS下使用
要在WINDOWS下使用,你需要安装VS.可以直接编译,前提是配置好你的引擎目录,你需要把加载的头文件的引擎目录修改为你自己的目录.
## LINUX Macos下使用
你可以使用命令编译,gdb调试.编译的命令在代码里面有.可以直接复制注释的编译命令,打开终端,切换到当前目录,直接输入即可编译.

# 安装示例
在使用我们例子,你需要先按照引擎的README说明安装  
然后修改代码,比如  
头文件:#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h" 修改为: #include <XEngine_Include/XEngine_CommHdr.h>  
库文件:#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib") 修改为 #pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib") 