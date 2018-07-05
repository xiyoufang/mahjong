//
// Created by farmer on 2018/7/5.
// 游戏引擎

#ifndef COCOSTUDIO_MAHJONG_GAMEENGINE_H
#define COCOSTUDIO_MAHJONG_GAMEENGINE_H

#include "GameCmd.h"


class IPlayer;

class IGameEngineEventListener{

public:
    /**
     * 玩家进入通知
     * @param pIPlayer
     * @return
     */
    virtual bool onUserEnterEvent(IPlayer *pIPlayer) = 0;

    /**
     * 游戏开始
     * @return
     */
    virtual bool onGameStartEvent() = 0;

};


class GameEngine {

private:
    IPlayer *m_pIPlayer[GAME_PLAYER];        //游戏玩家
    unsigned char m_currChair;               //当前椅子数量
public:

    GameEngine();   //构造函数
    ~GameEngine();  //析构函数

    /**
     * 玩家进入
     */
    bool onUserEnter(IPlayer *pIPlayer);
    /**
     * 开始游戏
     * @return
     */
    void onGameStart();

};


#endif //COCOSTUDIO_MAHJONG_GAMEENGINE_H
