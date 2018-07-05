//
// Created by farmer on 2018/7/5.
//

#include "GameEngine.h"
#include "IPlayer.h"

GameEngine::GameEngine() {
    m_CurrChair = 0;
    init();
}

GameEngine::~GameEngine() {

}


void GameEngine::init() {
    m_cbLeftCardCount = 0;
    m_cbBankerUser = INVALID_CHAIR;
    m_cbCurrentUser = INVALID_CHAIR;
    m_cbProvideUser = INVALID_CHAIR;
    m_cbProvideCard = 0;
    m_GameLogic = new GameLogic;
    for (uint8_t i = 0; i < GAME_PLAYER; ++i) {
        memset(&m_cbCardIndex[i], 0, sizeof(m_cbCardIndex[i]));
    }
}

/**
 * 玩家进入事件
 * @param pIPlayer
 */
bool GameEngine::onUserEnter(IPlayer *pIPlayer) {
    if (m_CurrChair >= GAME_PLAYER) {
        cocos2d::log("玩家已满，无法加入！");
        return false;
    }
    pIPlayer->setChairID(m_CurrChair++);
    m_pIPlayer[pIPlayer->getChairID()] = pIPlayer;
    for (uint8_t i = 0; i < m_CurrChair; i++) {      //通知全部玩家有人进入游戏
        IGameEngineEventListener *pListener = m_pIPlayer[i]->getGameEngineEventListener();
        if (pListener != NULL) {
            pListener->onUserEnterEvent(pIPlayer);
        }
    }
    if (m_CurrChair == GAME_PLAYER) {    //人满了，开始游戏
        onGameStart();
    }
    return true;
}

/**
 * 开始游戏事件
 */
void GameEngine::onGameStart() {
    m_GameLogic->shuffle(m_cbRepertoryCard, sizeof(m_cbRepertoryCard));        //洗牌
    iDiceCount = static_cast<uint32_t>(rand() % 6 + 1 + rand() % 6 + 1);    //骰子点数
    if (m_cbBankerUser == INVALID_CHAIR) {
        m_cbBankerUser = static_cast<uint8_t>(iDiceCount % GAME_PLAYER);        //确定庄家
    }
    m_cbLeftCardCount = sizeof(m_cbRepertoryCard);  //剩余排
    for (uint8_t i = 0; i < m_CurrChair; i++) {
        m_cbLeftCardCount -= (MAX_COUNT - 1);            //发牌13张
        m_GameLogic->switchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount], MAX_COUNT - 1, m_cbCardIndex[i]); //初始化用户扑克到 m_cbCardIndex 数组
    }
    //设置变量
    m_cbProvideCard = 0;            //初始化供应扑克
    m_cbProvideUser = INVALID_CHAIR;    //初始化供应玩家
    m_cbCurrentUser = m_cbBankerUser;    //设置当前操作玩家为庄家
    //构造数据
    CMD_S_GameStart GameStart;
    GameStart.iDiceCount = iDiceCount;
    GameStart.cbBankerUser = m_cbBankerUser;
    GameStart.cbCurrentUser = m_cbCurrentUser;
    GameStart.cbLeftCardCount = m_cbLeftCardCount - m_cbMa;

    for (int i = 0; i < m_CurrChair; i++) {      //通知全部玩家开始游戏
        m_GameLogic->switchToCardData(m_cbCardIndex[i], GameStart.cbCardData, MAX_COUNT);
        if (m_pIPlayer[i]->isAndroid()) {   //机器人作弊用，用于分析其他玩的牌
            uint8_t bIndex = 1;
            for (uint8_t j = 0; j < GAME_PLAYER; j++) {
                if (j == i) continue;
                m_GameLogic->switchToCardData(m_cbCardIndex[j], &GameStart.cbCardData[MAX_COUNT * bIndex++], MAX_COUNT);
            }
        }
        IGameEngineEventListener *pListener = m_pIPlayer[i]->getGameEngineEventListener();
        if (pListener != NULL) {
            pListener->onGameStartEvent(GameStart);
        }
    }
    dispatchCardData(m_cbCurrentUser);
}

/**
 * 发牌
 * @param cbCurrentUser
 *  当前玩家
 * @param bTail
 *  杠牌的发牌
 * @return
 */
bool GameEngine::dispatchCardData(uint8_t cbCurrentUser,bool bTail){



    return true;
}