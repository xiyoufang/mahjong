//
// Created by farmer on 2018/7/4.
//

#include <cocos/audio/include/SimpleAudioEngine.h>
#include "GameLayer.h"
#include "AIPlayer.h"
#include "RealPlayer.h"
#include "AIEngine.h"
#include "SetLayer.h"
#include "GameConfig.h"


GameLayer::GameLayer() {
    m_CurPlayer = 0;
    m_MeChairID = 0;
    m_GameEngine = GameEngine::GetGameEngine();  //构造游戏引擎
    m_GameLogic = new GameLogic;
    m_bOperate = false;
    m_bMove = false;
    m_pOperateNotifyGroup = NULL;
    m_pTextCardNum = NULL;
    m_pGameOverNode = NULL;
    m_pOutCard = NULL;
    m_iOutCardTimeOut = 30;
    initGame();
    initLayer();
    schedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame), 1.0f);    //创建个定时任务，用来添加机器人
}

GameLayer::~GameLayer() {

}

/**
 * 初始化游戏变量
 */
void GameLayer::initGame() {
    m_cbLeftCardCount = 0;
    m_cbBankerChair = INVALID_CHAIR;
    memset(&m_cbWeaveItemCount, 0, sizeof(m_cbWeaveItemCount));
    memset(&m_cbDiscardCount, 0, sizeof(m_cbDiscardCount));
    for (uint8_t i = 0; i < GAME_PLAYER; i++) {
        memset(m_cbCardIndex[i], 0, sizeof(m_cbCardIndex[i]));
        memset(m_WeaveItemArray[i], 0, sizeof(m_WeaveItemArray[i]));
        memset(m_cbDiscardCard[i], 0, sizeof(m_cbDiscardCard[i]));
    }
}

/**
 * AI 进入游戏
 * @param f
 */
void GameLayer::aiEnterGame(float) {
    //机器人玩家加入游戏，返回false说明已经满了，随机生成性别
    if (!m_GameEngine->onUserEnter(new AIPlayer(time(NULL) % 2 == 0 ? IPlayer::MALE : IPlayer::FEMALE, new AIEngine))) {
        unschedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame));//人满，关闭定时任务
    };
}

void GameLayer::initLayer() {
    cocos2d::log("GameLayer initLayer");
    m_pLayer = CSLoader::createNode("res/GameLayer.csb");   //加载Cocostudio创建的Layer
    m_pLayer->addChild(this, -1);
    for (unsigned char i = 0; i < GAME_PLAYER; i++) {       //初始化头像节点数组
        m_FaceFrame[i] = UIHelper::seekNodeByName(m_pLayer, utility::toString("face_frame_", (int) i));
        m_PlayerPanel[i] = UIHelper::seekNodeByName(m_pLayer, utility::toString("PlayerPanel_", (int) i));
    }
    m_pOperateNotifyGroup = UIHelper::seekNodeByName(m_pLayer, "OperateNotifyGroup");   //操作节点
    m_pTextCardNum = dynamic_cast<ui::Text *>(UIHelper::seekNodeByName(m_pLayer, "Text_LeftCard"));   //操作节点
    setTouchEventListener(m_pLayer);
    RealPlayer *pIPlayer = new RealPlayer(IPlayer::MALE, this);
    m_GameEngine->onUserEnter(pIPlayer);    //玩家加入游戏
}

Node *GameLayer::GetLayer() {
    return m_pLayer;
}


bool GameLayer::onUserEnterEvent(IPlayer *pIPlayer) {
    m_Players[m_CurPlayer++] = pIPlayer;
    for (unsigned char i = 0; i < m_CurPlayer; i++) {           //显示头像
        ui::ImageView *pImageHeader = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_FaceFrame[i], "Image_Header"));  //头像
        ui::Text *pTextScore = dynamic_cast<ui::Text *>(UIHelper::seekNodeByName(m_FaceFrame[i], "Text_Score"));  //头像
        pTextScore->setString("0");     //进来分数为0
        pImageHeader->loadTexture(utility::toString("res/GameLayer/im_defaulthead_", m_Players[i]->getSex() == IPlayer::FEMALE ? 0 : 1, ".png"));    //设置头像
    }
    if (m_CurPlayer == GAME_PLAYER) {
        unschedule(CC_SCHEDULE_SELECTOR(GameLayer::aiEnterGame));   //人满，关闭ai加入任务
        return true;
    } //显示头像
    return true;
}

bool GameLayer::onGameStartEvent(CMD_S_GameStart GameStart) {
    cocos2d::log("接收到游戏开始事件");   //接收到游戏开始事件
    initGame();
    //调整头像位置
    m_FaceFrame[0]->runAction(MoveTo::create(0.50f, Vec2(0080.00f, 250.00f)));
    m_FaceFrame[1]->runAction(MoveTo::create(0.50f, Vec2(0080.00f, 380.00f)));
    m_FaceFrame[2]->runAction(MoveTo::create(0.50f, Vec2(1060.00f, 640.00f)));
    m_FaceFrame[3]->runAction(MoveTo::create(0.50f, Vec2(1200.00f, 380.00f)));
    //剩余的牌
    m_cbLeftCardCount = GameStart.cbLeftCardCount;
    m_cbBankerChair = GameStart.cbBankerUser;
    m_GameLogic->switchToCardIndex(GameStart.cbCardData, MAX_COUNT - 1, m_cbCardIndex[m_MeChairID]);
    //界面显示
    m_pTextCardNum->setString(utility::toString((int) m_cbLeftCardCount));
    showAndUpdateHandCard();
    showAndUpdateDiscardCard();
    return true;
}

/**
 * 发牌事件
 * @param SendCard
 * @return
 */
bool GameLayer::onSendCardEvent(CMD_S_SendCard SendCard) {
    m_iOutCardTimeOut = 30; //重置计时器
    m_cbLeftCardCount--;
    if (SendCard.cbCurrentUser == m_MeChairID) {
        m_cbCardIndex[m_MeChairID][m_GameLogic->switchToCardIndex(SendCard.cbCardData)]++;
    }
    return showSendCard(SendCard);
}

/**
 * 出牌事件
 * @param OutCard
 * @return
 */
bool GameLayer::onOutCardEvent(CMD_S_OutCard OutCard) {
    if (OutCard.cbOutCardUser == m_MeChairID) {
        m_cbCardIndex[m_MeChairID][m_GameLogic->switchToCardIndex(OutCard.cbOutCardData)]--;
    }
    m_cbDiscardCard[OutCard.cbOutCardUser][m_cbDiscardCount[OutCard.cbOutCardUser]++] = OutCard.cbOutCardData;
    uint8_t cbViewID = switchViewChairID(OutCard.cbOutCardUser);
    cocostudio::timeline::ActionTimeline *action = CSLoader::createTimeline("res/SignAnim.csb");
    action->gotoFrameAndPlay(0, 10, true);
    Node *pSignNode = CSLoader::createNode("res/SignAnim.csb");
    pSignNode->setName("SignAnim");
    std::vector<Node *> aChildren;
    aChildren.clear();
    UIHelper::getChildren(m_pLayer, "SignAnim", aChildren);
    for (auto &subWidget : aChildren) {
        subWidget->removeFromParent();
    }
    ui::ImageView *pRecvCard = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_PlayerPanel[cbViewID], utility::toString("RecvCard_", (int) cbViewID)));
    if (pRecvCard) {
        pRecvCard->setVisible(false);
    }
    switch (cbViewID) {
        case 0: {
            showAndUpdateHandCard();                     //更新手上的牌
            ui::Layout *pRecvCardList = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[cbViewID], utility::toString("RecvHandCard_0")));
            pRecvCardList->removeAllChildren(); //移除出牌位置的牌
            ui::Layout *pDiscardCard0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_0"));//显示出的牌
            pDiscardCard0->removeAllChildren();
            uint8_t bDiscardCount = m_cbDiscardCount[OutCard.cbOutCardUser]; //12
            float x = 0;
            float y = 0;
            for (int i = 0; i < bDiscardCount; i++) {
                uint8_t col = static_cast<uint8_t>(i % 12);
                uint8_t row = static_cast<uint8_t>(i / 12);
                x = (col == 0) ? 0 : x;  //X复位
                y = row * 90;
                ui::ImageView *pCard0 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[OutCard.cbOutCardUser][i]);
                pCard0->setAnchorPoint(Vec2(0, 0));
                pCard0->setPosition(Vec2(x, y));
                pCard0->setLocalZOrder(10 - row);
                x += 76;
                if (i == bDiscardCount - 1) {   //最后一张，添加动画效果
                    pSignNode->setAnchorPoint(Vec2(0.5, 0.5));
                    pSignNode->setPosition(Vec2(39, 110));
                    pCard0->addChild(pSignNode);
                    pSignNode->runAction(action);
                }
                pDiscardCard0->addChild(pCard0);
            }
            pDiscardCard0->setScale(0.7, 0.7);
            //听牌判断
            uint8_t cbWeaveItemCount = m_cbWeaveItemCount[OutCard.cbOutCardUser];
            tagWeaveItem *pTagWeaveItem = m_WeaveItemArray[OutCard.cbOutCardUser];
            uint8_t *cbCardIndex = m_cbCardIndex[OutCard.cbOutCardUser];
            showTingResult(cbCardIndex, pTagWeaveItem, cbWeaveItemCount);
            break;
        }
        case 1: {
            //显示出的牌
            ui::Layout *pDiscardCard1 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_1"));
            pDiscardCard1->removeAllChildren();
            uint8_t bDiscardCount = m_cbDiscardCount[OutCard.cbOutCardUser]; //
            float x = 0;
            float y = 0;
            for (int i = 0; i < bDiscardCount; i++) {
                uint8_t col = static_cast<uint8_t>(i % 11);
                uint8_t row = static_cast<uint8_t>(i / 11);
                y = (col == 0) ? 0 : y;  //X复位
                x = 116 * row;
                ui::ImageView *pCard1 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[OutCard.cbOutCardUser][i]);
                pCard1->setAnchorPoint(Vec2(0, 0));
                pCard1->setPosition(Vec2(x, 740 - y));
                pCard1->setLocalZOrder(col);
                y += 74;
                if (i == bDiscardCount - 1) {   //最后一张，添加动画效果
                    pSignNode->setAnchorPoint(Vec2(0.5, 0.5));
                    pSignNode->setPosition(Vec2(81, 110));
                    pCard1->addChild(pSignNode);
                    pSignNode->runAction(action);
                }
                pDiscardCard1->addChild(pCard1);
            }
            pDiscardCard1->setScale(0.5, 0.5);
            break;
        }
        case 2: {
            ui::Layout *pDiscardCard2 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_2"));
            pDiscardCard2->removeAllChildren();
            uint8_t bDiscardCount = m_cbDiscardCount[OutCard.cbOutCardUser]; //12
            float x = 0;
            float y = 0;
            for (int i = 0; i < bDiscardCount; i++) {
                uint8_t col = static_cast<uint8_t>(i % 12);
                uint8_t row = static_cast<uint8_t>(i / 12);
                x = (col == 0) ? 0 : x;  //X复位
                y = 90 - row * 90;
                ui::ImageView *pCard2 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[OutCard.cbOutCardUser][i]);
                pCard2->setAnchorPoint(Vec2(0, 0));
                pCard2->setPosition(Vec2(x, y));
                pCard2->setLocalZOrder(row);
                x += 76;
                if (i == bDiscardCount - 1) {   //最后一张，添加动画效果
                    pSignNode->setAnchorPoint(Vec2(0.5, 0.5));
                    pSignNode->setPosition(Vec2(39, 59));
                    pCard2->addChild(pSignNode);
                    pSignNode->runAction(action);
                }
                pDiscardCard2->addChild(pCard2);
            }
            pDiscardCard2->setScale(0.7, 0.7);

            break;
        }
        case 3: {
            //显示出的牌
            ui::Layout *pDiscardCard3 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_3"));
            pDiscardCard3->removeAllChildren();
            uint8_t bDiscardCount = m_cbDiscardCount[OutCard.cbOutCardUser]; //
            float x = 0;
            float y = 0;
            for (int i = 0; i < bDiscardCount; i++) {
                uint8_t col = static_cast<uint8_t>(i % 11);
                uint8_t row = static_cast<uint8_t>(i / 11);
                y = (col == 0) ? 0 : y;  //X复位
                x = 240 - (116 * row);
                ui::ImageView *pCard3 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[OutCard.cbOutCardUser][i]);
                pCard3->setAnchorPoint(Vec2(0, 0));
                pCard3->setPosition(Vec2(x, y));
                pCard3->setLocalZOrder(20 - col);
                y += 74;
                if (i == bDiscardCount - 1) {   //最后一张，添加动画效果
                    pSignNode->setAnchorPoint(Vec2(0.5, 0.5));
                    pSignNode->setPosition(Vec2(39, 110));
                    pCard3->addChild(pSignNode);
                    pSignNode->runAction(action);
                }
                pDiscardCard3->addChild(pCard3);
            }
            pDiscardCard3->setScale(0.5, 0.5);
            break;
        }
        default:
            break;
    }
    for (int j = 0; j < GAME_PLAYER; ++j) {   //发牌后隐藏导航
        ui::ImageView *pHighlight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, utility::toString("Image_Wheel_", j)));
        pHighlight->setVisible(false);
    }
    playSound(utility::toString("raw/Mahjong/", (IPlayer::FEMALE == m_Players[OutCard.cbOutCardUser]->getSex() ? "female" : "male"), "/mjt", utility::toString((
            (OutCard.cbOutCardData & MASK_COLOR)
                    >> 4) + 1, "_", OutCard.cbOutCardData & MASK_VALUE), ".mp3"));    //播放牌的声音
    return true;
}


/**
 * 操作提示
 * @param OperateNotify
 * @return
 */
bool GameLayer::onOperateNotifyEvent(CMD_S_OperateNotify OperateNotify) {
    return showOperateNotify(OperateNotify);
}

/**
 * 操作结果事件
 * @param OperateResult
 * @return
 */
bool GameLayer::onOperateResultEvent(CMD_S_OperateResult OperateResult) {
    //更新数据
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
            if (OperateResult.cbOperateUser == m_MeChairID) {  //自己
                uint8_t cbReomveCard[] = {OperateResult.cbOperateCard, OperateResult.cbOperateCard};
                m_GameLogic->removeCard(m_cbCardIndex[OperateResult.cbOperateUser], cbReomveCard, sizeof(cbReomveCard));
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

    //更新界面
    uint8_t cbViewID = switchViewChairID(OperateResult.cbOperateUser);
    switch (cbViewID) {
        case 0: {   //自己操作反馈
            m_pOperateNotifyGroup->removeAllChildren();
            m_pOperateNotifyGroup->setVisible(false);
            ui::ImageView *pHighlight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, "Image_Wheel_0"));
            pHighlight->setVisible(true);
            break;
        }
        default:
            break;
    }
    std::string strSex = (IPlayer::FEMALE == m_Players[OperateResult.cbOperateUser]->getSex() ? "female" : "male");
    switch (OperateResult.cbOperateCode) {
        case WIK_NULL: {
            break;
        }
        case WIK_P: {
            playSound(utility::toString("raw/Mahjong/", strSex, "/peng.mp3"));
            if (cbViewID == 0) {                    //如果是自己，碰完需要出牌
                uint8_t bTempCardData[MAX_COUNT];   //手上的牌
                memset(bTempCardData, 0, sizeof(bTempCardData));
                m_GameLogic->switchToCardData(m_cbCardIndex[OperateResult.cbOperateUser], bTempCardData, MAX_COUNT);
                uint8_t cbWeaveItemCount = m_cbWeaveItemCount[OperateResult.cbOperateUser]; //组合数量
                CMD_S_SendCard SendCard;
                memset(&SendCard, 0, sizeof(CMD_S_SendCard));
                SendCard.cbCurrentUser = OperateResult.cbOperateUser;
                SendCard.cbCardData = bTempCardData[MAX_COUNT - (cbWeaveItemCount * 3) - 1];
                showSendCard(SendCard);    //模拟发送一张牌
            }
            //移除桌上的哪张牌
            m_cbDiscardCount[OperateResult.cbProvideUser]--;  //移除桌上的牌
            showAndUpdateDiscardCard();
            break;
        }
        case WIK_G: {
            playSound(utility::toString("raw/Mahjong/", strSex, "/gang.mp3"));
            if (OperateResult.cbProvideUser != OperateResult.cbOperateUser) {     //放的杠
                m_cbDiscardCount[OperateResult.cbProvideUser]--;  //移除桌上的牌
                showAndUpdateDiscardCard();
            } else {                                                            //移除出牌位置的牌
                ui::Layout *pRecvCardList = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[0], utility::toString("RecvHandCard_0")));
                pRecvCardList->removeAllChildren();
            }
            break;
        }
        case WIK_H: { //胡牌动作放在游戏结束 信息中
            if (OperateResult.cbOperateUser == OperateResult.cbProvideUser) {  //自摸
                playSound(utility::toString("raw/Mahjong/", strSex, "/zimo.mp3"));
            } else {
                playSound(utility::toString("raw/Mahjong/", strSex, "/hu.mp3"));
            }
            break;
        }
        default:
            break;
    }
    showAndPlayOperateEffect(cbViewID, OperateResult.cbOperateCode, OperateResult.cbProvideUser == OperateResult.cbOperateUser);
    showAndUpdateHandCard();
    return true;
}

/**
 * 游戏结束事件
 * @param GameEnd
 * @return
 */
bool GameLayer::onGameEndEvent(CMD_S_GameEnd GameEnd) {
    showAndUpdateUserScore(GameEnd.lGameScoreTable);    //更新分数
    for (uint8_t i = 0; i < GAME_PLAYER; i++) {                    //播放音效
        if (FvMask::HasAny(GameEnd.cbHuUser, _MASK_(i))) {
            std::string strSex = (IPlayer::FEMALE == m_Players[i]->getSex() ? "female" : "male");
            uint8_t cbViewID = switchViewChairID(i);
            if (FvMask::HasAny(GameEnd.cbHuUser, _MASK_(GameEnd.cbProvideUser))) {  //自摸
                playSound(utility::toString("raw/Mahjong/", strSex, "/zimo.mp3"));
                showAndPlayOperateEffect(cbViewID, WIK_H, true); //播放自摸动画
            } else {
                playSound(utility::toString("raw/Mahjong/", strSex, "/hu.mp3"));
                showAndPlayOperateEffect(cbViewID, WIK_H, false); //播放自摸动画
            }
        }
    }

    Node *pTingNode = UIHelper::seekNodeByName(m_PlayerPanel[0], "ting_0");    //隐藏听牌
    pTingNode->setVisible(false);
    m_pOperateNotifyGroup->removeAllChildren();
    m_pOperateNotifyGroup->setVisible(false);             //移除通知

    //显示结算界面
    m_pGameOverNode = CSLoader::createNode("GameOverLayer.csb");
    m_pGameOverNode->setAnchorPoint(Vec2(0.5, 0.5));
    m_pGameOverNode->setPosition(GameSceneManager::getInstance()->getVisibleSize() / 2);
    m_pLayer->addChild(m_pGameOverNode);
    ui::Button *pOverClose = dynamic_cast<ui::Button *>(UIHelper::seekNodeByName(m_pGameOverNode, "Button_Over_Close"));    //关闭结算界面按钮
    pOverClose->addTouchEventListener(CC_CALLBACK_2(GameLayer::onTouch, this));
    ui::ImageView *pOverResultImage = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pGameOverNode, "ImageView_Over_Result"));
    pOverResultImage->loadTexture((GameEnd.lGameScore[m_MeChairID] < 0) ? "res/GameOverLayer/result_lose.png" : "res/GameOverLayer/result_win.png");
    if (GameEnd.cbHuUser == 0x00) {
        pOverResultImage->loadTexture("res/GameOverLayer/result_draw.png");  //流局
    }
    //显示牌
    for (uint8_t i = 0; i < m_CurPlayer; i++) {
        uint8_t cbViewID = switchViewChairID(i);
        Node *pPlayerViewNode = UIHelper::seekNodeByName(m_pGameOverNode, "PlayerView_" + utility::toString((int) cbViewID));
        ui::ImageView *pOverImgHead = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pPlayerViewNode, "ImageView_Over_Head")); //头像
        ui::Text *pOverScoreText = dynamic_cast<ui::Text *>(UIHelper::seekNodeByName(pPlayerViewNode, "Text_Over_Score"));       //分数
        pOverImgHead->loadTexture(utility::toString("res/GameLayer/im_defaulthead_", m_Players[i]->getSex() == IPlayer::FEMALE ? 0 : 1, ".png"));    //设置头像
        pOverScoreText->setString(((GameEnd.lGameScore[i] >= 0) ? utility::toString("+", GameEnd.lGameScore[i]) : utility::toString(GameEnd.lGameScore[i])));
        ui::Layout *pOverHandCard = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(pPlayerViewNode, "Panel_Over_Hand_Card"));
        pOverHandCard->removeAllChildren();
        ui::ImageView *pOverHuCard = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pPlayerViewNode, "ImageView_Over_Card"));  //胡的那张
        ui::ImageView *pOverHuFlag = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pPlayerViewNode, "ImageView_Over_Hu"));  //胡的那张
        pOverHuCard->setVisible(false);
        pOverHuFlag->setVisible(false);
        if (m_cbBankerChair == i) { //庄
            ui::Layout *pOverPanelLeft = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(pPlayerViewNode, "Panel_Over_Left"));
            ui::ImageView *pOverBanker = ui::ImageView::create("res/GameOverLayer/over_banker.png");
            pOverBanker->setAnchorPoint(Vec2(0.5, 0.5));
            pOverBanker->setPosition(Vec2(20, 96));
            pOverPanelLeft->addChild(pOverBanker);
        }

        uint8_t cbWeaveCount = GameEnd.cbWeaveCount[i];
        if (FvMask::HasAny(GameEnd.cbHuUser, _MASK_(i))) {
            //胡牌手上移除胡的那张牌
            m_GameLogic->removeCard(GameEnd.cbCardData[i], GameEnd.cbCardCount[i], &GameEnd.cbHuCard, 1);
            GameEnd.cbCardCount[i]--;
            pOverHuCard->loadTexture(getDiscardCardImagePath(0, GameEnd.cbHuCard));
            pOverHuCard->setVisible(true);
            pOverHuFlag->setVisible(true);
        }
        float x = 0;
        for (uint8_t j = 0; j < cbWeaveCount; j++) {   //组合数量
            CMD_WeaveItem weaveItem = GameEnd.WeaveItemArray[i][j];
            Node *pWeaveNode = NULL;
            if (weaveItem.cbWeaveKind == WIK_G) {
                pWeaveNode = CSLoader::createNode("res/Gang0.csb");
            }
            if (weaveItem.cbWeaveKind == WIK_P) {
                pWeaveNode = CSLoader::createNode("res/Peng0.csb");
            }
            pWeaveNode->setScale(0.6f, 0.6f);
            pWeaveNode->setPosition(Vec2(x, 0));
            const std::string &strImagePath = getDiscardCardImagePath(0, weaveItem.cbCenterCard);
            const std::string &strBackImagePath = getDiscardCardImagePath(0, weaveItem.cbCenterCard);
            ui::ImageView *pImageRight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_right"));
            ui::ImageView *pImageLeft = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_left"));
            ui::ImageView *pImageCenter = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_center"));
            pImageRight->loadTexture(strImagePath);
            pImageCenter->loadTexture(strImagePath);
            pImageLeft->loadTexture(strImagePath);
            uint8_t wProvideViewID = switchViewChairID(weaveItem.cbProvideUser);
            switch (cbViewID) {
                case 0:
                    switch (wProvideViewID) {
                        case 0: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 1: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                case 1:
                    switch (wProvideViewID) {
                        case 0: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 2: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                case 2:
                    switch (wProvideViewID) {
                        case 0: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 3: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                case 3:
                    switch (wProvideViewID) {
                        case 0: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

            pOverHandCard->addChild(pWeaveNode);
            x += 132;
        }
        for (int j = 0; j < MAX_COUNT - 1 - (3 * cbWeaveCount); j++) {
            ui::ImageView *pCard = createDiscardCardImageView(0, GameEnd.cbCardData[i][j]);
            pCard->setAnchorPoint(Vec2(0, 0));
            pCard->setPosition(Vec2(x, 0));
            pCard->setScale(0.6f, 0.6f);
            pOverHandCard->addChild(pCard);
            x += 43;
        }
    }

    return true;
}

void GameLayer::sendCardTimerUpdate(float) {
    m_iOutCardTimeOut = static_cast<uint8_t>((m_iOutCardTimeOut-- < 1) ? 0 : m_iOutCardTimeOut);
    ui::ImageView *pTimer1 = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, "Image_Timer_1"));
    ui::ImageView *pTimer0 = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, "Image_Timer_0"));
    int t = m_iOutCardTimeOut / 10;   //十位
    int g = m_iOutCardTimeOut % 10;   //各位
    pTimer1->loadTexture(utility::toString("res/GameLayer/timer_", g, ".png"));
    pTimer0->loadTexture(utility::toString("res/GameLayer/timer_", t, ".png"));
}


/**
 * 发牌显示
 * @param SendCard
 * @return
 */
bool GameLayer::showSendCard(CMD_S_SendCard SendCard) {
    m_pOperateNotifyGroup->removeAllChildren();
    m_pOperateNotifyGroup->setVisible(true);
    this->unschedule(schedule_selector(GameLayer::sendCardTimerUpdate));
    this->schedule(schedule_selector(GameLayer::sendCardTimerUpdate), 1.0f);    //出牌计时
    sendCardTimerUpdate(1.0f);//立即执行
    m_pTextCardNum->setString(utility::toString((int) m_cbLeftCardCount));
    uint8_t cbViewID = switchViewChairID(SendCard.cbCurrentUser);
    m_bOperate = false;
    switch (cbViewID) {
        case 0: {
            m_bOperate = true;   //允许选牌
            ui::Layout *pRecvCardList = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[cbViewID], utility::toString("RecvHandCard_0")));
            pRecvCardList->removeAllChildren();
            ui::ImageView *pCard = createHandCardImageView(cbViewID, SendCard.cbCardData);
            pCard->setAnchorPoint(Vec2(0, 0));
            pCard->setPosition(Vec2(0, 0));
            pCard->setTouchEnabled(true);
            pCard->setTag(SendCard.cbCardData);
            pCard->setName(utility::toString("bt_card_", (int) SendCard.cbCardData));
            pCard->addTouchEventListener(CC_CALLBACK_2(GameLayer::onCardTouch, this));
            pRecvCardList->addChild(pCard);
            UIHelper::seekNodeByName(m_PlayerPanel[cbViewID], "ting_0")->setVisible(false); //拿到牌，隐藏听牌界面
            if (SendCard.cbActionMask != WIK_NULL) {//发的牌存在动作，模拟发送操作通知
                CMD_S_OperateNotify OperateNotify;
                memset(&OperateNotify, 0, sizeof(CMD_S_OperateNotify));
                OperateNotify.cbActionMask = SendCard.cbActionMask;
                OperateNotify.cbActionCard = SendCard.cbCardData;
                OperateNotify.cbGangCount = SendCard.cbGangCount;
                memcpy(OperateNotify.cbGangCard, SendCard.cbGangCard, sizeof(OperateNotify.cbGangCard));
                showOperateNotify(OperateNotify);
            }
            break;
        }
        case 1:
        case 2:
        case 3:
        default:
            m_bOperate = false;  //不允许操作
            break;
    }
    for (int i = 0; i < GAME_PLAYER; i++) {
        ui::ImageView *pRecvCard = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_PlayerPanel[cbViewID], utility::toString("RecvCard_", i)));
        if (pRecvCard) {
            pRecvCard->setVisible(cbViewID == i);
        }
        ui::ImageView *pHighlight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, utility::toString("Image_Wheel_", i)));
        if (pHighlight) {
            pHighlight->setVisible(cbViewID == i);
        }
    }
    return true;
}


bool GameLayer::showTingResult(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount) {
    tagTingResult tingResult;
    memset(&tingResult, 0, sizeof(tagTingResult));
    Node *pTingNode = UIHelper::seekNodeByName(m_PlayerPanel[m_MeChairID], "ting_0");
    if (m_GameLogic->analyseTingCardResult(cbCardIndex, WeaveItem, cbWeaveCount, tingResult)) {  //听牌
        pTingNode->setVisible(true);
        Node *pTingCard = UIHelper::seekNodeByName(m_PlayerPanel[m_MeChairID], "ting_card");
        pTingCard->removeAllChildren();
        for (int i = 0; i < tingResult.cbTingCount; i++) {  //循环听的牌
            ui::ImageView *pTingCardImage = createDiscardCardImageView(0, tingResult.cbTingCard[i]);
            pTingCardImage->setAnchorPoint(Vec2(0, 0));
            pTingCardImage->setPosition(Vec2(76 * i + ((76.0f * 3) - (76.0f * tingResult.cbTingCount)) / 2.0f, 0));
            pTingCard->addChild(pTingCardImage);
        }
    } else {
        pTingNode->setVisible(false);
    }
    return true;
}

bool GameLayer::showAndUpdateUserScore(int64_t lGameScoreTable[GAME_PLAYER]) {
    for (uint8_t i = 0; i < GAME_PLAYER; i++) {
        uint8_t cbViewID = switchViewChairID(i);
        ui::Text *pScore = dynamic_cast<ui::Text *>(UIHelper::seekNodeByName(m_FaceFrame[cbViewID], "Gold_Label"));
        if (pScore) {
            pScore->setString(utility::toString(lGameScoreTable[i]));
        }
    }
    return true;
}

/**
 * 展示操作层
 * @param OperateNotify
 * @return
 */
bool GameLayer::showOperateNotify(CMD_S_OperateNotify OperateNotify) {
    if (OperateNotify.cbActionMask == WIK_NULL) {
        return true; //无动作
    }
    m_pOperateNotifyGroup->removeAllChildren();
    m_pOperateNotifyGroup->setVisible(true);
    float x = 500.0f;
    float y = 65.0f;
    if ((OperateNotify.cbActionMask & WIK_H) != 0) {
        Node *pHuNode = CSLoader::createNode("res/BtnHu.csb");
        pHuNode->setPosition(Vec2(x, y));
        ui::Button *pHuBtn = dynamic_cast<ui::Button *>(pHuNode->getChildren().at(0));
        pHuBtn->setTag(OperateNotify.cbActionCard);
        pHuBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::onOperateTouch, this));
        m_pOperateNotifyGroup->addChild(pHuNode);
        x -= 160;
    }
    if ((OperateNotify.cbActionMask & WIK_G) != 0) {
        for (int i = 0; i < OperateNotify.cbGangCount; i++) {
            Node *pGangNode = CSLoader::createNode("res/BtnGang.csb");
            pGangNode->setPosition(Vec2(x, y + (i * 120)));
            ui::Button *pGangBtn = dynamic_cast<ui::Button *>(pGangNode->getChildren().at(0));
            ui::ImageView *pImgGangCard = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pGangBtn, "ImgGangCard"));
            pImgGangCard->loadTexture(getDiscardCardImagePath(0, OperateNotify.cbGangCard[i]));
            pImgGangCard->setVisible(OperateNotify.cbGangCount > 1);
            pGangBtn->setTag(OperateNotify.cbGangCard[i]);
            pGangBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::onOperateTouch, this));
            m_pOperateNotifyGroup->addChild(pGangNode);
        }
        x -= 160;
    }
    if ((OperateNotify.cbActionMask & WIK_P) != 0) {
        Node *pPengNode = CSLoader::createNode("res/BtnPeng.csb");
        pPengNode->setPosition(Vec2(x, y));
        ui::Button *pPengBtn = dynamic_cast<ui::Button *>(pPengNode->getChildren().at(0));
        pPengBtn->setTag(OperateNotify.cbActionCard);
        pPengBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::onOperateTouch, this));
        m_pOperateNotifyGroup->addChild(pPengNode);
        x -= 160;
    }
    Node *pGuoNode = CSLoader::createNode("res/BtnGuo.csb");
    pGuoNode->setPosition(Vec2(x, y));
    ui::Button *pGuoBtn = dynamic_cast<ui::Button *>(pGuoNode->getChildren().at(0));
    pGuoBtn->setTag(OperateNotify.cbActionCard);
    pGuoBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::onOperateTouch, this));
    m_pOperateNotifyGroup->addChild(pGuoNode);
    return true;
}

/**
 * 显示手上的牌
 * @return
 */
bool GameLayer::showAndUpdateHandCard() {
    for (uint8_t i = 0; i < m_CurPlayer; i++) {
        uint8_t viewChairID = switchViewChairID(i);
        uint8_t bCardData[MAX_COUNT];  //手上的牌
        memset(bCardData, 0, sizeof(bCardData));
        m_GameLogic->switchToCardData(m_cbCardIndex[i], bCardData, MAX_COUNT);
        uint8_t cbWeaveItemCount = m_cbWeaveItemCount[i]; //组合数量
        tagWeaveItem WeaveItemArray[MAX_WEAVE];         //组合
        memcpy(WeaveItemArray, m_WeaveItemArray[i], sizeof(WeaveItemArray));
        switch (viewChairID) {
            case 0: {
                ui::Layout *pHandCard0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "HandCard_0"));
                ui::Layout *pComb0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "comb_0"));  //组合
                pHandCard0->removeAllChildren();
                pComb0->removeAllChildren();
                int x = 0;  //宽起始位置
                for (uint8_t j = 0; j < cbWeaveItemCount; j++) {
                    tagWeaveItem weaveItem = WeaveItemArray[j];
                    Node *pWeaveNode = NULL;
                    if (weaveItem.cbWeaveKind == WIK_G) {
                        pWeaveNode = CSLoader::createNode("res/Gang0.csb");
                    }
                    if (weaveItem.cbWeaveKind == WIK_P) {
                        pWeaveNode = CSLoader::createNode("res/Peng0.csb");
                    }
                    assert(pWeaveNode != NULL);
                    pWeaveNode->setPosition(Vec2(x, 0));
                    const std::string &strImagePath = getDiscardCardImagePath(0, weaveItem.cbCenterCard);
                    const std::string &strBackImagePath = getBackCardImagePath(0, weaveItem.cbCenterCard);
                    ui::ImageView *pImageRight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_right"));
                    ui::ImageView *pImageLeft = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_left"));
                    ui::ImageView *pImageCenter = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_center"));
                    pImageRight->loadTexture(strImagePath);
                    pImageCenter->loadTexture(strImagePath);
                    pImageLeft->loadTexture(strImagePath);
                    switch (switchViewChairID(weaveItem.cbProvideUser)) {
                        case 0: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageCenter->loadTexture(strImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 1: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    pComb0->addChild(pWeaveNode);
                    x += 228;
                }
                for (uint8_t j = 0; j < MAX_COUNT - 1 - (3 * cbWeaveItemCount); j++) {
                    ui::ImageView *pCard = createHandCardImageView(viewChairID, bCardData[j]);
                    pCard->setAnchorPoint(Vec2(0, 0));
                    pCard->setPosition(Vec2(x, 0));
                    pCard->setTouchEnabled(true);
                    pCard->setTag(bCardData[j]);
                    pCard->setName(utility::toString("bt_card_", (int) bCardData[j]));
                    pCard->addTouchEventListener(CC_CALLBACK_2(GameLayer::onCardTouch, this));
                    pHandCard0->addChild(pCard);
                    x += 76;
                }
                break;
            }
            case 1: {
                ui::Layout *pHandCard1 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "HandCard_1"));
                ui::Layout *pComb1 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "comb_1"));  //组合
                pHandCard1->removeAllChildren();
                pComb1->removeAllChildren();
                int y = 800;  //高起始位置
                for (uint8_t j = 0; j < cbWeaveItemCount; j++) {
                    y -= 160;
                    tagWeaveItem weaveItem = WeaveItemArray[j];
                    Node *pWeaveNode = NULL;
                    if (weaveItem.cbWeaveKind == WIK_G) {
                        pWeaveNode = CSLoader::createNode("res/Gang1.csb");
                    }
                    if (weaveItem.cbWeaveKind == WIK_P) {
                        pWeaveNode = CSLoader::createNode("res/Peng1.csb");
                    }
                    assert(pWeaveNode != NULL);
                    pWeaveNode->setPosition(Vec2(0, y + 20));
                    const std::string &strImagePath = getDiscardCardImagePath(1, weaveItem.cbCenterCard);
                    const std::string &strBackImagePath = getBackCardImagePath(1, weaveItem.cbCenterCard);
                    ui::ImageView *pImageRight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_right"));
                    ui::ImageView *pImageLeft = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_left"));
                    ui::ImageView *pImageCenter = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_center"));
                    pImageRight->loadTexture(strImagePath);
                    pImageCenter->loadTexture(strImagePath);
                    pImageLeft->loadTexture(strImagePath);
                    switch (switchViewChairID(weaveItem.cbProvideUser)) {
                        case 0: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageCenter->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 2: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    pComb1->addChild(pWeaveNode);
                }
                for (uint8_t j = 0; j < MAX_COUNT - 1 - (3 * cbWeaveItemCount); j++) {
                    y -= 60;
                    ui::ImageView *pCard = createHandCardImageView(viewChairID, bCardData[j]);
                    pCard->setAnchorPoint(Vec2(0, 0));
                    pCard->setPosition(Vec2(0, y - 20));
                    pCard->setLocalZOrder(j);
                    pHandCard1->addChild(pCard);
                }
                break;
            }
            case 2: {
                ui::Layout *pHandCard2 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "HandCard_2"));
                ui::Layout *pComb2 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "comb_2"));  //组合
                pHandCard2->removeAllChildren();
                pComb2->removeAllChildren();
                int x = 1027;  //宽起始位置
                for (uint8_t j = 0; j < cbWeaveItemCount; j++) {
                    x -= 228;
                    tagWeaveItem weaveItem = WeaveItemArray[j];
                    Node *pWeaveNode = NULL;
                    if (weaveItem.cbWeaveKind == WIK_G) {
                        pWeaveNode = CSLoader::createNode("res/Gang0.csb");
                    }
                    if (weaveItem.cbWeaveKind == WIK_P) {
                        pWeaveNode = CSLoader::createNode("res/Peng0.csb");
                    }
                    assert(pWeaveNode != NULL);
                    pWeaveNode->setPosition(Vec2(x + 23, 0));
                    const std::string &strImagePath = getDiscardCardImagePath(2, weaveItem.cbCenterCard);
                    const std::string &strBackImagePath = getBackCardImagePath(2, weaveItem.cbCenterCard);
                    ui::ImageView *pImageRight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_right"));
                    ui::ImageView *pImageLeft = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_left"));
                    ui::ImageView *pImageCenter = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_center"));
                    pImageRight->loadTexture(strImagePath);
                    pImageCenter->loadTexture(strImagePath);
                    pImageLeft->loadTexture(strImagePath);
                    switch (switchViewChairID(weaveItem.cbProvideUser)) {
                        case 0: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageCenter->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        case 3: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        default:
                            break;
                    }
                    pComb2->addChild(pWeaveNode);
                }
                for (uint8_t j = 0; j < MAX_COUNT - 1 - (3 * cbWeaveItemCount); j++) {
                    x -= 76;
                    ui::ImageView *pCard = createHandCardImageView(viewChairID, bCardData[j]);
                    pCard->setAnchorPoint(Vec2(0, 0));
                    pCard->setPosition(Vec2(x, 0));
                    pHandCard2->addChild(pCard);
                }
                break;
            }
            case 3: {
                ui::Layout *pHandCard3 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "HandCard_3"));
                ui::Layout *pComb3 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[viewChairID], "comb_3"));  //组合
                pHandCard3->removeAllChildren();
                pComb3->removeAllChildren();
                int y = 0;  //高起始位置
                for (uint8_t j = 0; j < cbWeaveItemCount; j++) {
                    tagWeaveItem weaveItem = WeaveItemArray[j];
                    Node *pWeaveNode = NULL;
                    if (weaveItem.cbWeaveKind == WIK_G) {
                        pWeaveNode = CSLoader::createNode("res/Gang1.csb");
                    }
                    if (weaveItem.cbWeaveKind == WIK_P) {
                        pWeaveNode = CSLoader::createNode("res/Peng1.csb");
                    }
                    assert(pWeaveNode != NULL);
                    pWeaveNode->setPosition(Vec2(0, y - 10));
                    const std::string &strImagePath = getDiscardCardImagePath(3, weaveItem.cbCenterCard);
                    const std::string &strBackImagePath = getBackCardImagePath(3, weaveItem.cbCenterCard);
                    ui::ImageView *pImageRight = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_right"));
                    ui::ImageView *pImageLeft = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_left"));
                    ui::ImageView *pImageCenter = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_center"));
                    pImageRight->loadTexture(strImagePath);
                    pImageCenter->loadTexture(strImagePath);
                    pImageLeft->loadTexture(strImagePath);
                    switch (switchViewChairID(weaveItem.cbProvideUser)) {
                        case 0: {
                            pImageRight->loadTexture(strBackImagePath);
                            break;
                        }
                        case 1: {
                            pImageCenter->loadTexture(strBackImagePath);
                            break;
                        }
                        case 2: {
                            pImageLeft->loadTexture(strBackImagePath);
                            break;
                        }
                        case 3: {
                            if (weaveItem.cbPublicCard == FALSE) {    //暗杠
                                pImageRight->loadTexture(strBackImagePath);
                                pImageCenter->loadTexture(strBackImagePath);
                                pImageLeft->loadTexture(strBackImagePath);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    pComb3->addChild(pWeaveNode);
                    y += 160;
                }
                for (uint8_t j = 0; j < MAX_COUNT - 1 - (3 * cbWeaveItemCount); j++) {
                    ui::ImageView *pCard = createHandCardImageView(viewChairID, bCardData[j]);
                    pCard->setAnchorPoint(Vec2(0, 0));
                    pCard->setPosition(Vec2(0, y - 20));
                    pCard->setLocalZOrder(MAX_COUNT - j);
                    pHandCard3->addChild(pCard);
                    y += 60;
                }
                break;
            }
            default:
                break;
        }
    }
    return true;
}

/**
 * 播放特效
 * @param cbViewID
 * @param cbOperateCode
 * @param bZm
 * @return
 */
bool GameLayer::showAndPlayOperateEffect(uint8_t cbViewID, uint8_t cbOperateCode, bool bZm) {

    float x = 0;
    float y = 0;
    switch (cbViewID) {
        case 0: {   //自己操作反馈
            x = 640.0f;
            y = 200.0f;
            break;
        }
        case 1: {
            x = 440.0f;
            y = 360.0f;
            break;
        }
        case 2: {
            x = 640.0f;
            y = 520.0f;
            break;
        }
        case 3: {
            x = 840.0f;
            y = 360.0f;
            break;
        }
        default:
            break;
    }
    std::string strEffect = "";
    switch (cbOperateCode) {
        case WIK_NULL: {
            break;
        }
        case WIK_P: {
            strEffect = "res/EffectPeng.csb";
            break;
        }
        case WIK_G: {
            strEffect = "res/EffectGang.csb";
            break;
        }
        case WIK_H: {
            strEffect = bZm ? "res/EffectZm.csb" : "res/EffectHu.csb";
            break;
        }
        default:
            break;
    }
    if (strEffect != "") {
        std::string strNodeName = "EffectNode";
        Node *pEffectNode = CSLoader::createNode(strEffect);
        pEffectNode->setPosition(Vec2(x, y));
        cocostudio::timeline::ActionTimeline *action = CSLoader::createTimeline(strEffect);
        action->gotoFrameAndPlay(0, false);
        pEffectNode->setName(strNodeName);
        m_pLayer->addChild(pEffectNode);
        if (cbOperateCode != WIK_H) {    //胡牌不自动删除动画
            action->setLastFrameCallFunc(CC_CALLBACK_0(GameLayer::removeEffectNode, this, strNodeName));
        }
        pEffectNode->runAction(action);
    }
    return true;
}

/**
 * 移除动画节点
 * @param strNodeName
 */
void GameLayer::removeEffectNode(std::string strNodeName) {
    std::vector<Node *> aChildren;
    aChildren.clear();
    UIHelper::getChildren(m_pLayer, strNodeName, aChildren);
    for (auto &subChild : aChildren) {
        subChild->removeFromParent();
    }
}

/**
 * 显示、更新桌上的牌
 * @return
 */
bool GameLayer::showAndUpdateDiscardCard() {
    for (uint8_t cbChairID = 0; cbChairID < m_CurPlayer; cbChairID++) {
        uint8_t cbViewID = switchViewChairID(cbChairID);
        switch (cbViewID) {
            case 0: {
                ui::Layout *pDiscardCard0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_0"));
                pDiscardCard0->removeAllChildren();
                uint8_t bDiscardCount = m_cbDiscardCount[cbChairID];
                float x = 0;
                float y = 0;
                for (uint8_t i = 0; i < bDiscardCount; i++) {
                    uint8_t col = static_cast<uint8_t>(i % 12);
                    uint8_t row = static_cast<uint8_t>(i / 12);
                    x = (col == 0) ? 0 : x;  //X复位
                    y = row * 90;
                    ui::ImageView *pCard0 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[cbChairID][i]);
                    pCard0->setAnchorPoint(Vec2(0, 0));
                    pCard0->setPosition(Vec2(x, y));
                    pCard0->setLocalZOrder(10 - row);
                    x += 76;
                    pDiscardCard0->addChild(pCard0);
                }
                pDiscardCard0->setScale(0.7, 0.7);
                break;
            }
            case 1: {
                //显示出的牌
                ui::Layout *pDiscardCard1 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_1"));
                pDiscardCard1->removeAllChildren();
                uint8_t bDiscardCount = m_cbDiscardCount[cbChairID];
                float x = 0;
                float y = 0;
                for (uint8_t i = 0; i < bDiscardCount; i++) {
                    uint8_t col = static_cast<uint8_t>(i % 11);
                    uint8_t row = static_cast<uint8_t>(i / 11);
                    y = (col == 0) ? 0 : y;  //X复位
                    x = 116 * row;
                    ui::ImageView *pCard1 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[cbChairID][i]);
                    pCard1->setAnchorPoint(Vec2(0, 0));
                    pCard1->setPosition(Vec2(x, 740 - y));
                    pCard1->setLocalZOrder(col);
                    y += 74;
                    pDiscardCard1->addChild(pCard1);
                }
                pDiscardCard1->setScale(0.5, 0.5);
                break;
            }
            case 2: {
                ui::Layout *pDiscardCard2 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_2"));
                pDiscardCard2->removeAllChildren();
                uint8_t bDiscardCount = m_cbDiscardCount[cbChairID];
                float x = 0;
                float y = 0;
                for (uint8_t i = 0; i < bDiscardCount; i++) {
                    uint8_t col = static_cast<uint8_t>(i % 12);
                    uint8_t row = static_cast<uint8_t>(i / 12);
                    x = (col == 0) ? 0 : x;  //X复位
                    y = 90 - row * 90;
                    ui::ImageView *pCard2 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[cbChairID][i]);
                    pCard2->setAnchorPoint(Vec2(0, 0));
                    pCard2->setPosition(Vec2(x, y));
                    pCard2->setLocalZOrder(row);
                    x += 76;
                    pDiscardCard2->addChild(pCard2);
                }
                pDiscardCard2->setScale(0.7, 0.7);

                break;
            }
            case 3: {
                //显示出的牌
                ui::Layout *pDiscardCard3 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_pLayer, "DiscardCard_3"));
                pDiscardCard3->removeAllChildren();
                uint8_t bDiscardCount = m_cbDiscardCount[cbChairID];
                float x = 0;
                float y = 0;
                for (uint8_t i = 0; i < bDiscardCount; i++) {
                    uint8_t col = static_cast<uint8_t>(i % 11);
                    uint8_t row = static_cast<uint8_t>(i / 11);
                    y = (col == 0) ? 0 : y;  //X复位
                    x = 240 - (116 * row);
                    ui::ImageView *pCard3 = createDiscardCardImageView(cbViewID, m_cbDiscardCard[cbChairID][i]);
                    pCard3->setAnchorPoint(Vec2(0, 0));
                    pCard3->setPosition(Vec2(x, y));
                    pCard3->setLocalZOrder(20 - col);
                    y += 74;
                    pDiscardCard3->addChild(pCard3);
                }
                pDiscardCard3->setScale(0.5, 0.5);
                break;
            }
            default:
                break;
        }
    }
    return true;
}

/**
 * 手上的牌
 * @param cbViewID
 * @param cbData
 * @return
 */
ui::ImageView *GameLayer::createHandCardImageView(uint8_t cbViewID, uint8_t cbData) {
    return ui::ImageView::create(getHandCardImagePath(cbViewID, cbData));
}

/**
 * 创建桌面的牌
 * @param cbViewID
 * @param cbData
 * @return
 */
ui::ImageView *GameLayer::createDiscardCardImageView(uint8_t cbViewID, uint8_t cbData) {
    return ui::ImageView::create(getDiscardCardImagePath(cbViewID, cbData));
}

/**
 * 桌面显示的牌
 * @param cbViewID
 * @param cbData
 * @return
 */
std::string GameLayer::getDiscardCardImagePath(uint8_t cbViewID, uint8_t cbData) {
    std::string strImagePath = "";
    switch (cbViewID) {
        case 0: {
            strImagePath = "res/GameLayer/Mahjong/2/mingmah_" + utility::toString(((cbData & MASK_COLOR)
                    >> 4) + 1, cbData & MASK_VALUE) + ".png";
            break;
        }
        case 1: {
            strImagePath = "res/GameLayer/Mahjong/3/mingmah_" + utility::toString(((cbData & MASK_COLOR)
                    >> 4) + 1, cbData & MASK_VALUE) + ".png";
            break;
        }
        case 2: {
            strImagePath = "res/GameLayer/Mahjong/2/mingmah_" + utility::toString(((cbData & MASK_COLOR)
                    >> 4) + 1, cbData & MASK_VALUE) + ".png";
            break;
        }
        case 3: {
            strImagePath = "res/GameLayer/Mahjong/1/mingmah_" + utility::toString(((cbData & MASK_COLOR)
                    >> 4) + 1, cbData & MASK_VALUE) + ".png";
            break;
        }
        default:
            break;
    }
    return strImagePath;
}

/**
 *
 * 背面
 * @param cbViewID
 * @param cbData
 * @return
 */
std::string GameLayer::getBackCardImagePath(uint8_t cbViewID, uint8_t) {
    std::string strImagePath = "";
    switch (cbViewID) {
        case 0: {
            strImagePath = "res/GameLayer/Mahjong/2/mingmah_00.png";
            break;
        }
        case 1: {
            strImagePath = "res/GameLayer/Mahjong/1/mingmah_00.png";
            break;
        }
        case 2: {
            strImagePath = "res/GameLayer/Mahjong/2/mingmah_00.png";
            break;
        }
        case 3: {
            strImagePath = "res/GameLayer/Mahjong/1/mingmah_00.png";
            break;
        }
        default:
            break;
    }
    return strImagePath;
}

/**
 * 手上的牌路径
 * @param cbViewID
 * @param cbData
 * @return
 */
std::string GameLayer::getHandCardImagePath(uint8_t cbViewID, uint8_t cbData) {
    std::string strImagePath = "";
    switch (cbViewID) {
        case 0: {
            strImagePath = "res/GameLayer/Mahjong/2/handmah_" + utility::toString(((cbData & MASK_COLOR)
                    >> 4) + 1, cbData & MASK_VALUE) + ".png";
            break;
        }
        case 1: {
            strImagePath = "res/GameLayer/Mahjong/hand_left.png";
            break;
        }
        case 2: {
            strImagePath = "res/GameLayer/Mahjong/hand_top.png";
            break;
        }
        case 3: {
            strImagePath = "res/GameLayer/Mahjong/hand_right.png";
            break;
        }
        default:
            break;
    }
    return strImagePath;
}

void GameLayer::onOperateTouch(Ref *ref, ui::Widget::TouchEventType eventType) {
    ui::Button *pRef = dynamic_cast<ui::Button *>(ref);
    if (pRef != NULL) {
        std::string btnName = pRef->getName();
        int iTag = pRef->getTag();
        switch (eventType) {
            case ui::Widget::TouchEventType::ENDED:
                CMD_C_OperateCard OperateCard;
                memset(&OperateCard, 0, sizeof(CMD_C_OperateCard));
                if (strcmp(btnName.c_str(), "btn_hu") == 0) {
                    OperateCard.cbOperateCode = WIK_H;
                    OperateCard.cbOperateCard = static_cast<uint8_t >(iTag);
                }
                if (strcmp(btnName.c_str(), "btn_gang") == 0) {
                    OperateCard.cbOperateCode = WIK_G;
                    OperateCard.cbOperateCard = static_cast<uint8_t>(iTag);
                }
                if (strcmp(btnName.c_str(), "btn_peng") == 0) {
                    OperateCard.cbOperateCode = WIK_P;
                    OperateCard.cbOperateCard = static_cast<uint8_t>(iTag);
                }
                if (strcmp(btnName.c_str(), "btn_guo") == 0) {
                    OperateCard.cbOperateCode = WIK_NULL;
                    OperateCard.cbOperateCard = static_cast<uint8_t>(iTag);
                    m_pOperateNotifyGroup->removeAllChildren();
                    m_pOperateNotifyGroup->setVisible(false);
                }
                OperateCard.cbOperateUser = m_MeChairID;
                m_GameEngine->onUserOperateCard(OperateCard);
                break;
            default:
                break;
        }
    }
}

/**
 * 出牌
 * @param ref
 * @param eventType
 */
void GameLayer::onCardTouch(Ref *ref, ui::Widget::TouchEventType eventType) {
    if (m_pOutCard != NULL && m_pOutCard != ref) return;
    if (m_bOperate) {
        ui::ImageView *pCard = dynamic_cast<ui::ImageView *>(ref);
        ui::Layout *pHandCard0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[0], "HandCard_0"));
        const auto &aChildren = pHandCard0->getChildren();
        for (auto &subWidget : aChildren) {
            Node *child = dynamic_cast<Node *>(subWidget);
            if (pCard != child) {
                child->setPositionY(0.0f);
            }
        }
        ui::Layout *pRecvHandCard0 = dynamic_cast<ui::Layout *>(UIHelper::seekNodeByName(m_PlayerPanel[0], "RecvHandCard_0"));
        const auto &bChildren = pRecvHandCard0->getChildren();
        for (auto &subWidget : bChildren) {
            Node *child = dynamic_cast<Node *>(subWidget);
            if (pCard != child) {
                child->setPositionY(0.0f);
            }
        }
        if (pCard != NULL) {
            std::string btCardName = pCard->getName();
            switch (eventType) {
                case ui::Widget::TouchEventType::BEGAN: {
                    m_pOutCard = dynamic_cast<Node *>(ref);         //正在出牌状态
                    m_bMove = false;                                //是否移动牌状态
                    m_startVec = pCard->getPosition();              //记录开始位置
                    //听牌判断
                    uint8_t cbCardData = static_cast<uint8_t>(pCard->getTag());   //牌
                    uint8_t cbWeaveItemCount = m_cbWeaveItemCount[m_MeChairID];
                    tagWeaveItem *pTagWeaveItem = m_WeaveItemArray[m_MeChairID];
                    uint8_t cbTingCard[MAX_INDEX];
                    memset(&cbTingCard, 0, sizeof(cbTingCard));
                    uint8_t *cbCardIndex = m_cbCardIndex[m_MeChairID];
                    memcpy(&cbTingCard, cbCardIndex, sizeof(cbTingCard));   //内存拷贝
                    cbTingCard[m_GameLogic->switchToCardIndex(cbCardData)]--;   //移除要出的牌进行分析
                    showTingResult(cbTingCard, pTagWeaveItem, cbWeaveItemCount);
                    break;
                }
                case ui::Widget::TouchEventType::MOVED: {
                    Vec2 a = pCard->getTouchBeganPosition();
                    Vec2 b = pCard->getTouchMovePosition();
                    pCard->setPosition(m_startVec + (b - a));       //移动
                    if (b.y - a.y > 60 || abs(b.x - a.x) > 30) {    //移动判定
                        m_bMove = true;
                    }
                    break;
                }
                case ui::Widget::TouchEventType::CANCELED:
                case ui::Widget::TouchEventType::ENDED: {
                    m_pOutCard = NULL;                             //结束出牌状态
                    Vec2 endVec = pCard->getPosition();
                    if (endVec.y - m_startVec.y > 118) {
                        CCLOG("out card");
                        CMD_C_OutCard OutCard;
                        memset(&OutCard, 0, sizeof(CMD_C_OutCard));
                        OutCard.cbCardData = static_cast<uint8_t >(pCard->getTag());
                        m_GameEngine->onUserOutCard(OutCard);
                    } else {
                        if (m_startVec.y == m_cardPosY) {                               //正常状态
                            if (m_bMove) {
                                pCard->setPosition(m_startVec);                         //移动
                            } else {
                                pCard->setPosition(m_startVec + Vec2(0, m_outY));       //移动
                            }
                        } else if (m_startVec.y == m_cardPosY + m_outY) {               //选牌状态
                            Vec2 a = pCard->getTouchBeganPosition();                    //触摸上半部分，撤销出牌
                            if (a.y > 118) {
                                pCard->setPosition(m_startVec - Vec2(0, m_outY));
                            } else {                                                   //触摸下半部分，出牌
                                pCard->setPosition(m_startVec);
                                CMD_C_OutCard OutCard;
                                memset(&OutCard, 0, sizeof(CMD_C_OutCard));
                                OutCard.cbCardData = static_cast<uint8_t>(pCard->getTag());
                                m_GameEngine->onUserOutCard(OutCard);
                            }
                        } else {
                            pCard->setPosition(m_startVec);    //移动
                        }
                    }
                    break;
                }

            }
        }
    }
}


void GameLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    if (strcmp(pName, "Button_Over_Close") == 0) {    //关闭按钮
        m_pGameOverNode->removeFromParent();
        m_pGameOverNode = NULL;
        m_GameEngine->onGameRestart();              //重新开始游戏
    } else if (strcmp(pName, "Button_Exit") == 0) {     //退出游戏按钮
        GameSceneManager::getInstance()->confirm("退出游戏后，本局游戏将直接结束无法恢复，确定是否退出？", false, false, this, CC_CALLFUNCN_SELECTOR(GameLayer::exitGame));
    } else if (strcmp(pName, "Button_Set") == 0) {      //游戏设置按钮
        m_pLayer->addChild(SetLayer::create()->GetLayer()); //显示设置层
    }
}

/**
 * 退出游戏
 */
void GameLayer::exitGame(Node *) {
    GameSceneManager::getInstance()->end();
}

/**
 * 椅子视图切换成界面视图
 * @param cbChairID
 * @return
 */
uint8_t GameLayer::switchViewChairID(uint8_t cbChairID) {
    return (cbChairID + m_CurPlayer - m_MeChairID) % m_CurPlayer;
}

/**
 * 界面视图切换成椅子视图
 * @param cbViewID
 * @return
 */
uint8_t GameLayer::switchChairViewID(uint8_t cbViewID) {
    return (cbViewID + m_MeChairID) % m_CurPlayer;
}

/**
 * 播放声音
 * @param file
 */
void GameLayer::playSound(std::string file) {
    float volume = GameConfig::getInstance()->m_EffectsVolume;
    if (volume > 0) {
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);   //音量
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(file.c_str(), false);
    }
}