//
// Created by farmer on 2018/7/5.
//

#include "GameEngine.h"
#include "IPlayer.h"


static GameEngine *pGameEngine = NULL;

/**
 * 单例
 * @return
 */
GameEngine *GameEngine::GetGameEngine() {
    if (pGameEngine == NULL) {
        pGameEngine = new GameEngine;
    }
    return pGameEngine;
}

GameEngine::GameEngine() {
    m_CurrChair = 0;
    m_GameLogic = new GameLogic;
    init();
}

GameEngine::~GameEngine() {

}

//重置麻将游戏变量
void GameEngine::init() {
    m_cbLeftCardCount = 0;
    m_cbBankerUser = INVALID_CHAIR;
    m_cbCurrentUser = INVALID_CHAIR;
    m_cbProvideUser = INVALID_CHAIR;
    m_cbProvideCard = 0;
    m_cbSendCardCount = 0;
    m_cbSendCardData = 0;
    m_cbMa = 0;
    memset(m_cbGangCard, 0, sizeof(m_cbGangCard));                                    //重置杠的牌
    memset(m_llHuRight, 0, sizeof(m_llHuRight));                                      //清空胡牌类型
    memset(m_cbHuKind, 0, sizeof(m_cbHuKind));                                        //清空胡牌方式
    memset(m_cbHuSpecial, 0, sizeof(m_cbHuSpecial));                                  //清空胡牌方式
    memset(m_cbTempUserAction, 0, sizeof(m_cbTempUserAction));                        //清空临时动作
    memset(m_cbWeaveItemCount, 0, sizeof(m_cbWeaveItemCount));                        //清空组合
    memset(m_cbUserAction, 0, sizeof(m_cbUserAction));                                //玩家动作
    memset(m_cbTempUserAction, 0, sizeof(m_cbTempUserAction));                        //临时动作
    memset(m_cbOperateCard, 0, sizeof(m_cbOperateCard));                              //操作的牌
    memset(m_cbPerformAction, 0, sizeof(m_cbPerformAction));                          //自动默认动作
    memset(m_cbFanShu, 0, sizeof(m_cbFanShu));                                        //结算番数
    for (uint8_t i = 0; i < MAX_WEAVE; ++i) {
        memset(&m_WeaveItemArray[i], 0, sizeof(m_WeaveItemArray[i]));
    }
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
bool GameEngine::onGameStart() {
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
    return true;
}

/**
 * 玩家出牌
 * @param outCard
 */
bool GameEngine::onUserOutCard(CMD_C_OutCard OutCard) {
    if (m_cbUserAction[m_cbCurrentUser] != WIK_NULL) return true;            //存在操作不允许出牌，需要等操作结束
    if (!m_GameLogic->removeCard(m_cbCardIndex[m_cbCurrentUser], OutCard.cbCardData)) { //删除扑克
        return true;
    }
    //用户切换
    m_cbProvideUser = m_cbCurrentUser;
    m_cbProvideCard = OutCard.cbCardData;
    m_cbCurrentUser = static_cast<uint8_t>((m_cbCurrentUser + m_CurrChair - 1) % m_CurrChair);  //切换当前玩家
    m_cbGangCount = 0;                                                  //本手牌杠的情况还原
    memset(m_cbGangCard, 0, sizeof(m_cbGangCard));                      //重置内存
    m_bGangStatus = false;                                              //只要出完牌杠状态为false
    m_bQiangGangStatus = false;                                         //抢杠状态为false
    memset(m_cbTempUserAction, 0, sizeof(m_cbTempUserAction));          //清空临时动作
    m_cbTargetUser = 0;                                                 //重置胡牌人员
    m_cbUserAction[m_cbCurrentUser] = WIK_NULL;                         //出牌的玩家动作为NULL
    m_cbPerformAction[m_cbCurrentUser] = WIK_NULL;                      //默认动作为NULL
    //出牌记录
    m_cbOutCardCount++;
    m_cbOutCardUser = m_cbCurrentUser;
    m_cbOutCardData = OutCard.cbCardData;

    //构造数据
    CMD_S_OutCard SOutCard;
    memset(&SOutCard, 0, sizeof(CMD_S_OutCard));                        //初始化内存
    SOutCard.cbOutCardUser = m_cbProvideUser;                           //出牌的用户
    SOutCard.cbOutCardData = OutCard.cbCardData;                        //出牌的数据
    for (uint8_t i = 0; i < GAME_PLAYER; i++) {
        m_pIPlayer[i]->getGameEngineEventListener()->onOutCardEvent(SOutCard); //出牌时间
    }
    bool bAroseAction = estimateUserRespond(m_cbCurrentUser, OutCard.cbCardData, EstimateKind_OutCard);     //响应判断
    if (!bAroseAction) dispatchCardData(m_cbCurrentUser);    //派发扑克
    return true;
}

/**
 * 发牌
 * @param cbCurrentUser
 *  当前玩家
 * @param bTail
 *  杠牌的发牌
 * @return
 */
bool GameEngine::dispatchCardData(uint8_t cbCurrentUser, bool bTail) {

    if ((m_cbOutCardUser != INVALID_CHAIR) && (m_cbOutCardData != 0))                        //往出牌记录里添加上一位出牌数据
    {
        m_cbDiscardCount[m_cbOutCardUser]++;
        m_cbDiscardCard[m_cbOutCardUser][m_cbDiscardCount[m_cbOutCardUser] - 1] = m_cbOutCardData;
    }
    m_cbTargetUser = 0;                                                               //重置操作人员
    m_cbOutCardData = 0;                                                              //重置出牌数据
    m_cbOutCardUser = INVALID_CHAIR;                                                  //重置出牌人员
    m_cbGangCount = 0;                                                                //重置杠的数量
    memset(m_cbGangCard, 0, sizeof(m_cbGangCard));                                    //重置杠的牌
    memset(m_llHuRight, 0, sizeof(m_llHuRight));                                      //清空胡牌类型
    memset(m_cbHuKind, 0, sizeof(m_cbHuKind));                                        //清空胡牌方式
    memset(m_cbHuSpecial, 0, sizeof(m_cbHuSpecial));                                  //清空胡牌方式
    memset(m_cbTempUserAction, 0, sizeof(m_cbTempUserAction));                        //清空临时动作
    m_cbCurrentUser = cbCurrentUser;                                                  //设置当前玩家
    m_cbFanShu[cbCurrentUser] = 0;                                                    //牌过手则设置本局番数为0
    memset(m_cbPassPeng[cbCurrentUser], 0, sizeof(m_cbPassPeng[cbCurrentUser]));       //牌过手重置同一张牌碰牌检测
    //剩余牌 == 马数量
    if (m_cbLeftCardCount == m_cbMa) {                                                //没牌发了,荒庄结束
        m_cbHuCard = 0;
        m_cbProvideUser = INVALID_CHAIR;
        onEventGameConclude(INVALID_CHAIR);                                     //流局
        return true;
    }
    m_cbSendCardCount++;                                                              //发牌数据计数
    m_cbSendCardData = m_cbRepertoryCard[--m_cbLeftCardCount];                        //获取要发的具体牌
    m_cbCardIndex[cbCurrentUser][m_GameLogic->switchToCardIndex(m_cbSendCardData)]++; //将牌发给当前玩家
    m_cbProvideUser = cbCurrentUser;                                                  //设置供应用户为当前玩家
    m_cbProvideCard = m_cbSendCardData;                                               //设置供应扑克为当前发的牌
    if (m_cbLeftCardCount > 0)                                                        //暗杠判定，剩下的牌>1才能杠
    {
        tagGangCardResult GangCardResult;
        m_cbUserAction[cbCurrentUser] |= m_GameLogic->analyseGangCard(m_cbCardIndex[cbCurrentUser], m_WeaveItemArray[cbCurrentUser], m_cbWeaveItemCount[cbCurrentUser], GangCardResult);
        if ((m_cbUserAction[cbCurrentUser] & WIK_G) != 0x0) {                                //判定是否杠牌
            //记录杠的数量
            m_cbGangCount = GangCardResult.cbCardCount;
            memcpy(m_cbGangCard, GangCardResult.cbCardData, sizeof(m_cbGangCard));    //杠的数量
        }
    }
    //胡牌判断
    uint8_t cbTempCardIndex[MAX_INDEX];
    memcpy(cbTempCardIndex, m_cbCardIndex[m_cbCurrentUser], sizeof(cbTempCardIndex));
    m_GameLogic->removeCard(cbTempCardIndex, m_cbSendCardData);    //移除发的那张牌进行分析
    //如果胡牌则是自摸
    m_cbUserAction[cbCurrentUser] |= m_GameLogic->analyseHuCard(cbTempCardIndex, m_WeaveItemArray[cbCurrentUser], m_cbWeaveItemCount[cbCurrentUser], m_cbSendCardData, m_cbHuKind[cbCurrentUser], m_llHuRight[cbCurrentUser], m_cbHuSpecial[cbCurrentUser], m_cbSendCardCount, m_cbOutCardCount, m_bGangStatus, true, m_bQiangGangStatus, m_cbFanShu[cbCurrentUser], false);
    if (m_cbUserAction[cbCurrentUser] != WIK_NULL) {    //存在暗杠、拐弯杠、或者自摸
        m_cbTempUserAction[cbCurrentUser] = m_cbUserAction[cbCurrentUser];
    }
    //构造数据
    CMD_S_SendCard SendCard;
    SendCard.cbCurrentUser = cbCurrentUser;
    SendCard.cbActionMask = m_cbUserAction[cbCurrentUser];
    SendCard.cbCardData = m_cbSendCardData;
    SendCard.cbGangCount = m_cbGangCount;
    memcpy(&SendCard.cbGangCard, m_cbGangCard, sizeof(m_cbGangCard));
    SendCard.bTail = bTail;

    for (uint8_t i = 0; i < GAME_PLAYER; i++) {
        m_pIPlayer[i]->getGameEngineEventListener()->onSendCardEvent(SendCard); //出牌数据只发送给当前玩家(网游不能这么发牌，网游其他人的cbCardData要重置防止 透视挂)
    }
    return true;
}

bool GameEngine::estimateUserRespond(uint8_t cbCurrentUser, uint8_t cbCurrentCard, EstimateKind estimateKind) {
    return false;
}

/**
 * 游戏结束
 * @param cbChairID
 */
bool GameEngine::onEventGameConclude(uint8_t cbChairID) {

    return true;
}

