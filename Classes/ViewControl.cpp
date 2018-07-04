//
// Created by farmer on 2018/6/24.
//

#include "ViewControl.h"
#include "ViewObject.h"
#include "HelloWorldScene.h"
#include "GameSceneManager.h"
#include "HelloLayer.h"

ViewControl::ViewControl() {
    auto scene = HelloWorld::createScene();
    GameSceneManager::getInstance()->setScene(scene);
    onViewNotify(NULL);
}

ViewControl::~ViewControl() {

}

void ViewControl::onEnter() {
    Node::onEnter();
    //观察者模式，处理界面切换
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ViewControl::onViewNotify),
                                                     ccNd_ViewNotify, NULL);

}

void ViewControl::onExit() {
    Node::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

/**
 *  界面控制
 * @param render
 */
void ViewControl::onViewNotify(Ref *render) {
    if (render == NULL){    //进入游戏
        GameSceneManager::getInstance()->setRootLayer(HelloLayer::create()->GetLayer());
        return;
    }
    ViewObject *pObject = (ViewObject *) render;
    if (pObject->m_subString == "GameView"){    //切换到游戏视图


    }

}
