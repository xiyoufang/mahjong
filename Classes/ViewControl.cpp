//
// Created by farmer on 2018/6/24.
//

#include "ViewControl.h"
#include "ViewObject.h"
#include "HelloWorldScene.h"
#include "GameSceneManager.h"
#include "HelloLayer.h"
#include "GameLayer.h"

ViewControl::ViewControl() {
    auto scene = HelloWorld::createScene();
    GameSceneManager::getInstance()->setScene(scene);
    onViewNotify(NULL);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ViewControl::onViewNotify), ccNd_ViewNotify, NULL);
}

ViewControl::~ViewControl() {
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
    if (pObject->m_MainString == VIEW_SWITCH_MAIN_LAYER){
        if (pObject->m_subString == "GameLayer"){    //切换到游戏视图
            GameSceneManager::getInstance()->setRootLayer(GameLayer::create()->GetLayer());
        }
    }

}
