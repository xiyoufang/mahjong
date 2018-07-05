//
// Created by farmer on 2018/6/24.
//

#include "ViewObject.h"

bool ViewObject::init() {
    this->m_MainString = "";
    this->m_subString = "";
    this->m_pObjectData = NULL;
    this->m_iDataSize = 0;
    return true;
}

/**
 * 创建切换主界面对象
 * @param subString
 * @return
 */
ViewObject *ViewObject::create(std::string subString) {
    return ViewObject::create(VIEW_SWITCH_MAIN_LAYER, subString);;
}

/**
 * 创建界面对象
 * @param mainString
 *  主指令
 * @param subString
 *  子指令
 * @return
 */
ViewObject *ViewObject::create(std::string mainString, std::string subString) {
    ViewObject *pViewObject = ViewObject::create();
    pViewObject->init();
    pViewObject->m_MainString = mainString;
    pViewObject->m_subString = subString;
    return pViewObject;
}

/**
 * 创建界面对象
 * @param mainString
 *  主指令
 * @param subString
 *  子指令
 * @param pData
 *  数据指针
 * @param iSize
 *  数据大小
 * @return
 */
ViewObject *ViewObject::create(std::string mainString, std::string subString, void *pData, unsigned int iSize) {
    ViewObject *pViewObject = ViewObject::create(mainString, subString);
    pViewObject->m_pObjectData = pData;
    pViewObject->m_iDataSize = iSize;
    return nullptr;
}
