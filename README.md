# COCOS2D-X 单机麻将

#### 项目介绍
基于最新版 Cocos2d-X 3.17 与 cocostudio V3.10 开发的单机麻将游戏，麻将算法为商业级麻将服务器端算法，整个项目代码精简、注释详细。

- 如果你刚刚接触Cocos2d-X 这绝对是个不错的入门项目
- 如果是你C++初学者或者其他语言转游戏开发，这项目没有复杂的语法绝对是一个不错的学习项目
- 如果你是游戏开发从业者想转行棋牌行业，这项目的棋牌算法绝对能够给予你启发

![截图](https://images.gitee.com/uploads/images/2018/0706/103629_69b97848_369917.jpeg "Simulator Screen Shot - iPhone 5s - 2018-07-06 at 10.21.08.jpg")

![输入图片说明](https://images.gitee.com/uploads/images/2018/0707/141833_47a9c5ce_369917.jpeg "Simulator Screen Shot - iPhone 5s - 2018-07-07 at 11.22.14副本.jpg")

![输入图片说明](https://images.gitee.com/uploads/images/2018/0707/141846_21c4009b_369917.jpeg "Simulator Screen Shot - iPhone 5s - 2018-07-07 at 14.01.40副本.jpg")

#### 软件架构

* 游戏引擎基于最新版本 Cocos2d-X v3.17
* 游戏UI设计使用 Cocostudio3.10
* 游戏使用C++开发
* 商业级麻将算法

##### 相关下载：

* [cocos2d-x-3.17下载](http://www.cocos2d-x.org/filedown/cocos2d-x-3.17.zip)
* [Windows版本下载，CocosForWin-v3.10.exe](http://www.cocos2d-x.org/filedown/CocosForWin-v3.10.exe)
* [MAC版本下载，CocosForMac-v3.10](http://www.cocos2d-x.org/filedown/CocosForMac-v3.10.dmg)

#### 安装教程

1. 由于Cocos2d源码比较庞大，GIT上不包括Cocos2d-X 3.17源码，所以第一步需要下载并且安装 [cocos2d-x-3.17](http://www.cocos2d-x.org/filedown/cocos2d-x-3.17.zip) 安装说明移驾 [最新版本Cocos2d-X V3.17开发环境搭建](https://www.xiyoufang.com/archives/183) 安装好后使用命令行创建个新项目，将项目里的cocos2d文件夹拷到本项目下，
![输入图片说明](https://images.gitee.com/uploads/images/2018/0706/131106_5add28c6_369917.png "WX20180706-130947.png")
2. 下载安装好Cocostudio3.10，打开UI.ccs文件即可编辑和发布UI
![UI编辑器](https://images.gitee.com/uploads/images/2018/0706/105227_36709445_369917.png "WX20180706-105153.png")
3. MAC直接用Xcode 或者AppCode 直接打开 proj.ios_mac 即可运行
![输入图片说明](https://images.gitee.com/uploads/images/2018/0706/125845_3a993ad4_369917.jpeg "WX20180706-125720.jpg")
4. AndroidStudio安装直接打开 proj.android 项目即可运行
![输入图片说明](https://images.gitee.com/uploads/images/2018/0706/114443_07ff095f_369917.jpeg "WX20180706-113529.jpg")
5. Visual Studio 安装未验证

#### 源码说明

##### UI项目说明

* cocosstudio目录为UI目录
* UI.ccs是UI项目配置文件

| 文件 | 说明 |
| -------- | -------- |
|HelloLayer.csd|欢迎界面层|
|AlertLayer.csd|弹框层|
|GameLayer.csd|游戏层|
|SetLayer.csd|设置层|
|GameOverLayer.csd|游戏结算层|
|Gang0.csd|横向显示杠的节点|
|Peng0.csd|横向显示碰的节点|
|Gang1.csd|竖向显示杠的节点|
|Peng1.csd|竖向显示碰的节点|
|BtnPeng.csd|碰按钮节点|
|BtnGang.csd|杠按钮节点|
|BtnHu.csd|胡按钮节点|
|BtnGuo.csd|过按钮节点|
|EffectZm.csd|自摸特效|
|EffectPeng.csd|碰特效|
|EffectHu.csd|胡特效|
|EffectGang.csd|杠特效|
|SignAnim.csd|标记当前出牌动画|
|~~MainScene.csd~~|~~没用~~|

##### 源码说明

| 文件 | 说明 |
| -------- | -------- |
|AppDelegate.h/AppDelegate.cpp|Cocos2d游戏启动类|
|GameSceneManager.h/GameSceneManager.cpp|单例，封装创建提示层、设置Scene和Layer等|
|AlertLayer.h/AlertLayer.cpp|弹出层类|
|HelloWorldScene.h/HelloWorldScene.cpp|游戏Scene类|
|BaseUILayer.h/BaseUILayer.cpp|Cocostudio界面基类，封装了部分按钮事件绑定功能|
|HelloLayer.h/HelloLayer.cpp|游戏初始欢迎界面类|
|GameLayer.h/GameLayer.cpp|游戏层界面类，用来控制游戏的显示|
|SetLayer.h/SetLayer.cpp|游戏设置层|
|GameConfig.h/GameConfig.cpp|加载与存储游戏设置信息|
|ViewControl.h/ViewControl.cpp|通过观察者模式，控制界面切换|
|ViewObject.h/ViewObject.cpp|封装界面切换指令与数据|
|UIHelper.h/UIHelper.cpp|UI工具类，封装节点查找方法|
|Utility.h|工具类，封装字符串操作方法|
|IPlayer.h|游戏玩家基类|
|AIPlayer.h/AIPlayer.cpp|机器人玩家|
|RealPlayer.h/RealPlayer.cpp|真实玩法|
|GameEngine.h/GameEngine.cpp|麻将引擎，控制游戏逻辑流转|
|GameLogic.h/GameLogic.cpp|麻将算法类|
|GameCmd.h|麻将传递的对象定义|
|AIEngine.h/AIEngine.cpp|机器人引擎|
|FvMask.h|标记位操作工具|

* GameEngine.h中定义的IGameEngineEventListener接口，该接口用于玩家处理游戏事件，AIEngine实现了机器人的游戏事件处理接口，GameLayer中实现了真实玩家的游戏事件处理接口。


#### 参与贡献

1. Fork 本项目
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request


#### 其他

1. 个人博客 [www.xiyoufang.com](https://www.xiyoufang.com) 获取更多软件开发信息
2. gitee项目首页 [https://gitee.com/xiyoufang/cocostudio-mahjong](https://gitee.com/xiyoufang/cocostudio-mahjong)
3. github项目首页 [https://github.com/xiyoufang/mahjong](https://github.com/xiyoufang/mahjong)

### 您也可以加入游戏开发交流QQ群：112958956 ，一起讨论游戏开发技术。

![输入图片说明](https://images.gitee.com/uploads/images/2018/0708/183503_d1f599f2_369917.png "temp_qrcode_share_112958956.png")