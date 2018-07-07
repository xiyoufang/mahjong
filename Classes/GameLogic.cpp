//
// Created by farmer on 2018/7/5.
//

#include "GameLogic.h"

//麻将牌
const uint8_t GameLogic::m_cbCardDataArray[MAX_REPERTORY] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,                        //筒子
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,                        //筒子
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,                        //筒子
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,                        //筒子
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,                        //万子
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,                        //万子
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,                        //万子
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,                        //万子
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,                        //条子
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,                        //条子
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,                        //条子
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,                        //条子
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,                                    //番子
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,                                    //番子
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,                                    //番子
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,                                    //番子
};

/**
 * 洗牌
 * @param cbCardData
 * @param cbMaxCount
 */
void GameLogic::shuffle(uint8_t *cbCardData, uint8_t cbMaxCount) {
    srand(static_cast<unsigned int>(time(NULL)));
    uint8_t cbCardDataTemp[sizeof(m_cbCardDataArray)];
    memcpy(cbCardDataTemp, m_cbCardDataArray, sizeof(m_cbCardDataArray));
    uint8_t cbRandCount = 0, cbPosition = 0;
    do {
        cbPosition = static_cast<uint8_t>(rand() % (cbMaxCount - cbRandCount));
        cbCardData[cbRandCount++] = cbCardDataTemp[cbPosition];
        cbCardDataTemp[cbPosition] = cbCardDataTemp[cbMaxCount - cbRandCount];
    } while (cbRandCount < cbMaxCount);
}

/**
 * Index转成牌
 * @param cbCardIndex
 * @return
 */
uint8_t GameLogic::switchToCardData(uint8_t cbCardIndex) {
    return static_cast<uint8_t>(((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1));;
}

/**
 * 牌转成Index
 * @param cbCardData
 * @return
 */
uint8_t GameLogic::switchToCardIndex(uint8_t cbCardData) {
    return static_cast<uint8_t>(((cbCardData & MASK_COLOR) >> 4) * 9 + (cbCardData & MASK_VALUE) - 1);
}

/**
 * Index转成牌
 * @param cbCardIndex
 *  牌的Index
 * @param cbCardData
 *  牌
 * @param bMaxCount
 *  数量
 * @return
 */
uint8_t GameLogic::switchToCardData(uint8_t *cbCardIndex, uint8_t *cbCardData, uint8_t bMaxCount) {
    uint8_t bPosition = 0;
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndex[i] != 0) {
            for (uint8_t j = 0; j < cbCardIndex[i]; j++) {
                cbCardData[bPosition++] = switchToCardData(i);
            }
        }
    }
    return bPosition;
}

/**
 * 牌转成Index
 * @param cbCardData
 *  牌
 * @param cbCardCount
 *  数量
 * @param cbCardIndex
 *  牌的Index
 * @return
 */
uint8_t GameLogic::switchToCardIndex(uint8_t *cbCardData, uint8_t cbCardCount, uint8_t *cbCardIndex) {
    //设置变量
    memset(cbCardIndex, 0, sizeof(uint8_t) * MAX_INDEX);
    //转换扑克
    for (uint8_t i = 0; i < cbCardCount; i++) {
        cbCardIndex[switchToCardIndex(cbCardData[i])]++;
    }
    return cbCardCount;
}


/**
 * 是否有效的牌
 * @param cbCardData
 * @return
 */
bool GameLogic::isValidCard(uint8_t cbCardData) {
    uint8_t cbValue = static_cast<uint8_t>(cbCardData & MASK_VALUE);
    uint8_t cbColor = static_cast<uint8_t>((cbCardData & MASK_COLOR) >> 4);
    return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
}

/**
 * 移除牌
 * @param cbCardIndex
 *  牌数组
 * @param cbRemoveCard
 *  移除的牌
 * @return
 */
bool GameLogic::removeCard(uint8_t *cbCardIndex, uint8_t cbRemoveCard) {
    uint8_t cbRemoveIndex = switchToCardIndex(cbRemoveCard);     //删除扑克
    if (cbCardIndex[cbRemoveIndex] > 0) {
        cbCardIndex[cbRemoveIndex]--;
        return true;
    }
    return false;
}

/**
 * 移除牌
 * @param cbCardIndex
 *  牌数组
 * @param cbRemoveCard
 *  牌
 * @param cbRemoveCount
 *  数量
 * @return
 */
bool GameLogic::removeCard(uint8_t *cbCardIndex, uint8_t *cbRemoveCard, uint8_t cbRemoveCount) {
    //删除扑克
    for (uint8_t i = 0; i < cbRemoveCount; i++) {
        uint8_t cbRemoveIndex = switchToCardIndex(cbRemoveCard[i]);
        if (cbCardIndex[cbRemoveIndex] == 0) {
            for (uint8_t j = 0; j < i; j++) {
                cbCardIndex[switchToCardIndex(cbRemoveCard[j])]++;
            }
            return false;
        } else {
            //删除扑克
            --cbCardIndex[cbRemoveIndex];
        }
    }
    return true;
}

/**
 * 移除牌
 * @param cbCardData
 *  牌数组
 * @param cbCardCount
 *  牌数量
 * @param cbRemoveCard
 *  牌
 * @param cbRemoveCount
 *  牌数量
 * @return
 */
bool GameLogic::removeCard(uint8_t *cbCardData, uint8_t cbCardCount, uint8_t *cbRemoveCard, uint8_t cbRemoveCount) {
    //定义变量
    uint8_t cbDeleteCount = 0, cbTempCardData[MAX_COUNT];
    if (cbCardCount > sizeof(cbTempCardData)) return false;
    memcpy(cbTempCardData, cbCardData, cbCardCount * sizeof(cbCardData[0]));
    for (uint8_t i = 0; i < cbRemoveCount; i++) {
        for (uint8_t j = 0; j < cbCardCount; j++) {
            if (cbRemoveCard[i] == cbTempCardData[j]) {
                cbDeleteCount++;
                cbTempCardData[j] = 0;
                break;
            }
        }
    }
    //成功判断
    if (cbDeleteCount != cbRemoveCount) {
        return false;
    }
    //清理扑克
    uint8_t cbCardPos = 0;
    for (uint8_t i = 0; i < cbCardCount; i++) {
        if (cbTempCardData[i] != 0) {
            cbCardData[cbCardPos++] = cbTempCardData[i];
        }
    }
    return true;
}

/**
 * 删除指定的麻将
 * @param cbCardIndex
 *  麻将数组
 * @param cbRemoveCard
 *  麻将
 * @return
 */
bool GameLogic::removeAllCard(uint8_t cbCardIndex[MAX_INDEX], uint8_t cbRemoveCard) {
    uint8_t cbRemoveIndex = switchToCardIndex(cbRemoveCard);
    cbCardIndex[cbRemoveIndex] = 0;
    return true;
}

/**
 * 获取牌数量
 * @param cbCardIndex
 * @return
 */
uint8_t GameLogic::getCardCount(uint8_t *cbCardIndex) {
    //数目统计
    uint8_t cbCardCount = 0;
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        cbCardCount += cbCardIndex[i];
    }
    return cbCardCount;
}


/**
 * 获取组合的牌
 * @param cbWeaveKind
 *  组合类型
 * @param cbCenterCard
 *  中心牌
 * @param cbCardBuffer
 *  获取的牌
 * @return
 */
uint8_t GameLogic::getWeaveCard(uint8_t cbWeaveKind, uint8_t cbCenterCard, uint8_t *cbCardBuffer) {
    //组合扑克
    switch (cbWeaveKind) {
        case WIK_P:        //碰牌操作
        {
            //设置变量
            cbCardBuffer[0] = cbCenterCard;
            cbCardBuffer[1] = cbCenterCard;
            cbCardBuffer[2] = cbCenterCard;
            return 3;
        }
        case WIK_G:        //杠牌操作
        {
            //设置变量
            cbCardBuffer[0] = cbCenterCard;
            cbCardBuffer[1] = cbCenterCard;
            cbCardBuffer[2] = cbCenterCard;
            cbCardBuffer[3] = cbCenterCard;
            return 4;
        }
        default: {
        }
    }
    return 0;
}

/**
 * 检测碰
 * @param cbCardIndex
 * @param cbCurrentCard
 * @return
 */
uint8_t GameLogic::estimatePengCard(uint8_t *cbCardIndex, uint8_t cbCurrentCard) {
    //碰牌判断
    return static_cast<uint8_t>((cbCardIndex[switchToCardIndex(cbCurrentCard)] >= 2) ? WIK_P : WIK_NULL);
}

/**
 * 检测杠
 * @param cbCardIndex
 * @param cbCurrentCard
 * @return
 */
uint8_t GameLogic::estimateGangCard(uint8_t *cbCardIndex, uint8_t cbCurrentCard) {
    return static_cast<uint8_t>((cbCardIndex[switchToCardIndex(cbCurrentCard)] == 3) ? WIK_G : WIK_NULL);
}

/**
 * 分析杠
 * @param cbCardIndex
 *  手上的牌
 * @param WeaveItem
 *  碰、杠的牌
 * @param cbWeaveCount
 *  碰、杠数量
 * @param GangCardResult
 *  杠分析结果
 * @return
 */
uint8_t GameLogic::analyseGangCard(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, tagGangCardResult &GangCardResult) {
    //设置变量
    uint8_t cbActionMask = WIK_NULL;
    memset(&GangCardResult, 0, sizeof(GangCardResult));
    //手上杠牌
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndex[i] == 4) {
            cbActionMask |= WIK_G;
            GangCardResult.cbPublic[GangCardResult.cbCardCount] = FALSE;
            GangCardResult.cbCardData[GangCardResult.cbCardCount++] = switchToCardData(i);
        }
    }
    //组合杠牌
    for (uint8_t i = 0; i < cbWeaveCount; i++) {
        if (WeaveItem[i].cbWeaveKind == WIK_P) {
            if (cbCardIndex[switchToCardIndex(WeaveItem[i].cbCenterCard)] == 1) {
                cbActionMask |= WIK_G;
                GangCardResult.cbPublic[GangCardResult.cbCardCount] = TRUE;
                GangCardResult.cbCardData[GangCardResult.cbCardCount++] = WeaveItem[i].cbCenterCard;
            }
        }
    }
    return cbActionMask;
}

/**
 * 分析胡牌
 * @param cbCardIndex
 *  手上的牌
 * @param WeaveItem
 *  碰、杠的牌
 * @param cbWeaveCount
 *  碰、杠数量
 * @param cbCurrentCard
 *  当前牌
 * @param huKind
 *  胡牌类型
 * @param huRight
 *  胡牌权重
 * @param huSpecial
 *  特殊类型
 * @param cbSendCardCount
 *  发牌总数
 * @param cbOutCardCount
 *  出牌总数
 * @param bGangStatus
 *  当前杠的状态（杠开）
 * @param bZimo
 *  自摸标识
 * @param bQiangGangStatus
 *  枪杆状态（枪杆）
 * @param cbFanShu
 *  牌型的番数
 * @param bCheck
 *  校验位（处理 杠先碰 在杠逻辑）
 * @return
 */
uint8_t GameLogic::analyseHuCard(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, uint8_t cbCurrentCard, uint8_t &huKind, uint64_t &huRight, uint8_t &huSpecial, const uint8_t cbSendCardCount, const uint8_t cbOutCardCount, const bool bGangStatus, const bool bZimo, const bool bQiangGangStatus, uint8_t &cbFanShu, const bool bCheck) {
    //=================构造扑克开始
    uint8_t cbCardIndexTemp[MAX_INDEX];
    memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));                //临时扑克，用来分析
    if (cbCurrentCard != 0) {cbCardIndexTemp[switchToCardIndex(cbCurrentCard)]++;}//14只牌
    //计算数目
    uint8_t cbCardCountTemp = getCardCount(cbCardIndexTemp);
    uint8_t cbCardCount = static_cast<uint8_t>(cbCardCountTemp - 1);
    //=================构造扑克结束
    //分析扑克
    CAnalyseItemArray AnalyseItemArray;
    AnalyseItemArray.clear();
    analyseCard(cbCardIndexTemp, cbCardCountTemp, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //*********************************分析胡牌方式开始**********************************************//
    //平胡
    huRight |= pingHu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //碰碰胡
    huRight |= pengPengHu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //清一色
    huRight |= qingSe(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //七对
    huRight |= qiDui(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //钓鱼
    huRight |= diaoYu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //********************************分析胡牌类型***************************************************//
    huKind |= ziMo(huRight, bGangStatus, bZimo);            //自摸
    huKind |= gangKai(huRight, bGangStatus, bZimo);         //杠开
    huKind |= qiangGang(huRight, bQiangGangStatus, bZimo);  //枪杆
    huKind |= jiePao(huRight, bQiangGangStatus, bZimo);     //接炮

    //*********************************分析特殊牌****************************************************//
    huSpecial |= gangPai(cbCardIndexTemp);                          //有杠
    huSpecial |= danZhang(cbCardCount);                             //单张牌
    huSpecial |= diHu(cbSendCardCount, cbOutCardCount);             //地胡
    huSpecial |= tianHu(cbSendCardCount, cbOutCardCount);           //天胡
    huSpecial |= kaZhang(cbCardIndex, WeaveItem, cbWeaveCount);     //卡张判定
    //************************************************************************************************//

    //结果判断
    if (huRight != 0x0) {                                                   //胡牌
        /*//有些地方麻将规则胡牌和番数有关，需要在此处理
        uint8_t cbFs = getHuFanShu(huRight, huKind, huSpecial);             //计算番数
        //如果bCheck为true，永远返回false，不存在枪杆
        if ((huKind & CHK_QG) == 0x0 && !bZimo) {                           //不是枪杆并且不是自摸才判定
            if (!bCheck && cbFs <= cbFanShu)                                //番数如果不大于这轮最大的番数则不可以胡牌，枪杆除外。
            {
                return WIK_NULL;
            }
        }
        cbFanShu = cbFs;                                                    //暂存番数
        if (cbFs < 0x2)                                                     //1分的平湖
        {
            if ((huKind & CHK_ZM) == 0x0 && (huKind & CHK_QG) == 0x0)       //麻将不能胡
            {
                return WIK_NULL;
            }
        }*/
        return WIK_H;
    }
    return WIK_NULL;
}

/**
 * 获取胡牌番数
 * @param huRight
 * @param huKind
 * @param huSpecial
 * @return
 */
uint8_t GameLogic::getHuFanShu(const uint64_t huRight, const uint8_t huKind, const uint8_t huSpecial) {
    return 1;
}

/**
 * 获取操作权重
 * @param wUserAction
 * @return
 */
uint8_t GameLogic::getUserActionRank(uint8_t cbUserAction) {
    if ((cbUserAction & WIK_H) != 0x0) {return 3;}  //胡牌优先级
    if ((cbUserAction & WIK_G) != 0x0) {return 2;} //杠牌优先级
    if ((cbUserAction & WIK_P) != 0x0) {return 1;} //碰牌优先级
    return 0;
}

/**
 * 分析能够胡牌的数量
 * @param cbCardIndex
 * @param WeaveItem
 * @param cbWeaveCount
 * @return
 */
uint8_t GameLogic::analyseHuCardCount(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount) {
    uint8_t cbCount = 0;
    uint8_t cbCardIndexTemp[MAX_INDEX];
    memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
    for (uint8_t j = 0; j < MAX_INDEX; j++) {
        uint8_t cbCurrentCard = switchToCardData(j);
        if (analyseCanHuCard(cbCardIndexTemp, WeaveItem, cbWeaveCount, cbCurrentCard)) {
            cbCount++;
        }
    }
    return cbCount;
}

bool GameLogic::analyseCard(const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbItemCount, CAnalyseItemArray &AnalyseItemArray) {
    //效验数目
    if ((cbCardCount < 2) || (cbCardCount > MAX_COUNT) || ((cbCardCount - 2) % 3 != 0)) return false;
    //变量定义
    uint8_t cbKindItemCount = 0;
    tagKindItem KindItem[2 * MAX_INDEX];
    memset(KindItem, 0, sizeof(KindItem));
    //需求判断
    uint8_t cbLessKindItem = static_cast<uint8_t>((cbCardCount - 2) / 3);
    //单吊判断
    if (cbLessKindItem == 0) {
        //牌眼判断
        for (uint8_t i = 0; i < MAX_INDEX; i++) {
            if (cbCardIndex[i] == 2) {
                //变量定义
                tagAnalyseItem AnalyseItem;
                memset(&AnalyseItem, 0, sizeof(AnalyseItem));
                //设置结果
                for (uint8_t j = 0; j < cbItemCount; j++) {
                    AnalyseItem.cbWeaveKind[j] = WeaveItem[j].cbWeaveKind;
                    AnalyseItem.cbCenterCard[j] = WeaveItem[j].cbCenterCard;
                }
                AnalyseItem.cbCardEye = switchToCardData(i);
                AnalyseItemArray.push_back(AnalyseItem);            //插入结果
                return true;
            }
        }
        return false;
    }
    //拆分分析
    if (cbCardCount >= 3) {
        for (uint8_t i = 0; i < MAX_INDEX; i++) {
            //同牌判断
            if (cbCardIndex[i] >= 3) {
                KindItem[cbKindItemCount].cbCenterCard = switchToCardData(i);
                KindItem[cbKindItemCount].cbCardIndex[0] = i;
                KindItem[cbKindItemCount].cbCardIndex[1] = i;
                KindItem[cbKindItemCount].cbCardIndex[2] = i;
                KindItem[cbKindItemCount++].cbWeaveKind = WIK_P;
            }
            //连牌判断
            if ((i < (MAX_INDEX - 2 - 7)) && (cbCardIndex[i] > 0) && ((i % 9) < 7)) {
                for (uint8_t j = 1; j <= cbCardIndex[i]; j++) {
                    if ((cbCardIndex[i + 1] >= j) && (cbCardIndex[i + 2] >= j)) {
                        KindItem[cbKindItemCount].cbCenterCard = switchToCardData(static_cast<uint8_t>(i + 1));
                        KindItem[cbKindItemCount].cbCardIndex[0] = i;
                        KindItem[cbKindItemCount].cbCardIndex[1] = static_cast<uint8_t>(i + 1);
                        KindItem[cbKindItemCount].cbCardIndex[2] = static_cast<uint8_t>(i + 2);
                        KindItem[cbKindItemCount++].cbWeaveKind = WIK_S;
                    }
                }
            }
        }
    }

    //组合分析
    if (cbKindItemCount >= cbLessKindItem) {
        //变量定义
        uint8_t cbCardIndexTemp[MAX_INDEX];
        memset(cbCardIndexTemp, 0, sizeof(cbCardIndexTemp));
        //变量定义
        uint8_t cbIndex[MAX_WEAVE] = {0, 1, 2, 3};
        tagKindItem *pKindItem[MAX_WEAVE];
        memset(&pKindItem, 0, sizeof(pKindItem));
        //开始组合
        do {
            //设置变量
            memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
            for (uint8_t i = 0; i < cbLessKindItem; i++) {
                pKindItem[i] = &KindItem[cbIndex[i]];
            }
            //数量判断
            bool bEnoughCard = true;
            for (uint8_t i = 0; i < cbLessKindItem * 3; i++) {
                //存在判断
                uint8_t cbTempCardIndex = pKindItem[i / 3]->cbCardIndex[i % 3];
                if (cbCardIndexTemp[cbTempCardIndex] == 0) {
                    bEnoughCard = false;
                    break;
                } else
                    cbCardIndexTemp[cbTempCardIndex]--;
            }


            if (bEnoughCard) {            //胡牌判断
                uint8_t cbCardEye = 0;    //牌眼判断
                //是否继续
                for (uint8_t i = 0; i < MAX_INDEX; i++) {
                    if (cbCardIndexTemp[i] == 2) {
                        //眼牌数据
                        cbCardEye = switchToCardData(i);
                        //是否继续
                        break;
                    }
                }

                //组合类型
                if (cbCardEye != 0) {
                    //变量定义
                    tagAnalyseItem AnalyseItem;
                    memset(&AnalyseItem, 0, sizeof(AnalyseItem));
                    //设置组合
                    for (uint8_t i = 0; i < cbItemCount; i++) {
                        AnalyseItem.cbWeaveKind[i] = WeaveItem[i].cbWeaveKind;
                        AnalyseItem.cbCenterCard[i] = WeaveItem[i].cbCenterCard;
                    }
                    //设置牌型
                    for (uint8_t i = 0; i < cbLessKindItem; i++) {
                        AnalyseItem.cbWeaveKind[i + cbItemCount] = pKindItem[i]->cbWeaveKind;
                        AnalyseItem.cbCenterCard[i + cbItemCount] = pKindItem[i]->cbCenterCard;
                    }
                    AnalyseItem.cbCardEye = cbCardEye;      //设置牌眼
                    AnalyseItemArray.push_back(AnalyseItem);//插入结果
                }
            }

            //设置索引
            if (cbIndex[cbLessKindItem - 1] == (cbKindItemCount - 1)) {
                uint8_t i = static_cast<uint8_t>(cbLessKindItem - 1);
                for (; i > 0; i--) {
                    if ((cbIndex[i - 1] + 1) != cbIndex[i]) {
                        uint8_t cbNewIndex = cbIndex[i - 1];
                        for (uint8_t j = static_cast<uint8_t>(i - 1); j < cbLessKindItem; j++)
                            cbIndex[j] = static_cast<uint8_t>(cbNewIndex + j - i + 2);
                        break;
                    }
                }

                if (i == 0)
                    break;

            } else
                cbIndex[cbLessKindItem - 1]++;

        } while (true);

    }
    return (AnalyseItemArray.size() > 0);
}

bool GameLogic::analyseTingCard(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount) {
    uint8_t cbCardIndexTemp[MAX_INDEX];
    memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndexTemp[i] == 0) continue;//空牌过滤
        cbCardIndexTemp[i]--;                  //假设出掉的牌
        for (uint8_t j = 0; j < MAX_INDEX; j++) {
            uint8_t cbCurrentCard = switchToCardData(j);
            if (analyseCanHuCard(cbCardIndexTemp, WeaveItem, cbWeaveCount, cbCurrentCard)) {
                return true;
            }
        }
        cbCardIndexTemp[i]++;                 //还原假设的牌
    }
    return false;
}

/**
 * 判断是否能胡牌
 * @param cbCardIndex
 * @param WeaveItem
 * @param cbWeaveCount
 * @param cbCurrentCard
 * @return
 */
bool GameLogic::analyseCanHuCard(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, uint8_t cbCurrentCard) {
    //=================构造扑克开始
    uint8_t cbCardIndexTemp[MAX_INDEX];
    memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));                //临时扑克，用来分析
    if (cbCurrentCard != 0) {cbCardIndexTemp[switchToCardIndex(cbCurrentCard)]++;}//14只牌
    //计算数目
    uint8_t cbCardCountTemp = getCardCount(cbCardIndexTemp);
    uint8_t cbCardCount = static_cast<uint8_t>(cbCardCountTemp - 1);
    //分析扑克
    CAnalyseItemArray AnalyseItemArray;
    AnalyseItemArray.clear();
    analyseCard(cbCardIndexTemp, cbCardCountTemp, WeaveItem, cbWeaveCount, AnalyseItemArray);
    //是否能胡牌
    return canHu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray);
}

/**
 * 分析听牌结果
 * @param cbCardIndex
 * @param WeaveItem
 * @param cbWeaveCount
 * @param tingResult
 * @return
 */
bool GameLogic::analyseTingCardResult(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, tagTingResult &tingResult) {
    memset(&tingResult, 0, sizeof(tagTingResult));
    uint8_t cbCardIndexTemp[MAX_INDEX];
    memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
    for (uint8_t j = 0; j < MAX_INDEX; j++) {
        uint8_t cbCurrentCard = switchToCardData(j);
        if (analyseCanHuCard(cbCardIndexTemp, WeaveItem, cbWeaveCount, cbCurrentCard)) {
            tingResult.cbTingCard[tingResult.cbTingCount++] = cbCurrentCard;
        }
    }
    return tingResult.cbTingCount > 0;
}

/**
 * 判断是否能胡
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
bool GameLogic::canHu(const uint8_t cbCardIndexTemp[MAX_INDEX], const uint8_t cbCardCountTemp, const uint8_t cbCardIndex[MAX_INDEX], const uint8_t cbCardCount, tagWeaveItem WeaveItem[], uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    if (cbWeaveCount == 0) {    //计算七对，	//不存在碰、杠
        uint8_t cbDuiCount = 0;
        for (uint8_t i = 0; i < MAX_INDEX; i++) {
            if (cbCardIndexTemp[i] == 2) //牌的数量存在奇数就是不是七对，否则是
            {
                cbDuiCount++;
            }
        }
        if (cbDuiCount == 7) {
            return true;
        }
    }
    return AnalyseItemArray.size() > 0;
}


/**
 * 平湖判定
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
uint64_t GameLogic::pingHu(const uint8_t *cbCardIndexTemp, const uint8_t cbCardCountTemp, const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    if (AnalyseItemArray.size() > 0) {
        for (uint8_t i = 0; i < AnalyseItemArray.size(); i++) {
            bool bLianCard = false, bPengCard = false;
            tagAnalyseItem *pAnalyseItem = &AnalyseItemArray[i];
            for (uint8_t j = 0; j < sizeof(pAnalyseItem->cbWeaveKind); j++) {
                uint8_t cbWeaveKind = pAnalyseItem->cbWeaveKind[j];
                bPengCard = ((cbWeaveKind & (WIK_G | WIK_P)) != 0) ? true : bPengCard;
                bLianCard = ((cbWeaveKind & (WIK_S)) != 0) ? true : bLianCard;
            }
            if (bLianCard == true) {//平胡
                return CHR_PH;
            }
        }
    }
    return CHR_NULL;
}

/**
 * 清一色判断
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
uint64_t GameLogic::qingSe(const uint8_t *cbCardIndexTemp, const uint8_t cbCardCountTemp, const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    uint8_t cbCardColor = 0xFF;
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndexTemp[i] != 0) {
            uint8_t cbTempCardColor = static_cast<uint8_t>(switchToCardData(i) & MASK_COLOR);

            if (cbCardColor == 0xFF) {//花色判断
                cbCardColor = cbTempCardColor;
            }
            if (cbTempCardColor != cbCardColor) {
                return CHR_NULL;
            }
        }
    }
    //组合判断
    for (uint8_t i = 0; i < cbWeaveCount; i++) {
        uint8_t cbCenterCard = WeaveItem[i].cbCenterCard;
        if ((cbCenterCard & MASK_COLOR) != cbCardColor) return CHR_NULL;
    }
    //对号判定
    if (canHu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray)) {
        return CHR_QS;
    }
    return CHR_NULL;
}

/**
 * 碰碰胡判定
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
uint64_t GameLogic::pengPengHu(const uint8_t *cbCardIndexTemp, const uint8_t cbCardCountTemp, const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    //胡牌分析
    if (AnalyseItemArray.size() > 0) {
        //牌型分析
        for (uint8_t i = 0; i < AnalyseItemArray.size(); i++) {
            //变量定义
            bool bLianCard = false, bPengCard = false;
            tagAnalyseItem *pAnalyseItem = &AnalyseItemArray[i];
            //牌型分析
            for (uint8_t j = 0; j < sizeof(pAnalyseItem->cbWeaveKind); j++) {
                uint8_t cbWeaveKind = pAnalyseItem->cbWeaveKind[j];
                bPengCard = ((cbWeaveKind & (WIK_G | WIK_P)) != 0) ? true : bPengCard;
                bLianCard = ((cbWeaveKind & (WIK_S)) != 0) ? true : bLianCard;
            }
            //碰碰胡
            if (!bLianCard && bPengCard) {
                return CHR_PPH;
            }
        }
    }
    return CHR_NULL;

}

/**
 * 七对判断
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
uint64_t GameLogic::qiDui(const uint8_t *cbCardIndexTemp, const uint8_t cbCardCountTemp, const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    //不存在碰、杠
    if (cbWeaveCount > 0) return CHR_NULL;
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndexTemp[i] == 1 || cbCardIndexTemp[i] == 3) //牌的数量存在奇数就是不是七对，否则是
        {
            return CHR_NULL;
        }
    }
    return CHR_QD;
}

/**
 * 单吊判断
 * @param cbCardIndexTemp
 * @param cbCardCountTemp
 * @param cbCardIndex
 * @param cbCardCount
 * @param WeaveItem
 * @param cbWeaveCount
 * @param AnalyseItemArray
 * @return
 */
uint64_t GameLogic::diaoYu(const uint8_t *cbCardIndexTemp, const uint8_t cbCardCountTemp, const uint8_t *cbCardIndex, const uint8_t cbCardCount, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount, CAnalyseItemArray &AnalyseItemArray) {
    //碰碰胡
    if (pengPengHu(cbCardIndexTemp, cbCardCountTemp, cbCardIndex, cbCardCount, WeaveItem, cbWeaveCount, AnalyseItemArray) == CHR_PPH
            && (cbCardCountTemp == 2)) {
        return CHR_DY;
    }
    return CHR_NULL;
}

/**
 * 手上有杠判断
 * @param cbCardIndex
 * @return
 */
uint8_t GameLogic::gangPai(const uint8_t *cbCardIndex) {
    uint8_t cbCardColor = 0xFF;
    for (uint8_t i = 0; i < MAX_INDEX; i++) {
        if (cbCardIndex[i] == 4) {
            return CHS_GP;
        }
    }
    return CHS_NULL;
}

/**
 * 单张判断
 * @param cbCardCount
 * @return
 */
uint8_t GameLogic::danZhang(const uint8_t cbCardCount) {
    if (cbCardCount == 2) {
        return CHS_DZ;
    }
    return CHS_NULL;
}

/**
 * 天湖判断
 * @param m_cbSendCardCount
 * @param m_cbOutCardCount
 * @return
 */
uint8_t GameLogic::tianHu(const uint8_t m_cbSendCardCount, const uint8_t m_cbOutCardCount) {
    if ((m_cbSendCardCount == 1) && (m_cbOutCardCount == 0)) {
        return CHS_TH;
    }
    return CHS_NULL;
}

/**
 * 地胡判断
 * @param m_cbSendCardCount
 * @param m_cbOutCardCount
 * @return
 */
uint8_t GameLogic::diHu(const uint8_t m_cbSendCardCount, const uint8_t m_cbOutCardCount) {
    if ((m_cbSendCardCount == 1) && (m_cbOutCardCount == 1)) {
        return CHS_DH;
    }
    return CHS_NULL;
}

/**
 * 卡张判断
 * @param cbCardIndex
 * @param WeaveItem
 * @param cbWeaveCount
 * @return
 */
uint8_t GameLogic::kaZhang(const uint8_t *cbCardIndex, tagWeaveItem *WeaveItem, uint8_t cbWeaveCount) {
    if (analyseHuCardCount(cbCardIndex, WeaveItem, cbWeaveCount) == 1) {
        return CHS_KZ;
    }
    return CHS_NULL;
}

/**
 * 自摸判断
 * @param llHuRight
 * @param bGangStatus
 * @param bZimo
 * @return
 */
uint8_t GameLogic::ziMo(uint64_t llHuRight, const bool bGangStatus, const bool bZimo) {
    if ((llHuRight != 0x0) && bZimo == true && bGangStatus == false) {
        return CHK_ZM;
    }
    return CHK_NULL;
}

/**
 * 杠开判断
 * @param llHuRight
 * @param bGangStatus
 * @param bZimo
 * @return
 */
uint8_t GameLogic::gangKai(uint64_t llHuRight, const bool bGangStatus, const bool bZimo) {
    if ((llHuRight != 0x0) && bZimo == true && bGangStatus == true) {
        return CHK_GK;
    }
    return CHK_NULL;
}

/**
 * 枪杆判断
 * @param llHuRight
 * @param bGangStatus
 * @param bZimo
 * @return
 */
uint8_t GameLogic::qiangGang(uint64_t llHuRight, const bool bGangStatus, const bool bZimo) {
    if ((llHuRight != 0x0) && bZimo == false && bGangStatus == true) {
        return CHK_QG;
    }
    return CHK_NULL;
}

/**
 * 接炮判断
 * @param llHuRight
 * @param bGangStatus
 * @param bZimo
 * @return
 */
uint8_t GameLogic::jiePao(uint64_t llHuRight, const bool bGangStatus, const bool bZimo) {
    if ((llHuRight != 0x0) && bZimo == false && bGangStatus == false) {
        return CHK_JP;
    }
    return CHK_NULL;
}
