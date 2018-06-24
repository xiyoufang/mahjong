//
// Created by farmer on 2018/6/24.
//

#include "GameSceneManager.h"


static GameSceneManager *m_pGameSceneManager = NULL;

GameSceneManager *GameSceneManager::getInstance() {
    if (m_pGameSceneManager == NULL) {
        m_pGameSceneManager = new GameSceneManager;
    }
    return m_pGameSceneManager;
}

GameSceneManager::GameSceneManager() {
    cocos2d::log("初始化 GameSceneManager");
    this->m_pCurrentScene = NULL;
}

GameSceneManager::~GameSceneManager() {
    cocos2d::log("释放 GameSceneManager");
}

void GameSceneManager::setScene(Scene *pScene) {
    if(this->m_pCurrentScene != NULL)
    {
        Director::getInstance()->replaceScene(pScene);
    }
    else
        Director::getInstance()->runWithScene(pScene);

    this->m_pCurrentScene = pScene;
}

Scene *GameSceneManager::getScene() {
    return this->m_pCurrentScene;
}

void GameSceneManager::setRootLayer(Node* pLayer){
    this->m_pCurrentScene->removeAllChildren();   //移除子节点
    pLayer->setAnchorPoint(Vec2(0.5f,0.5f));      //居中显示
    pLayer->setPosition(getVisibleSize()/2);
    this->m_pCurrentScene->addChild(pLayer);
}

/**
 * 获取可视大小
 * @return
 */
Size GameSceneManager::getVisibleSize() {
    return Director::getInstance()->getVisibleSize();
}
