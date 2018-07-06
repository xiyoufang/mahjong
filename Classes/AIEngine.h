//
// Created by farmer on 2018/7/5.
//  AI 逻辑

#ifndef COCOSTUDIO_MAHJONG_AIENGINE_H
#define COCOSTUDIO_MAHJONG_AIENGINE_H

#include "GameEngine.h"
#include "cocos2d.h"

class AIEngine : public IGameEngineEventListener, cocos2d::Node {

private:
    GameEngine *m_GameEngine;       //游戏引擎
    IPlayer *m_MePlayer;            //AI玩家
    uint8_t m_cbSendCardData;       //发送的牌

public:
    AIEngine();    //构造函数
    ~AIEngine();                    //析构
    void setIPlayer(IPlayer *pIPlayer);    //设置Player
public:
    virtual bool onUserEnterEvent(IPlayer *pIPlayer);           //玩家进入游戏事件
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart);   //游戏开始事件
    virtual bool onSendCardEvent(CMD_S_SendCard SendCard);      //发牌事件
    virtual bool onOutCardEvent(CMD_S_OutCard OutCard);         //出牌事件

    //操作
public:
    void sendCard(float f);
};


#endif //COCOSTUDIO_MAHJONG_AIENGINE_H
