//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_GAMESCENEMANAGER_H
#define PROJ_ANDROID_GAMESCENEMANAGER_H

#include "cocos2d.h"

using namespace cocos2d;

class GameSceneManager {

private:
    //场景
    Scene *m_pCurrentScene;
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
    void setRootLayer(Node* pLayer);

    /**
     * 获取可见区域大小
     * @return
     */
    Size getVisibleSize();

    /**
     * 获取GameSceneManager单例
     * @return
     */
    static GameSceneManager *getInstance();

};


#endif //PROJ_ANDROID_GAMESCENEMANAGER_H
