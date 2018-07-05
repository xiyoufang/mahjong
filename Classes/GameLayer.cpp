//
// Created by farmer on 2018/7/4.
//

#include "GameLayer.h"
#include "AIPlayer.h"
#include "RealPlayer.h"
#include "UIHelper.h"


GameLayer::GameLayer() {
    m_CurPlayer = 0;
    m_GameEngine = new GameEngine;  //构造游戏引擎
    initLayer();
    schedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame), 1.0f);    //创建个定时任务，用来添加机器人
}

GameLayer::~GameLayer() {

}

/**
 * AI 进入游戏
 * @param f
 */
void GameLayer::aiEnterGame(float f) {
    if(!m_GameEngine->onUserEnter(new AIPlayer(this))){ //机器人玩家加入游戏，返回false说明已经满了
        unschedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame));//人满，关闭定时任务
    };
}

void GameLayer::initLayer() {
    cocos2d::log("GameLayer initLayer");
    m_pLayer = CSLoader::createNode("res/GameLayer.csb");   //加载Cocostudio创建的Layer
    m_pLayer->addChild(this, -1);
    for (unsigned char i = 0; i < GAME_PLAYER; i++) {       //初始化头像节点数组
        m_FaceFrame[i] = UIHelper::seekNodeByName(m_pLayer, utility::toString("face_frame_", i));
    }
    setTouchEventListener(m_pLayer);
    RealPlayer *pIPlayer = new RealPlayer(this);
    m_GameEngine->onUserEnter(pIPlayer);    //玩家加入游戏
}

Node *GameLayer::GetLayer() {
    return m_pLayer;
}


bool GameLayer::onUserEnterEvent(IPlayer *pIPlayer) {
    m_Players[m_CurPlayer++] = pIPlayer;
    if (m_CurPlayer == GAME_PLAYER){
        unschedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame));   //人满，关闭ai加入任务
    }
    return true;
}

bool GameLayer::onGameStartEvent() {
    cocos2d::log("接收到游戏开始事件");   //接收到游戏开始事件

    return true;
}



void GameLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {

}