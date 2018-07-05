//
// Created by farmer on 2018/7/5.
//  AI 逻辑

#ifndef COCOSTUDIO_MAHJONG_AIENGINE_H
#define COCOSTUDIO_MAHJONG_AIENGINE_H

#include "GameEngine.h"

class AIEngine : public IGameEngineEventListener{

public:
    AIEngine();
    ~AIEngine();
public:

    virtual bool onUserEnterEvent(IPlayer *pIPlayer);           //玩家进入游戏事件
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart);

    virtual bool onSendCardEvent(CMD_S_SendCard SendCard);
    //游戏开始事件

};


#endif //COCOSTUDIO_MAHJONG_AIENGINE_H
