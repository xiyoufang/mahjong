//
// Created by farmer on 2018/7/5.
// 真实玩家

#ifndef COCOSTUDIO_MAHJONG_REALPLAYER_H
#define COCOSTUDIO_MAHJONG_REALPLAYER_H

#include "IPlayer.h"

class RealPlayer : public IPlayer {
public:
    RealPlayer(PlayerSex sex ,IGameEngineEventListener* pGameEngineEventListener);
};


#endif //COCOSTUDIO_MAHJONG_REALPLAYER_H
