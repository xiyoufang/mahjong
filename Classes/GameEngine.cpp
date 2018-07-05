//
// Created by farmer on 2018/7/5.
//

#include "GameEngine.h"
#include "IPlayer.h"

GameEngine::GameEngine() {
    m_currChair = 0;
}

GameEngine::~GameEngine() {

}

/**
 * 玩家进入事件
 * @param pIPlayer
 */
bool GameEngine::onUserEnter(IPlayer *pIPlayer) {
    if (m_currChair >= GAME_PLAYER){
        cocos2d::log("玩家已满，无法加入！");
        return false;
    }
    pIPlayer->setChairID(m_currChair++);
    m_pIPlayer[pIPlayer->getChairID()] = pIPlayer;
    for (int i = 0; i < m_currChair-1; i++) {      //通知全部玩家有人进入游戏
        IGameEngineEventListener *pListener = m_pIPlayer[i]->getGameEngineEventListener();
        if (pListener != NULL){
            pListener->onUserEnterEvent(pIPlayer);
        }
    }
    if (m_currChair == GAME_PLAYER){    //人满了，开始游戏
        onGameStart();
    }
    return true;
}

/**
 * 开始游戏时间
 */
void GameEngine::onGameStart() {


}
