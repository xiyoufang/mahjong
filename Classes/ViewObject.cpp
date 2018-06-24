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

ViewObject *ViewObject::create(std::string mainString, std::string subString) {
    ViewObject *pViewObject = ViewObject::create();
    pViewObject->init();
    pViewObject->m_MainString = mainString;
    pViewObject->m_subString = subString;
    return pViewObject;
}

ViewObject *
ViewObject::create(std::string mainString, std::string subString, void *pData, unsigned int iSize) {
    ViewObject *pViewObject = ViewObject::create(mainString, subString);
    pViewObject->m_pObjectData = pData;
    pViewObject->m_iDataSize = iSize;
    return nullptr;
}
