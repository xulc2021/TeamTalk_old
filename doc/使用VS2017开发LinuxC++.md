# 简介

[https://docs.microsoft.com/zh-cn/cpp/ide/cmake-tools-for-visual-cpp?view=vs-2017](Visual C++ 中的 CMake 项目)

在windows上开发linux c++会遇到以下3大问题：
1.文件格式换行的问题，在windows下编辑的问题在Linux上换行符无法识别
2.文件同步
3.代码调试，如断点、实时变量等

要解决以上问题，比较好的方式建议使用带GUI的Linux系统，但是如果宿主机是windows+虚拟机的方式，建议安装使用vs2017。

## linux配置

### 安装GDB

yum install openssh-server g++ gdb gdbserver

### 安装CMake 3.10

[https://blogs.msdn.microsoft.com/vcblog/2017/08/25/visual-c-for-linux-development-with-cmake/](参考)
[https://blog.csdn.net/jiang_xinxing/article/details/77945478](Centos7安装高版本Cmake)

> install -y git cmake                                  # 安装2.8cmake
> git clone https://github.com/Kitware/CMake.git        # 下载CMake源码
> cd CMake
> git checkout tags/v3.10.0                             # 切换到3.10.0
> mkdir out
> cd out
> cmake ../                                             # 生成makefile
> make                                                  # 编译cmkae
> sudo make install                                     # 安装cmake3.10.0

请确认版本号>3.8以上
> /usr/local/bin/cmake –version

请确认：servermode=true
> cmake -E capabilities

PS：如果提示需要cmake到3.1以上，按以下步骤：
> git checkout tags/v3.9.0                              # 设定到3.9tag
> mkdir out
> cd out
> cmake ../                                             # 生成makefile
> make                                                  # 编译cmkae
> sudo make install                                     # 安装cmake3.9.0
> yum remove cmake -y
> ln -s /usr/local/bin/cmake /usr/bin/                  # 添加软链接
> cmake --version
> 请按照上面的步骤，checkout 3.10重新编译安装

[可选，非必需]新建软链接：
> yum remove cmake -y
> ln -s /usr/local/bin/cmake /usr/bin/
> cmake --version

## 安装vs2017

1.双击mu_visual_studio_community_2017_version_15.3_x86_x64_11100062.exe
2.勾选"使用c++的linux开发"
3.更改位置到D:
4.安装即可

参考：[https://www.cnblogs.com/yearsj/p/9600682.html](vs2017搭建linux c++开发环境)

## 配置远程连接

1.请参考[https://docs.microsoft.com/zh-cn/cpp/linux/connect-to-your-remote-linux-computer?view=vs-2017](连接到远程 Linux 计算机)

## 配置CMake

1.在CMakeLists.txt上右键"更改 CMake 设置"
2.在打开的文件中更改：name为项目文件夹，比如login_server
3.更改：remoteCMakeListsRoot为Linux的在CMakeLists.txt所在目录，比如：/home/svn/TeamTalk/server/src/login_server
4.更改：remoteBuildRoot，设置MakeFile生成目录，如：/home/svn/TeamTalk/server/src/bin/${name}
5.保存

保存时vs2017会自动拷贝代码到/home/svn/TeamTalk/server/src/login_server，然后执行CMake命令生成MakeFile文件到/home/svn/TeamTalk/server/src/bin/login_server。
此时，可使用make手动编译：
> cd /home/svn/TeamTalk/server/src/bin/login_server
> make

## 编译

### 编译前准备

make指令链接静态库时，查找文件路径是相对于：/home/svn/TeamTalk/server/src/bin/login_server。所以需要修正在CMakeLists.txt
1.编辑CMakeLists.txt
2.增加：
SET(SRC_DIR  ../../) # CMAKE的输出路径为TeamTalk\server\src\bin\${name}，${name}代表项目名称，如login_server。../../定位到src下，并且只需要连接静态库时，增加该前缀
SET(PB_LIB_DIR ${SRC_DIR}/protobuf/lib)
SET(SLOG_LIB_DIR ${SRC_DIR}/slog)

即调整链接lib目录相对于/home/svn/TeamTalk/server/src/bin/login_server即可。

### 生成

选择CMake->全部生成。即可在远程上编译成功

### 调试

只能调试进程，动态库如何调试暂不清楚。

选择启动项>login_server后，点击即可。

### 推荐插件

推荐安装：VAssistX插件
1.关键字高亮
2.文件头注释目录、函数、类注释模板等
