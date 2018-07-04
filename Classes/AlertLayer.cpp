//
// Created by farmer on 2018/7/4.
//

#include "AlertLayer.h"

AlertLayer::AlertLayer() {
    m_pOKSelectorTarget = NULL;
    m_pCancelSelectorTarget = NULL;
    m_OKSelector = NULL;
    m_CancelSelector = NULL;
    m_autoClose = false;
    initLayer();
}

AlertLayer::~AlertLayer() {

}

Node *AlertLayer::GetLayer() {
    return m_pLayer;
}

void AlertLayer::initLayer() {
    m_pLayer = CSLoader::createNode("res/AlertLayer.csb");
    m_pLayer->addChild(this, -1);
    m_pLayer->setScale(0.5f);
    m_pLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f, 1.0f))); //动画
    cocos2d::Node* pNode = LayerColor::create(Color4B(0,0,0,200));
    Node* pLayoutNode = m_pLayer->getChildren().at(0);
    pLayoutNode->addChild(pNode,-1);
    setTouchEventListener(m_pLayer);
}


/**
 * 设置Alert类型
 * @param typeEnum
 *  类型
 * @param autoClose
 *  点击按钮是否关闭窗口
 */
void AlertLayer::setAlertType(AlertLayer::AlertTypeEnum typeEnum, bool autoClose) {
    switch (typeEnum) {
        case AlertTypeEnum::ENUM_ALERT : {
            UIHelper::seekNodeByName(m_pLayer, "Panel_Button_Group_Cancel")->setVisible(false);
            UIHelper::seekNodeByName(m_pLayer, "Panel_Button_Group_OK")->setVisible(true);
            break;
        }
        case AlertTypeEnum::ENUM_CONFIRM: {
            UIHelper::seekNodeByName(m_pLayer, "Panel_Button_Group_Cancel")->setVisible(true);
            UIHelper::seekNodeByName(m_pLayer, "Panel_Button_Group_OK")->setVisible(false);
            break;
        }
    }
    m_autoClose = autoClose;
}

void AlertLayer::setCallback(cocos2d::Node *okTarget, SEL_CallFuncN okSelector,cocos2d::Node *cancelTarget, SEL_CallFuncN cancelSelector) {
    m_pOKSelectorTarget = okTarget;
    m_OKSelector = okSelector;
    m_pCancelSelectorTarget = cancelTarget;
    m_CancelSelector = cancelSelector;
}

/**
 * 按钮事件
 * @param pWidget
 * @param pName
 */
void AlertLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    if (strcmp(pName, "Button_Alert_OK")){
        if (m_pOKSelectorTarget != NULL && m_OKSelector != NULL){
            cocos2d::log("call button alert ok!");
            (m_pOKSelectorTarget->*m_OKSelector)(m_pLayer); //OK回调
        }
    }
    else if (strcmp(pName, "Button_Alert_Cancel")){
        if (m_pCancelSelectorTarget != NULL && m_CancelSelector != NULL){
            cocos2d::log("call button alert cancel!");
            (m_pCancelSelectorTarget->*m_CancelSelector)(m_pLayer); //取消回调
        }
    }
    if (m_autoClose){   //关闭当前窗口
        m_pLayer->removeFromParent();
    }
}
