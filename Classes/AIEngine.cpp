//
// Created by farmer on 2018/7/5.
//

#include "AIEngine.h"


AIEngine::AIEngine() {
}

AIEngine::~AIEngine() {
}

bool AIEngine::onUserEnterEvent(IPlayer *pIPlayer) {
    return false;
}

bool AIEngine::onGameStartEvent(CMD_S_GameStart GameStart) {
    cocos2d::log("机器人接收到游戏开始事件");
    return false;
}
