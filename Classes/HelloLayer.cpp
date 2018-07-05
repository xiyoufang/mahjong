//
// Created by farmer on 2018/6/24.
//

#include "HelloLayer.h"
#include "GameSceneManager.h"
#include "ViewControl.h"
#include "ViewObject.h"

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

/**
 * 按钮事件
 * @param pWidget
 * @param pName
 */
void HelloLayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {
    cocos2d::log("控件:%s 触发onTouchEnded事件", pName);
    GameSceneManager::getInstance()->alert("欢迎运行此游戏，此游戏是基于Cocos2d-X的开源单机麻将，\n"
                                           "本游戏使用的资源全部收集自互联网，若侵犯了您的权益请联系我进行删除。\n"
                                           "希望此游戏源码可以帮助更多的人快速入门使用CPP开发Cocos2d-X，"
                                           "获取更多软件与游戏开发信息请关注我的博客：\n"
                                           "https://www.xiyoufang.com", false, false, this, CC_CALLFUNCN_SELECTOR(HelloLayer::GoToGameLayer));
}

/**
 * 进入游戏层
 * @param pNode
 */
void HelloLayer::GoToGameLayer(Node *pNode) {
    cocos2d::log("进入游戏层");
    ViewObject *pObject = ViewObject::create("GameLayer");
    __NotificationCenter::getInstance()->postNotification(ccNd_ViewNotify, pObject);

}
