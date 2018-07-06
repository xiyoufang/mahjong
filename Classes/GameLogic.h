//
// Created by farmer on 2018/7/5.
// 游戏逻辑

#ifndef COCOSTUDIO_MAHJONG_GAMELOGIC_H
#define COCOSTUDIO_MAHJONG_GAMELOGIC_H

#include "GameCmd.h"
#include <vector>

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
//胡牌类型
#define CHR_NULL     0x00    //没胡标识
#define CHR_PH       0x01    //平胡
#define CHR_PPH      0x02    //碰碰胡
#define CHR_QS       0x04    //清色
#define CHR_DY       0x08    //钓鱼
#define CHR_QD       0x10    //七对
#define CHR_DH       0x20    //地胡
#define CHR_TH       0x40    //天胡


//胡牌方式
#define CHK_NULL     0x00        //非胡
#define CHK_ZM       0x01        //自摸
#define CHK_JP       0x02        //接炮
#define CHK_QG       0x04        //抢杠
#define CHK_GK       0x08        //抢开

//特殊情况
#define CHS_NULL     0x00        //无情况
#define CHS_DZ       0x01        //单张
#define CHS_DH       0x02        //地胡
#define CHS_TH       0x04        //天胡
#define CHS_GP       0x08        //有杠
#define CHS_KZ       0x10        //卡张


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

//听牌
struct tagTingResult{
    uint8_t cbTingCount;                           //听牌数量
    uint8_t cbTingCard[MAX_INDEX];                 //听的牌
};
//////////////////////////////////////////////////////////////////////////

//数组说明
typedef std::vector<tagAnalyseItem> CAnalyseItemArray;

class GameLogic {

private:
    static const uint8_t m_cbCardDataArray[MAX_REPERTORY];    //扑克数据
public:

    void shuffle(uint8_t cbCardData[], uint8_t cbMaxCount);//洗牌
    bool removeCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbRemoveCard);//删除扑克
    bool removeCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbRemoveCard[], uint8_t cbRemoveCount); //删除扑克
    bool removeCard(uint8_t cbCardData[], uint8_t cbCardCount, uint8_t cbRemoveCard[], uint8_t cbRemoveCount); //删除扑克
    bool removeAllCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbRemoveCard);   //移除指定的牌

public://内部函数
    bool isValidCard(uint8_t cbCardData);           //有效判断
    uint8_t switchToCardData(uint8_t cbCardIndex);  //扑克转换
    uint8_t switchToCardIndex(uint8_t cbCardData);  //扑克转换
    uint8_t switchToCardData(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbCardData[MAX_COUNT], uint8_t bMaxCount); //扑克转换
    uint8_t switchToCardIndex(uint8_t cbCardData[], uint8_t cbCardCount, uint8_t cbCardIndex[MAX_INDEX]); //扑克转换
    uint8_t getCardCount(uint8_t cbCardIndex[MAX_INDEX]);  //扑克数目
    uint8_t getWeaveCard(uint8_t cbWeaveKind, uint8_t cbCenterCard, uint8_t cbCardBuffer[]);//组合扑克
    //动作判断
public:
    uint8_t estimatePengCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbCurrentCard); //碰牌判断
    uint8_t estimateGangCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbCurrentCard); //杠牌判断
    //等级函数
public:
    uint8_t getUserActionRank(uint8_t wUserAction); //动作等级
    uint8_t getHuFanShu(const uint64_t huRight, const uint8_t huKind, const uint8_t huSpecial); //胡牌分数
public://分析函数
    uint8_t analyseGangCard(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, tagGangCardResult &GangCardResult); //杠牌分析
    uint8_t analyseHuCard(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, uint8_t cbCurrentCard, uint8_t &huKind, uint64_t &huRight, uint8_t &huSpecial, const uint8_t cbSendCardCount, const uint8_t cbOutCardCount, const bool bGangStatus, const bool bZimo, const bool bQiangGangStatus, uint8_t &cbFanShu, const bool bCheck);    //胡牌分析，返回胡牌类型
    uint8_t analyseHuCardCount(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount);    //获取胡牌的数量
    bool analyseCard(const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbItemCount, CAnalyseItemArray &AnalyseItemArray); //分析扑克
    bool analyseTingCard(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount);    //是否听牌
    bool analyseCanHuCard(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, uint8_t cbCurrentCard);   //分析是否可以胡牌
    bool analyseTingCardResult(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount , tagTingResult& tingResult);
    bool canHu(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray); //能胡牌
private:    //胡牌类型
    uint64_t pingHu(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray); //平胡
    uint64_t qingSe(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray); //清一色
    uint64_t pengPengHu(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray);//碰碰胡
    uint64_t qiDui(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray);//七对
    uint64_t diaoYu(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray);//单钓
private: //一些判断
    uint8_t gangPai(const uint8_t cbCardIndex[MAX_INDEX]);    //手上有杠
    uint8_t danZhang(const uint8_t cbCardCount);    //手上剩下一张牌
    uint8_t tianHu(const uint8_t m_cbSendCardCount, const uint8_t m_cbOutCardCount);    //天胡
    uint8_t diHu(const uint8_t m_cbSendCardCount, const uint8_t m_cbOutCardCount); //地胡
    uint8_t kaZhang(const uint8_t cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], uint8_t cbWeaveCount);    //卡张
private:    //胡牌方式
    uint8_t ziMo(uint64_t llHuRight, const bool bGangStatus, const bool bZimo);//自摸
    uint8_t gangKai(uint64_t llHuRight, const bool bGangStatus, const bool bZimo);//杠开
    uint8_t qiangGang(uint64_t llHuRight, const bool bGangStatus, const bool bZimo);//枪杠
    uint8_t jiePao(uint64_t llHuRight, const bool bGangStatus, const bool bZimo);//接炮
};


#endif //COCOSTUDIO_MAHJONG_GAMELOGIC_H
