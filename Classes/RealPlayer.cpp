//
// Created by farmer on 2018/7/5.
//

#include "RealPlayer.h"

/**
 * 真实玩家
 * @param sex
 * @param pGameEngineEventListener
 */
RealPlayer::RealPlayer(PlayerSex sex, IGameEngineEventListener *pGameEngineEventListener)
        : IPlayer(false, sex, pGameEngineEventListener) {

}
