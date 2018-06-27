//
// Created by farmer on 2018/6/24.
//

#include "HelloLayer.h"

HelloLayer::HelloLayer() {
    cocos2d::log("初始化HelloLayer");
}

HelloLayer::~HelloLayer() {
    cocos2d::log("释放HelloLayer");
}

void HelloLayer::onEnter() {
    Node::onEnter();
}

void HelloLayer::onExit() {
    Node::onExit();
}

bool HelloLayer::init() {
    return Layer::init();
}

Node *HelloLayer::createLayer() {
    m_pLayer = CSLoader::createNode("res/HelloLayer.csb");  //加载Cocostudio创建的Layer
    m_pLayer->addChild(this, -1);
    setTouchEventListener(m_pLayer);
    return m_pLayer;
}

void HelloLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    cocos2d::log("控件:%s 触发onTouchEnded事件", pName);
}
