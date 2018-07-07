//
// Created by farmer on 2018/7/5.
// 游戏引擎

#ifndef COCOSTUDIO_MAHJONG_GAMEENGINE_H
#define COCOSTUDIO_MAHJONG_GAMEENGINE_H

#include "GameCmd.h"
#include "GameLogic.h"
#include "FvMask.h"


class IPlayer;

enum EstimateKind {
    EstimateKind_OutCard,            //出牌效验
    EstimateKind_GangCard,            //杠牌效验
};

class IGameEngineEventListener {

public:

    /**
     * 设置玩家
     * @param pIPlayer
     */
    virtual void setIPlayer(IPlayer *pIPlayer) {
    }

    /**
     * 玩家进入通知
     * @param pIPlayer
     * @return
     */
    virtual bool onUserEnterEvent(IPlayer *pIPlayer) = 0;

    /**
     * 游戏开始
     * @param GameStart
     * @return
     */
    virtual bool onGameStartEvent(CMD_S_GameStart GameStart) = 0;

    /**
     * 发牌事件
     * @param SendCard
     * @return
     */
    virtual bool onSendCardEvent(CMD_S_SendCard SendCard) = 0;

    /**
     * 出牌事件
     * @param OutCard
     * @return
     */
    virtual bool onOutCardEvent(CMD_S_OutCard OutCard) = 0;

    /**
     * 操作通知事件
     * @param OperateNotify
     * @return
     */
    virtual bool onOperateNotifyEvent(CMD_S_OperateNotify OperateNotify) = 0;

    /**
     * 操作结果事件
     * @param OperateResult
     * @return
     */
    virtual bool onOperateResultEvent(CMD_S_OperateResult OperateResult) = 0;

    /**
     * 游戏结束事件
     * @param GameEnd
     * @return
     */
    virtual bool onGameEndEvent(CMD_S_GameEnd GameEnd) = 0;

};


class GameEngine {

private:
    IPlayer *m_pIPlayer[GAME_PLAYER];        //游戏玩家
    GameLogic *m_GameLogic;
    int64_t m_lGameScoreTable[GAME_PLAYER];           //记录总分
    uint32_t iDiceCount;                              //骰子点数
    uint8_t m_CurrChair;                              //当前椅子数量

    uint8_t m_cbBankerUser;                           //庄家用户
    uint8_t m_cbCardIndex[GAME_PLAYER][MAX_INDEX];    //用户扑克
    uint8_t m_cbMa;                                   //买马数量
    uint8_t m_cbProvideCard;                          //当前供应的牌
    uint8_t m_cbProvideUser;                          //供应的玩家
    uint8_t m_cbResumeUser;                           //暂存玩家，用于碰、杠后恢复
    uint8_t m_cbCurrentUser;                          //当前操作的玩家
    uint8_t m_cbOutCardUser;                          //出牌玩家
    uint8_t m_cbOutCardData;                          //出牌数据
    uint8_t m_cbDiscardCount[GAME_PLAYER];            //出牌数据
    uint8_t m_cbDiscardCard[GAME_PLAYER][MAX_DISCARD];//丢弃记录

    bool m_bResponse[GAME_PLAYER];                //响应标志
    bool m_cbPassPeng[GAME_PLAYER][MAX_INDEX];    //用来检测放弃碰
    bool m_bGangStatus;                           //杠上状态
    bool m_bQiangGangStatus;                      //抢杆状态
    uint8_t m_cbUserAction[GAME_PLAYER];          //用户动作，可能包含多个动作
    uint8_t m_cbTempUserAction[GAME_PLAYER];      //暂存上一次的动作情况，用来恢复自摸点“过”的情况。
    uint8_t m_cbOperateCard[GAME_PLAYER];         //操作扑克
    uint8_t m_cbPerformAction[GAME_PLAYER];       //执行动作，只会包含一个动作
    uint8_t m_cbFanShu[GAME_PLAYER];              //上一圈番数，用来判定没过手不能胡第二家
    uint8_t m_cbGangCount;                        //当前可以杠的数量
    uint8_t m_cbGangCard[MAX_WEAVE];              //可以杠的牌
    uint8_t m_cbTargetUser;                       //胡牌玩家标识
    uint8_t m_cbHuCard;                           //胡牌扑克
    uint8_t m_cbHuKind[GAME_PLAYER];              //胡牌类型
    uint8_t m_cbHuSpecial[GAME_PLAYER];           //胡牌一些特殊情况
    uint64_t m_llHuRight[GAME_PLAYER];            //胡牌权重
private:
    uint8_t m_cbSendCardData;                     //发牌扑克
    uint8_t m_cbSendCardCount;                    //发牌数目
    uint8_t m_cbOutCardCount;                     //出牌数目
    uint8_t m_cbLeftCardCount;                    //剩余数目
    uint8_t m_cbRepertoryCard[MAX_REPERTORY];     //库存扑克
    uint8_t m_cbWeaveItemCount[GAME_PLAYER];      //组合数目
    tagWeaveItem m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];//组合扑克

public:

    GameEngine();   //构造函数
    ~GameEngine();  //析构函数

public:
    void init();    //初始化数据
    bool onGameStart();     //开始游戏
    bool onGameRestart();   //重新开始
    bool onUserOutCard(CMD_C_OutCard OutCard);   //出牌命令
    bool onEventGameConclude(uint8_t cbChairID); //结束游戏
    bool onUserEnter(IPlayer *pIPlayer);    //玩家进入
    bool dispatchCardData(uint8_t cbCurrentUser, bool bTail = false);    //发牌
    bool estimateUserRespond(uint8_t cbCurrentUser, uint8_t cbCurrentCard, EstimateKind estimateKind);  //检测响应
    bool sendOperateNotify();   //发送操作通知
public:
    static GameEngine *GetGameEngine();  //获取单例
    bool onUserOperateCard(CMD_C_OperateCard OperateCard);
};


#endif //COCOSTUDIO_MAHJONG_GAMEENGINE_H
