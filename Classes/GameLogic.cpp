//
// Created by farmer on 2018/7/5.
//

#include "GameLogic.h"

//麻将牌
const uint8_t GameLogic::m_cbCardDataArray[MAX_REPERTORY] =
        {
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
    uint8_t cbCardDataTemp[sizeof(m_cbCardDataArray)];
    memcpy(cbCardDataTemp, m_cbCardDataArray, sizeof(m_cbCardDataArray));
    uint8_t cbRandCount = 0, cbPosition = 0;
    do {
        srand(static_cast<unsigned int>(time(NULL)));
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
bool GameLogic::isValidCard(uint8_t cbCardData)
{
    uint8_t cbValue= static_cast<uint8_t>(cbCardData&MASK_VALUE);
    uint8_t cbColor= static_cast<uint8_t>((cbCardData&MASK_COLOR) >> 4);
    return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
}

