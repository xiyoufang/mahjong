//
// Created by farmer on 2018/7/5.
//

#include "AIEngine.h"
#include "IPlayer.h"

AIEngine::AIEngine() {
    m_GameEngine = GameEngine::GetGameEngine();
    m_cbSendCardData = 0;
    GameSceneManager::getInstance()->getScene()->addChild(this, -1);
    //将节点加入到场景，用于启动定时任务
}

AIEngine::~AIEngine() {
    cocos2d::log("~AIEngine");
}

void AIEngine::setIPlayer(IPlayer *pIPlayer) {
    m_MePlayer = pIPlayer;
}

bool AIEngine::onUserEnterEvent(IPlayer *pIPlayer) {
    return false;
}

bool AIEngine::onGameStartEvent(CMD_S_GameStart GameStart) {
    cocos2d::log("机器人接收到游戏开始事件");
    return true;
}

bool AIEngine::onSendCardEvent(CMD_S_SendCard SendCard) {
    cocos2d::log("机器人接收到发牌事件");
    if (SendCard.cbCurrentUser == m_MePlayer->getChairID()) { //出牌
        m_cbSendCardData = SendCard.cbCardData;
        scheduleOnce(CC_SCHEDULE_SELECTOR(AIEngine::sendCard), static_cast<float > (time(NULL) % 2 + 0.5f));
    }
    return true;
}


void AIEngine::sendCard(float f) {
    CMD_C_OutCard OutCard;
    memset(&OutCard, 0, sizeof(CMD_C_OutCard));
    OutCard.cbCardData = m_cbSendCardData;
    m_GameEngine->onUserOutCard(OutCard);
}

bool AIEngine::onOutCardEvent(CMD_S_OutCard OutCard) {
    return false;
}
