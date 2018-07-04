//
// Created by farmer on 2018/6/24.
//

#include "HelloLayer.h"
#include "GameSceneManager.h"

HelloLayer::HelloLayer() {
    initLayer();
}

HelloLayer::~HelloLayer() {
    cocos2d::log("释放HelloLayer");
}

void HelloLayer::initLayer() {
    cocos2d::log("HelloLayer initLayer");
    m_pLayer = CSLoader::createNode("res/HelloLayer.csb");  //加载Cocostudio创建的Layer
    m_pLayer->addChild(this, -1);
    setTouchEventListener(m_pLayer);

}

Node *HelloLayer::GetLayer() {
    return m_pLayer;
}

void HelloLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    cocos2d::log("控件:%s 触发onTouchEnded事件", pName);
    GameSceneManager::getInstance()->alert("");
}
