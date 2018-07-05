//
// Created by farmer on 2018/7/4.
//

#ifndef COCOSTUDIO_MAHJONG_GAMELAYER_H
#define COCOSTUDIO_MAHJONG_GAMELAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BaseUILayer.h"
#include "IPlayer.h"
#include "GameCmd.h"
#include "Utility.h"
#include "GameEngine.h"

using namespace cocos2d;

class GameLayer : public BaseUILayer, IGameEngineEventListener {

private:
    Node *m_FaceFrame[GAME_PLAYER];   //头像信息节点
    IPlayer *m_Players[GAME_PLAYER];  //全部玩家
    unsigned char m_CurPlayer;        //当前玩家数量
    GameEngine *m_GameEngine;          //游戏引擎

public:
    CREATE_FUNC(GameLayer)
    GameLayer();    // 构造函数
    ~GameLayer();   //析构
protected:
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);  //按钮事件处理
    virtual void initLayer();           //初始化层
public:
    virtual Node *GetLayer();           //获取层Layer
    void aiEnterGame(float f);          //机器人进入游戏

private:
    virtual bool onUserEnterEvent(IPlayer *pIPlayer);

    virtual bool onGameStartEvent();
};


#endif //COCOSTUDIO_MAHJONG_GAMELAYER_H
