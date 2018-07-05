//
// Created by farmer on 2018/7/5.
// 游戏逻辑

#ifndef COCOSTUDIO_MAHJONG_GAMELOGIC_H
#define COCOSTUDIO_MAHJONG_GAMELOGIC_H

#include "GameCmd.h"

#define    MASK_COLOR                    0xF0                                //花色掩码
#define    MASK_VALUE                    0x0F                                //数值掩码


#define FALSE        0
#define TRUE         1

#define WIK_NULL     0x00        //过
#define WIK_P        0x01        //碰
#define WIK_G        0x02        //杠
#define WIK_H        0x04        //胡
#define WIK_S        0x08        //吃


//////////////////////////////////////////////////////////////////////////
//类型子项
struct tagKindItem {
    uint8_t cbWeaveKind;                        //组合类型
    uint8_t cbCenterCard;                        //中心扑克
    uint8_t cbCardIndex[3];                        //扑克索引
};

//组合子项
struct tagWeaveItem {
    uint8_t cbWeaveKind;                        //组合类型
    uint8_t cbCenterCard;                        //中心扑克
    uint8_t cbPublicCard;                        //公开标志
    uint8_t cbProvideUser;                        //供应用户
    uint8_t cbValid;                            //有效标识
};

//杠牌结果
struct tagGangCardResult {
    uint8_t cbCardCount;                        //扑克数目
    uint8_t cbCardData[MAX_WEAVE];                //扑克数据
    uint8_t cbPublic[MAX_WEAVE];                //公开标识
};

//分析子项
struct tagAnalyseItem {
    uint8_t cbCardEye;                            //牌眼扑克
    uint8_t cbWeaveKind[MAX_WEAVE];                //组合类型
    uint8_t cbCenterCard[MAX_WEAVE];            //中心扑克
};

//////////////////////////////////////////////////////////////////////////

//数组说明
typedef std::vector<tagAnalyseItem, tagAnalyseItem &> CAnalyseItemArray;

class GameLogic {

private:
    static const uint8_t m_cbCardDataArray[MAX_REPERTORY];    //扑克数据
public:
    //洗牌
    void shuffle(uint8_t cbCardData[], uint8_t cbMaxCount);

public://内部函数
    uint8_t switchToCardData(uint8_t cbCardIndex);  //扑克转换
    uint8_t switchToCardIndex(uint8_t cbCardData);  //扑克转换
    uint8_t switchToCardData(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbCardData[MAX_COUNT], uint8_t bMaxCount); //扑克转换
    uint8_t switchToCardIndex(uint8_t cbCardData[], uint8_t cbCardCount, uint8_t cbCardIndex[MAX_INDEX]); //扑克转换
    bool isValidCard(uint8_t cbCardData);           //有效判断

};


#endif //COCOSTUDIO_MAHJONG_GAMELOGIC_H
