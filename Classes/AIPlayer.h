//
// Created by farmer on 2018/7/5.
//  机器人玩家

#ifndef COCOSTUDIO_MAHJONG_AIPLAYER_H
#define COCOSTUDIO_MAHJONG_AIPLAYER_H

#include "IPlayer.h"

class AIPlayer : public IPlayer{

public:
    AIPlayer(PlayerSex sex ,IGameEngineEventListener* pGameEngineEventListener);

};


#endif //COCOSTUDIO_MAHJONG_AIPLAYER_H
