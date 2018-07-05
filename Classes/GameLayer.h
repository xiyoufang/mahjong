//
// Created by farmer on 2018/7/4.
//

#ifndef COCOSTUDIO_MAHJONG_GAMELAYER_H
#define COCOSTUDIO_MAHJONG_GAMELAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIHelper.h"
#include "BaseUILayer.h"
#include "IPlayer.h"
#include "GameCmd.h"
#include "Utility.h"
#include "GameEngine.h"
#include "GameLogic.h"

using namespace cocos2d;

class GameLayer : public BaseUILayer, IGameEngineEventListener {

private:
    Node *m_FaceFrame[GAME_PLAYER];     //头像信息节点
    Node *m_PlayerPanel[GAME_PLAYER];   //玩家牌的区域
    IPlayer *m_Players[GAME_PLAYER];    //全部玩家
    uint8_t m_CurPlayer;                //当前玩家数量
    uint8_t m_MeChairID;                //自己的位置
    GameEngine *m_GameEngine;           //游戏引擎
    GameLogic *m_GameLogic;             //游戏逻辑
//游戏变量
private:
    tagWeaveItem m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];                 //组合
    uint8_t m_cbCardIndex[GAME_PLAYER][MAX_INDEX];                         //玩家牌
    uint8_t m_cbWeaveItemCount[GAME_PLAYER];                               //组合数目
    uint8_t m_cbDiscardCount[GAME_PLAYER];                                 //丢弃数目
    uint8_t m_cbDiscardCard[GAME_PLAYER][MAX_DISCARD];                     //丢弃记录
    uint8_t m_cbLeftCardCount;                                             //剩余
    uint8_t m_cbBankerChair;                                               //庄
    bool m_bOperate;                                                       //是否可操作

public:
    CREATE_FUNC(GameLayer)

    GameLayer();    // 构造函数
    ~GameLayer();   //析构
protected:
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);  //按钮事件处理
    virtual void initLayer();           //初始化层
public:
    virtual Node *GetLayer();           //获取层Layer
    void aiEnterGame(float f);          //机器人进入游戏
    void onCardTouch(Ref *ref, ui::Widget::TouchEventType eventType);   //触摸牌的事件
private:
    virtual bool onUserEnterEvent(IPlayer *pIPlayer);            //玩家进入事件
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart);    //游戏开始事件

private:
    bool showAndUpdateHandCard();                        //显示手上的牌
    bool showAndUpdateDiscardCard();                     //显示桌上的牌
    ui::ImageView *createHandCardImageView(uint8_t cbViewID, uint8_t cbData);   //创建牌的ImageView
    std::string getDiscardCardImagePath(uint8_t cbViewID, uint8_t cbData);      //获取牌的图片路径
    std::string getBackCardImagePath(uint8_t cbViewID, uint8_t cbData);         //获取牌背面的路径
    std::string getHandCardImagePath(uint8_t cbViewID, uint8_t cbData);         //获取手上的牌图片路径
    uint8_t switchViewChairID(uint8_t cbChairID);        //椅子位置切换成视图位置
    uint8_t switchChairViewID(uint8_t cbViewID);

    virtual bool onSendCardEvent(CMD_S_SendCard SendCard);
    //视图位置切换成椅子位置

};


#endif //COCOSTUDIO_MAHJONG_GAMELAYER_H
