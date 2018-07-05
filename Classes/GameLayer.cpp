//
// Created by farmer on 2018/7/4.
//

#include "GameLayer.h"
#include "AIPlayer.h"
#include "RealPlayer.h"
#include "AIEngine.h"


GameLayer::GameLayer() {
    m_CurPlayer = 0;
    m_MeChairID = 0;
    m_GameEngine = GameEngine::GetGameEngine();  //构造游戏引擎
    m_GameLogic = new GameLogic;
    memset(&m_cbCardIndex, 0, sizeof(m_cbCardIndex));
    memset(&m_cbWeaveItemCount, 0, sizeof(m_cbWeaveItemCount));
    memset(&m_cbDiscardCount, 0, sizeof(m_cbDiscardCount));
    memset(&m_cbDiscardCard, 0, sizeof(m_cbDiscardCard));
    m_cbLeftCardCount = 0;
    m_cbBankerChair = INVALID_CHAIR;
    m_bOperate = false;
    m_pOperateNotifyGroup = NULL;
    m_pTextCardNum = NULL;
    m_iOutCardTimeOut = 30;
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
    //调整头像位置
    m_FaceFrame[0]->runAction(MoveTo::create(0.50f, Vec2(0080.00f, 210.00f)));
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
            //发的牌存在动作，模拟发送操作通知
            if (SendCard.cbActionMask != WIK_NULL) {
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

void GameLayer::sendCardTimerUpdate(float f) {
    m_iOutCardTimeOut = static_cast<uint8_t>((m_iOutCardTimeOut-- < 1) ? 0 : m_iOutCardTimeOut);
    ui::ImageView *pTimer1 = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, "Image_Timer_1"));
    ui::ImageView *pTimer0 = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(m_pLayer, "Image_Timer_0"));
    int t = m_iOutCardTimeOut / 10;   //十位
    int g = m_iOutCardTimeOut % 10;   //各位
    pTimer1->loadTexture(utility::toString("res/GameLayer/timer_" , g , ".png"));
    pTimer0->loadTexture(utility::toString("res/GameLayer/timer_" , t , ".png"));
}


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
                    ui::ImageView *pImageBottom = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_bottom"));
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
                    ui::ImageView *pImageBottom = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_bottom"));
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
                    ui::ImageView *pImageBottom = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_bottom"));
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
                    ui::ImageView *pImageBottom = dynamic_cast<ui::ImageView *>(UIHelper::seekNodeByName(pWeaveNode, "Image_bottom"));
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
 * 显示桌上的牌
 * @return
 */
bool GameLayer::showAndUpdateDiscardCard() {
    return true;
}


ui::ImageView *GameLayer::createHandCardImageView(uint8_t cbViewID, uint8_t cbData) {
    return ui::ImageView::create(getHandCardImagePath(cbViewID, cbData));
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
std::string GameLayer::getBackCardImagePath(uint8_t cbViewID, uint8_t cbData) {
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
//              sendOperateResult(OperateCard);
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

}


void GameLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {

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
