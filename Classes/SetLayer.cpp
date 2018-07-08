//
// Created by farmer on 2018/7/8.
//

#include "SetLayer.h"
#include "UIHelper.h"
#include "GameConfig.h"

SetLayer::SetLayer() {
    m_pSliderVolume = NULL;
    initLayer();
}

SetLayer::~SetLayer() {
    cocos2d::log("~SetLayer");
}

Node *SetLayer::GetLayer() {
    return m_pLayer;
}

/**
 * 初始化层
 */
void SetLayer::initLayer() {
    m_pLayer = CSLoader::createNode("res/SetLayer.csb");
    m_pLayer->addChild(this, -1);
    m_pLayer->setScale(0.5f);
    m_pLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f, 1.0f))); //动画
    setTouchEventListener(m_pLayer);
    m_pSliderVolume = dynamic_cast<ui::Slider *>(UIHelper::seekNodeByName(m_pLayer, "Slider_Volume"));
    m_pSliderVolume->setMaxPercent(100);
    m_pSliderVolume->setPercent(static_cast<int>(GameConfig::getInstance()->m_EffectsVolume * 100));
    m_pSliderVolume->addEventListener(CC_CALLBACK_2(SetLayer::sliderCallback, this));   //滑块移动事件
}

/**
 * 滑动回调
 * @param ref
 * @param type
 */
void SetLayer::sliderCallback(cocos2d::Ref *ref, cocos2d::ui::Slider::EventType type) {
    switch (type) {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED: {    //滑动条变化
            int currPercent = m_pSliderVolume->getPercent();    //当前百分比
            int maxPercent = m_pSliderVolume->getMaxPercent();  //最大百分比
            GameConfig::getInstance()->m_EffectsVolume = (float) currPercent / (float) maxPercent;
            break;
        }
        default:
            break;
    }
}

/**
 * 按钮事件
 * @param pWidget
 * @param pName
 */
void SetLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    if (strcmp(pName, "Button_Close") == 0) {
        GameConfig::getInstance()->saveConfig();    //关闭前保存配置
        m_pLayer->removeFromParent();               //关闭层
    }
}
