# TeamTalk
	TeamTalk is a solution for enterprise IM
	
	具体文档见doc目录下,安装之前请仔细阅读相关文档。

# 改进

~~~~
1.修复android 加载历史消息的bug
2.为了兼容web端播放音频 android／ios改用ogg+opus的格式编码
3.修复php部分bug
4.修复ios部分bug
5.添加ssl的服务端支持
6.升级protobuf到3.5.1
7.ios使用google 官方的objc pb方便统一pb文件的生成
8.也换了android的pb类 以及protobuf的版本 删了一些不用的依赖库
9.android添加离线文件的上传跟下载功能
10.docker 部署编译的支持
~~~~

~~~~
2018-11-23:
  msg_server 添加线程池处理任务

~~~~




# 测试的msg_server

[http://im.xiaominfc.com:8080/msg_server](http://im.xiaominfc.com:8080/msg_server)


# 在线demo

[http://chat.xiaominfc.com/im/](http://chat.xiaominfc.com/im/)
