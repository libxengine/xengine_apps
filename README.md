# XEngine_Apps

xengine 例子代码

# 引擎例子
引擎的例子代码包含下面的例子,可以根据需要通过下面的目录结构找到相应的组件模块例子.  

1. XEngine_AVCoder                  音视频组件  
2. XEngine_Client                   客户端组件  
3. XEngine_Core                     核心服务组件  
4. XEngine_DownLoad                 下载组件  
5. XEngine_HelpComponents           帮助组件  
6. XEngine_Lib                      基础组件  
7. XEngine_NetHelp                  网络组件  
9. XEngine_RfcComponents            协议标准组件  
10. XEngine_StreamMedia             流媒体组件  
11. XEngine_SystemSdk               系统组件  

# 使用说明
要使用我们的代码例子.你可能需要做一部分修改.WINDOWS可以直接用VS2019打开并且编译,LINUX可以使用命令编译  
由于环境不同,每个使用者的目录结构可能也不同.所以,头文件和模块目录也不一样.你在编译前,需要根据自己的目录结构修改我们代码中的加载的目录结构,才可以正确编译使用.不光是WINDOWS的加载目录,linux的编译命令也需要根据需要修改目录地址.

## WINDOWS下使用
要在WINDOWS下使用,你需要安装VS2019.可以直接编译和允许,前提是配置好你的引擎目录,你需要把加载的头文件的引擎目录修改为你自己的目录.
## LINUX下使用
要在LINUX下使用.你可以使用命令编译,gdb调试.编译的命令在代码里面有.可以直接复制注释的编译命令,打开终端,切换到当前目录,直接输入即可编译.