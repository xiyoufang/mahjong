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

/**
 * 获取Layer
 * @return
 */
Node *HelloLayer::getLayer() {
    m_pLayer = CSLoader::createNode("res/HelloLayer.csb");  //加载Cocostudio创建的Layer
    m_pLayer->addChild(this, -1);
    return m_pLayer;
}


