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
    cocos2d::log("~AlertLayer");
}

Node *AlertLayer::GetLayer() {
    return m_pLayer;
}

void AlertLayer::initLayer() {
    m_pLayer = CSLoader::createNode("res/AlertLayer.csb");
    m_pLayer->addChild(this, -1);
    m_pLayer->setScale(0.5f);
    m_pLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f, 1.0f))); //动画
    cocos2d::Node *pNode = LayerColor::create(Color4B(0, 0, 0, 200));
    Node *pLayoutNode = m_pLayer->getChildren().at(0);
    pLayoutNode->addChild(pNode, -1);
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

/**
 * 设置内容
  * @param content
 *  内容
 * @param title
 *  标题

 */
void AlertLayer::setText(std::string content, std::string title) {
    ui::Text *pTextTitle = dynamic_cast<ui::Text *>(UIHelper::seekNodeByName(m_pLayer, "Text_Alert_Title"));
    pTextTitle->setString(title);
    ui::ScrollView *pScrollViewContent = dynamic_cast<ui::ScrollView *>(UIHelper::seekNodeByName(m_pLayer, "ScrollView_Alert_Content"));
    pScrollViewContent->removeAllChildren();
    ui::Text *pContent = ui::Text::create(content, "Arial", 30);    //设置字体
    pContent->setTextColor(Color4B(Color3B(135, 73, 48)));
    pContent->setPosition(Point::ZERO);
    pContent->setAnchorPoint(Vec2(0, 1));
    pContent->setTextAreaSize(Size(pScrollViewContent->getContentSize().width, 0)); //实现高度自动调整
    pContent->ignoreContentAdaptWithSize(false);
    pContent->setTextVerticalAlignment(TextVAlignment::CENTER);
    pContent->setTextHorizontalAlignment(TextHAlignment::CENTER);
    float h1 = pContent->getContentSize().height;
    float h2 = pScrollViewContent->getContentSize().height;
    pContent->setPosition(Vec2(10, ((h2 > h1) ? h2 : h1) - 10));
    pScrollViewContent->addChild(pContent);
    pScrollViewContent->setInnerContainerSize(Size(pScrollViewContent->getContentSize().width, pContent->getContentSize().height + 10));
}

void AlertLayer::setCallback(cocos2d::Node *okTarget, SEL_CallFuncN okSelector, cocos2d::Node *cancelTarget, SEL_CallFuncN cancelSelector) {
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
    bool isRemove = false;
    if (strcmp(pName, "Button_Alert_OK") == 0) {
        if (m_pOKSelectorTarget != NULL && m_OKSelector != NULL) {
            cocos2d::log("call button alert ok!");
            (m_pOKSelectorTarget->*m_OKSelector)(m_pLayer); //OK回调
        } else {
            m_pLayer->removeFromParent();
            isRemove = true;
        }
    } else if (strcmp(pName, "Button_Alert_Cancel") == 0) {
        if (m_pCancelSelectorTarget != NULL && m_CancelSelector != NULL) {
            cocos2d::log("call button alert cancel!");
            (m_pCancelSelectorTarget->*m_CancelSelector)(m_pLayer); //取消回调
        } else {
            m_pLayer->removeFromParent();
            isRemove = true;
        }
    }
    if (m_autoClose && !isRemove) {   //关闭当前窗口
        m_pLayer->removeFromParent();
    }
}
