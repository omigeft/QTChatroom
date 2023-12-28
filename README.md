# QTChatroom

## 简介
本项目是一个用QT实现的桌面端自部署聊天室系统软件，含服务端和客户端，使用TCP通信和SQLite数据库管理系统。

#### 运行方法

使用QT编译即可运行。作者测试时使用Qt5.12.9的MinGW64bit编译。

#### 已实现的主要功能有
1. 建立服务器
2. 局域网通信
3. 用户、聊天室管理，包括用户聊天室的创建和消息撤回等
4. 客户端可以进行注册、登录、创建和加入聊天室以及文本聊天等操作

#### 一些待实现的功能
1. 公网通信
2. 通信加密
3. 图片、文件传输等多媒体通信
4. 支持多国语言

#### 软件UI各界面介绍

![UIIntroduction](https://github.com/omigeft/QTChatroom/tree/master/assets/UIIntroduction.png)

#### ER图

![ERDiagram](https://github.com/omigeft/QTChatroom/tree/master/assets/ERDiagram.png)

#### 网络通信示意图

![network](https://github.com/omigeft/QTChatroom/tree/master/assets/network.png)

#### 服务端用户逻辑

![server](https://github.com/omigeft/QTChatroom/tree/master/assets/server.png)

#### 客户端用户逻辑

![cilent](https://github.com/omigeft/QTChatroom/tree/master/assets/client.png)

#### 服务端界面截图

![serverWidget](https://github.com/omigeft/QTChatroom/tree/master/assets/serverWidget.png)

![serverAdminmanagement1](https://github.com/omigeft/QTChatroom/tree/master/assets/serverAdminmanagement1.png)

![serverAdminmanagement2](https://github.com/omigeft/QTChatroom/tree/master/assets/serverAdminmanagement2.png)

![serverUsermanage](https://github.com/omigeft/QTChatroom/tree/master/assets/serverUsermanage.png)

![serverMessagemanage](https://github.com/omigeft/QTChatroom/tree/master/assets/serverMessagemanage.png)

#### 客户端界面截图

![clientMainwindow](https://github.com/omigeft/QTChatroom/tree/master/assets/clientMainwindow.png)

![cilentLoginwindow](https://github.com/omigeft/QTChatroom/tree/master/assets/clientLoginwindow.png)

![cilentChatlist](https://github.com/omigeft/QTChatroom/tree/master/assets/cilentChatlist.png)

![cilentChat](https://github.com/omigeft/QTChatroom/tree/master/assets/cilentChat.png)