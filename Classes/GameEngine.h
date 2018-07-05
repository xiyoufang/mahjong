//
// Created by farmer on 2018/7/5.
// 游戏引擎

#ifndef COCOSTUDIO_MAHJONG_GAMEENGINE_H
#define COCOSTUDIO_MAHJONG_GAMEENGINE_H

#include "GameCmd.h"
#include "GameLogic.h"


class IPlayer;

class IGameEngineEventListener {

public:
    /**
     * 玩家进入通知
     * @param pIPlayer
     * @return
     */
    virtual bool onUserEnterEvent(IPlayer *pIPlayer) = 0;

    /**
     * 游戏开始
     * @param GameStart
     * @return
     */
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart) = 0;

};


class GameEngine {

private:
    IPlayer *m_pIPlayer[GAME_PLAYER];        //游戏玩家
    uint8_t m_CurrChair;               //当前椅子数量
    GameLogic *m_GameLogic;
    uint32_t iDiceCount;                     //骰子点数
    uint8_t m_cbBankerUser;                   //庄家用户
    uint8_t m_cbCardIndex[GAME_PLAYER][MAX_INDEX];    //用户扑克
    uint8_t m_cbMa;                                   //买马数量
    uint8_t m_cbProvideCard;                //当前供应的牌
    uint8_t m_cbProvideUser;                //供应的玩家
    uint8_t m_cbCurrentUser;                //当前操作的玩家
private:
    uint8_t m_cbLeftCardCount;                       //剩余数目
    uint8_t m_cbRepertoryCard[MAX_REPERTORY];        //库存扑克

public:

    GameEngine();   //构造函数
    ~GameEngine();  //析构函数

public:
    void init();    //初始化数据
    void onGameStart();     //开始游戏
    bool onUserEnter(IPlayer *pIPlayer);    //玩家进入
    bool dispatchCardData(uint8_t cbCurrentUser,bool bTail = false);    //发牌

};


#endif //COCOSTUDIO_MAHJONG_GAMEENGINE_H
