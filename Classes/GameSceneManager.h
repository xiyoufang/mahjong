//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_GAMESCENEMANAGER_H
#define PROJ_ANDROID_GAMESCENEMANAGER_H

#include "cocos2d.h"
#include "AlertLayer.h"

#define TAG_ALERT                     10000  //提示框层次

using namespace cocos2d;

class GameSceneManager {

private:
    //场景
    Scene *m_pCurrentScene;
    //根节点
    Node *m_pRootLayer;
public:
    /**
     * 构造函数
     */
    GameSceneManager();

    /**
     * 析构函数
     */
    ~GameSceneManager();

    /**
     * 设置当前场景
     * @param pScene
     */
    void setScene(Scene *pScene);

    /**
     * 获取当前场景
     * @return
     */
    Scene *getScene();

    /**
     * 设置Layer为根节点
     * @param pLayer
     */
    void setRootLayer(Node *pLayer);

    /**
     * 提示框
     * @param strContent
     */
    void alert(std::string strContent, bool autoClose = true, bool keep = false,Node *okTarget = NULL, SEL_CallFuncN okSelector = NULL);

    /**
     * 选择框
     * @param strContent
     */
    void confirm(std::string strContent, bool autoClose = true, bool keep = false, Node *okTarget = NULL, SEL_CallFuncN okSelector = NULL, Node *cancelTarget = NULL, SEL_CallFuncN cancelSelector = NULL);

    /**
     * 移除弹出框节点
     */
    void removeAlertTag();

    /**
     * 获取可见区域大小
     * @return
     */
    cocos2d::Size getVisibleSize();

    /**
     * 结束游戏
     */
    void end();

    /**
     * 获取GameSceneManager单例
     * @return
     */
    static GameSceneManager *getInstance();

};


#endif //PROJ_ANDROID_GAMESCENEMANAGER_H
