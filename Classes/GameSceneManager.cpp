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
    this->m_pRootLayer = NULL;
}

GameSceneManager::~GameSceneManager() {
    cocos2d::log("释放 GameSceneManager");
}

void GameSceneManager::setScene(Scene *pScene) {
    if (this->m_pCurrentScene != NULL) {
        Director::getInstance()->replaceScene(pScene);
    } else
        Director::getInstance()->runWithScene(pScene);

    this->m_pCurrentScene = pScene;
}

Scene *GameSceneManager::getScene() {
    return this->m_pCurrentScene;
}

void GameSceneManager::setRootLayer(Node *pLayer) {
    this->m_pCurrentScene->removeAllChildren();   //移除子节点
    pLayer->setAnchorPoint(Vec2(0.5f, 0.5f));      //居中显示
    pLayer->setPosition(getVisibleSize() / 2);
    this->m_pRootLayer = pLayer;
    this->m_pCurrentScene->addChild(pLayer);
}

/**
 * 获取可视大小
 * @return
 */
cocos2d::Size GameSceneManager::getVisibleSize() {
    return Director::getInstance()->getVisibleSize();
}

/**
 * 结束游戏
 */
void GameSceneManager::end() {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/**
 * 提示框
 * @param strContent
 *  内容
 * @param autoClose
 *  点击""按钮是否自动
 * @param keep
 *  是否保持 之前的 提示框
 * @param okTarget
 *  OK回调对象
 * @param okSelector
 *  OK回调函数
 *
 */
void GameSceneManager::alert(std::string strContent, bool autoClose, bool keep, Node *okTarget, SEL_CallFuncN okSelector) {
    if (!keep) {
        removeAlertTag();
    }
    AlertLayer *pAlertLayer = AlertLayer::create();
    pAlertLayer->setAlertType(AlertLayer::ENUM_ALERT, autoClose);
    pAlertLayer->setCallback(okTarget, okSelector);
    pAlertLayer->setText(strContent);
    m_pRootLayer->addChild(pAlertLayer->GetLayer(), TAG_ALERT);
}

/**
 * 确认框
 * @param strContent
 *  内容
 * @param autoClose
 *  点击""按钮是否自动
 * @param keep
 *  是否保持 之前的 提示框
 * @param okTarget
 *  OK回调对象
 * @param okSelector
 *  OK回调函数
 * @param cancelTarget
 *  取消回调对象
 * @param cancelSelector
 *  取消回调函数
 */
void GameSceneManager::confirm(std::string strContent, bool autoClose, bool keep, Node *okTarget, SEL_CallFuncN okSelector, Node *cancelTarget, SEL_CallFuncN cancelSelector) {
    if (!keep) {
        removeAlertTag();
    }
    AlertLayer *pAlertLayer = AlertLayer::create();
    pAlertLayer->setAlertType(AlertLayer::ENUM_CONFIRM, autoClose);
    pAlertLayer->setCallback(okTarget, okSelector, cancelTarget, cancelSelector);
    pAlertLayer->setText(strContent);
    m_pRootLayer->addChild(pAlertLayer->GetLayer(), TAG_ALERT);
}

void GameSceneManager::removeAlertTag() {
    Node *pNode = UIHelper::seekNodeByTag(m_pRootLayer, TAG_ALERT);
    if (pNode != NULL) {
        pNode->removeFromParent();
    }
}
