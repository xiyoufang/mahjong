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

    //Error:(23, 17) allocating an object of abstract class type 'HelloLayer'

public:
    CREATE_FUNC(HelloLayer)
    HelloLayer();
    ~HelloLayer();
    void onEnter() override;
    void onExit() override;
    bool init() override;
    virtual Node *createLayer() override ;

protected:
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);

};


#endif //PROJ_ANDROID_HELLOLAYER_H
