//
// Created by farmer on 2018/7/8.
//  游戏设置层

#ifndef COCOSTUDIO_MAHJONG_SETLAYER_H
#define COCOSTUDIO_MAHJONG_SETLAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BaseUILayer.h"

using namespace cocos2d;

class SetLayer : public BaseUILayer{

private:
    ui::Slider * m_pSliderVolume;   //声音滑动条

public:
    SetLayer();
    ~SetLayer();
    CREATE_FUNC(SetLayer);
    virtual Node *GetLayer();
protected:
    virtual void initLayer();
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);

public:
    void sliderCallback(cocos2d::Ref * ref, cocos2d::ui::Slider::EventType type);   //滑动条移动事件回调
};


#endif //COCOSTUDIO_MAHJONG_SETLAYER_H
