//
// Created by farmer on 2018/7/5.
//

#include "AIEngine.h"
#include "IPlayer.h"

AIEngine::AIEngine() {
    m_GameEngine = GameEngine::GetGameEngine();
    m_GameLogic = new GameLogic();
    m_cbSendCardData = 0;
    memset(&m_cbCardIndex, 0, sizeof(m_cbCardIndex));
    memset(&m_cbWeaveItemCount, 0, sizeof(m_cbWeaveItemCount));
    memset(&m_cbDiscardCount, 0, sizeof(m_cbDiscardCount));
    memset(&m_cbDiscardCard, 0, sizeof(m_cbDiscardCard));
    m_MeChairID = INVALID_CHAIR;
    m_cbBankerChair = INVALID_CHAIR;
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
    m_MeChairID = m_MePlayer->getChairID();
    return true;
}

bool AIEngine::onGameStartEvent(CMD_S_GameStart GameStart) {
    cocos2d::log("机器人接收到游戏开始事件");
    m_cbLeftCardCount = GameStart.cbLeftCardCount;
    m_cbBankerChair = GameStart.cbBankerUser;
    m_GameLogic->switchToCardIndex(GameStart.cbCardData, MAX_COUNT - 1, m_cbCardIndex[m_MeChairID]);
    return true;
}

bool AIEngine::onSendCardEvent(CMD_S_SendCard SendCard) {
    cocos2d::log("机器人接收到发牌事件");
    if (SendCard.cbCurrentUser == m_MeChairID) { //出牌
        m_cbLeftCardCount--;
        if (SendCard.cbCurrentUser == m_MeChairID) {
            m_cbCardIndex[m_MeChairID][m_GameLogic->switchToCardIndex(SendCard.cbCardData)]++;
        }
        m_cbSendCardData = SendCard.cbCardData;
        scheduleOnce(CC_SCHEDULE_SELECTOR(AIEngine::sendCard), time(NULL) % 2 + 0.5f);
    }
    return true;
}

/**
 * 出牌事件
 * @param OutCard
 * @return
 */
bool AIEngine::onOutCardEvent(CMD_S_OutCard OutCard) {
    cocos2d::log("机器人接收到出牌事件");
    if (OutCard.cbOutCardUser == m_MeChairID) {
        m_cbCardIndex[m_MeChairID][m_GameLogic->switchToCardIndex(OutCard.cbOutCardData)]--;
    }
    m_cbDiscardCard[OutCard.cbOutCardUser][m_cbDiscardCount[OutCard.cbOutCardUser]++] = OutCard.cbOutCardData;
    return true;
}

/**
 * 操作通知事件
 * @param OperateNotify
 * @return
 */
bool AIEngine::onOperateNotifyEvent(CMD_S_OperateNotify OperateNotify) {
    cocos2d::log("机器人接收到操作通知事件");
    if (OperateNotify.cbActionMask == WIK_NULL) {
        return true; //无动作
    }
    CMD_C_OperateCard OperateCard;
    memset(&OperateCard, 0, sizeof(CMD_C_OperateCard));     //重置内存
    OperateCard.cbOperateUser = m_MeChairID;   //操作的玩家
    if ((OperateNotify.cbActionMask & WIK_H) != 0) {        //胡的优先级最高
        OperateCard.cbOperateCode = WIK_H;
        OperateCard.cbOperateCard = OperateNotify.cbActionCard;
    } else if ((OperateNotify.cbActionMask & WIK_G) != 0) { //杠的优先级第二
        OperateCard.cbOperateCode = WIK_G;
        OperateCard.cbOperateCard = OperateNotify.cbGangCard[0];//杠第一个
    } else if ((OperateNotify.cbActionMask & WIK_P) != 0) { //碰的优先级第三
        OperateCard.cbOperateCode = WIK_P;
        OperateCard.cbOperateCard = OperateNotify.cbActionCard;
    }
    return m_GameEngine->onUserOperateCard(OperateCard);
}

/**
 * 操作结果事件
 * @param OperateResult
 * @return
 */
bool AIEngine::onOperateResultEvent(CMD_S_OperateResult OperateResult) {
    cocos2d::log("机器人接收到操作结果事件");
    tagWeaveItem weaveItem;
    memset(&weaveItem, 0, sizeof(tagWeaveItem));
    switch (OperateResult.cbOperateCode) {
        case WIK_NULL: {
            break;
        }
        case WIK_P: {
            weaveItem.cbWeaveKind = WIK_P;
            weaveItem.cbCenterCard = OperateResult.cbOperateCard;
            weaveItem.cbPublicCard = TRUE;
            weaveItem.cbProvideUser = OperateResult.cbProvideUser;
            weaveItem.cbValid = TRUE;
            m_WeaveItemArray[OperateResult.cbOperateUser][m_cbWeaveItemCount[OperateResult.cbOperateUser]++] = weaveItem;
            if (OperateResult.cbOperateUser == m_MeChairID) { //自己出牌操作
                uint8_t cbReomveCard[] = {OperateResult.cbOperateCard, OperateResult.cbOperateCard};
                m_GameLogic->removeCard(m_cbCardIndex[OperateResult.cbOperateUser], cbReomveCard, sizeof(cbReomveCard));
                m_cbSendCardData = m_GameLogic->switchToCardData(m_cbCardIndex[m_MeChairID][0]); //碰完需要出一张
                scheduleOnce(CC_SCHEDULE_SELECTOR(AIEngine::sendCard), time(NULL) % 2 + 0.5f);
            }
            break;
        }
        case WIK_G: {
            weaveItem.cbWeaveKind = WIK_G;
            weaveItem.cbCenterCard = OperateResult.cbOperateCard;
            uint8_t cbPublicCard = (OperateResult.cbOperateUser == OperateResult.cbProvideUser) ? FALSE : TRUE;
            int j = -1;
            for (int i = 0; i < m_cbWeaveItemCount[OperateResult.cbOperateUser]; i++) {
                tagWeaveItem tempWeaveItem = m_WeaveItemArray[OperateResult.cbOperateUser][i];
                if (tempWeaveItem.cbCenterCard == OperateResult.cbOperateCard) {   //之前已经存在
                    cbPublicCard = TRUE;
                    j = i;
                }
            }
            weaveItem.cbPublicCard = cbPublicCard;
            weaveItem.cbProvideUser = OperateResult.cbProvideUser;
            weaveItem.cbValid = TRUE;
            if (j == -1) {
                m_WeaveItemArray[OperateResult.cbOperateUser][m_cbWeaveItemCount[OperateResult.cbOperateUser]++] = weaveItem;
            } else {
                m_WeaveItemArray[OperateResult.cbOperateUser][j] = weaveItem;
            }
            if (OperateResult.cbOperateUser == m_MeChairID) {  //自己
                m_GameLogic->removeAllCard(m_cbCardIndex[OperateResult.cbOperateUser], OperateResult.cbOperateCard);
            }
            break;
        }
        case WIK_H: {
            break;
        }
        default:
            break;
    }
    return true;
}

/**
 * 出牌
 * @param f
 */
void AIEngine::sendCard(float f) {
    CMD_C_OutCard OutCard;
    memset(&OutCard, 0, sizeof(CMD_C_OutCard));
    OutCard.cbCardData = m_cbSendCardData;
    m_GameEngine->onUserOutCard(OutCard);
}

