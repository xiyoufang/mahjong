//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_HELLOLAYER_H
#define PROJ_ANDROID_HELLOLAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BaseUILayer.h"

using namespace cocos2d;


/**
 * 欢迎层
 */
class HelloLayer : public BaseUILayer {


public:
    CREATE_FUNC(HelloLayer)
    HelloLayer();
    ~HelloLayer();

protected:
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);  //按钮事件处理
    virtual void initLayer();           //初始化层
public:
    virtual Node *GetLayer();           //获取层Layer
    void GoToGameLayer(Node* pNode);    //进入游戏层

};


#endif //PROJ_ANDROID_HELLOLAYER_H
