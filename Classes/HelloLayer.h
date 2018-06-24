//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_HELLOLAYER_H
#define PROJ_ANDROID_HELLOLAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

/**
 * 欢迎层
 */
class HelloLayer : public Layer {

private:
    Node* m_pLayer; //层节点
public:
    CREATE_FUNC(HelloLayer)
    HelloLayer();
    ~HelloLayer();
    void onEnter() override;
    void onExit() override;
    bool init() override;;
public:
    Node* getLayer();
};


#endif //PROJ_ANDROID_HELLOLAYER_H
