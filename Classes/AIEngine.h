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
    GameLogic *m_GameLogic;         //游戏逻辑
    IPlayer *m_MePlayer;            //AI玩家
    uint8_t m_cbSendCardData;       //发送的牌

private:
    tagWeaveItem m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];                 //组合
    uint8_t m_cbCardIndex[GAME_PLAYER][MAX_INDEX];                         //玩家牌
    uint8_t m_cbWeaveItemCount[GAME_PLAYER];                               //组合数目
    uint8_t m_cbDiscardCount[GAME_PLAYER];                                 //丢弃数目
    uint8_t m_cbDiscardCard[GAME_PLAYER][MAX_DISCARD];                     //丢弃记录
    uint8_t m_cbLeftCardCount;                                             //剩余
    uint8_t m_cbBankerChair;                                               //庄
    uint8_t m_MeChairID;                                                   //自己的位置
public:
    AIEngine();    //构造函数
    ~AIEngine();                    //析构
    void initGame();                //初始化游戏变量
    void setIPlayer(IPlayer *pIPlayer);    //设置Player
public:
    virtual bool onUserEnterEvent(IPlayer *pIPlayer);           //玩家进入游戏事件
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart);   //游戏开始事件
    virtual bool onSendCardEvent(CMD_S_SendCard SendCard);      //发牌事件
    virtual bool onOutCardEvent(CMD_S_OutCard OutCard);         //出牌事件
    virtual bool onOperateNotifyEvent(CMD_S_OperateNotify OperateNotify);   //操作通知事件
    virtual bool onOperateResultEvent(CMD_S_OperateResult OperateResult);   //操作结果事件
    virtual bool onGameEndEvent(CMD_S_GameEnd GameEnd);                     //游戏结束事件
    //操作
public:
    void sendCard(float f);

};


#endif //COCOSTUDIO_MAHJONG_AIENGINE_H
