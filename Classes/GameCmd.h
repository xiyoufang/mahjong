//
// Created by farmer on 2018/7/5.
// 游戏指令数据结构，部分常量
//

#ifndef COCOSTUDIO_MAHJONG_GAMECMD_H
#define COCOSTUDIO_MAHJONG_GAMECMD_H


#include <cstdint>

#define INVALID_CHAIR                0xFF                              //无效椅子
#define INVALID_BYTE                 0xFF                              //无效BYTE

#define GAME_PLAYER                  4                                 //玩家数量
#define MAX_WEAVE                    4                                 //最大组合
#define MAX_INDEX                    34                                //最大索引
#define MAX_REPERTORY                136                               //最大库存
#define MAX_COUNT                    14                                //最大数
#define MAX_MA                       8                                 //最大马数
#define MAX_DISCARD                  60                                //丢弃的牌


struct CMD_WeaveItem {
    uint8_t cbWeaveKind;                        //组合类型
    uint8_t cbCenterCard;                       //中心扑克
    uint8_t cbPublicCard;                       //公开标志
    uint8_t cbProvideUser;                      //供应用户
    uint8_t cbValid;                            //有效标识
};

//游戏开始
struct CMD_S_GameStart {
    uint32_t iDiceCount;                         //骰子点数
    uint8_t cbBankerUser;                        //庄家用户
    uint8_t cbCurrentUser;                       //当前用户
    uint8_t cbCardData[MAX_COUNT * GAME_PLAYER]; //扑克列表
    uint8_t cbLeftCardCount;                     //剩余数目
};

//出牌命令
struct CMD_S_OutCard {
    uint8_t cbOutCardUser;                        //出牌用户
    uint8_t cbOutCardData;                        //出牌扑克
};

//发牌命令
struct CMD_S_SendCard {
    uint8_t cbCardData;                            //扑克数据
    uint8_t cbActionMask;                        //动作掩码
    uint8_t cbCurrentUser;                        //当前用户
    uint8_t cbGangCount;                        //可以杠的数量
    uint8_t cbGangCard[MAX_WEAVE];              //杠的牌
    bool bTail;                                //末尾发牌
};


//操作提示
struct CMD_S_OperateNotify {
    uint8_t cbResumeUser;                        //还原用户
    uint8_t cbActionMask;                        //动作掩码
    uint8_t cbActionCard;                        //动作扑克
    uint8_t cbGangCount;                         //可以杠的数量
    uint8_t cbGangCard[MAX_WEAVE];               //可以杠的牌
};

//操作结果
struct CMD_S_OperateResult {
    uint8_t cbOperateUser;                        //操作用户
    uint8_t cbProvideUser;                        //供应用户
    uint8_t cbOperateCode;                        //操作代码
    uint8_t cbOperateCard;                        //操作扑克
};

//游戏结束
struct CMD_S_GameEnd {
    uint8_t cbCardCount[GAME_PLAYER];                //扑克总数
    uint8_t cbCardData[GAME_PLAYER][MAX_COUNT];      //扑克数据
    uint8_t cbHuUser;                                //胡牌人员
    uint8_t cbProvideUser;                           //供应用户
    uint8_t cbHuCard;                                //供应扑克
    uint64_t dwHuRight[GAME_PLAYER];                 //胡牌类型
    uint8_t cbHuKind[GAME_PLAYER];                   //胡牌方式
    uint8_t cbHuSpecial[GAME_PLAYER];                //特殊情况
    uint8_t cbWeaveCount[GAME_PLAYER];               //组合数量
    CMD_WeaveItem WeaveItemArray[GAME_PLAYER][MAX_WEAVE];    //组合扑克
    uint8_t cbMaCard[MAX_MA];                                //扎鸟
    int64_t lMaGameScore[GAME_PLAYER];                      //单局马积分
    int64_t lNormalGameScore[GAME_PLAYER];                  //单局常规积分
    int64_t lGameScore[GAME_PLAYER];                //单局积分
    int64_t lGameScoreTable[GAME_PLAYER];           //总局积分
};

//出牌命令
struct CMD_C_OutCard {
    uint8_t cbCardData;                            //扑克数据
};

//操作命令
struct CMD_C_OperateCard {
    uint8_t cbOperateUser;                         //操作玩家
    uint8_t cbOperateCode;                         //操作代码
    uint8_t cbOperateCard;                         //操作扑克
};


#endif //COCOSTUDIO_MAHJONG_GAMECMD_H
