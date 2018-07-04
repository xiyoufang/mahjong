//
// Created by farmer on 2018/7/4.
//

#ifndef COCOSTUDIO_MAHJONG_ALERTLAYER_H
#define COCOSTUDIO_MAHJONG_ALERTLAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BaseUILayer.h"
#include "UIHelper.h"

using namespace cocos2d;

/**
 * 提示框层
 */
class AlertLayer : public BaseUILayer {

private:
    cocos2d::Ref *m_pOKSelectorTarget;          //OK回调对象
    cocos2d::Ref *m_pCancelSelectorTarget;      //Cancel回调对象
    SEL_CallFuncN m_OKSelector;                 //OK回调方法
    SEL_CallFuncN m_CancelSelector;             //Cancel回调方法
    bool m_autoClose;                           //自动关闭窗口

public:
    //提示类型
    enum AlertTypeEnum {
        ENUM_ALERT = 0,    //一个确定按钮
        ENUM_CONFIRM = 1,    //确定按钮+取消按钮
    };
public:
    CREATE_FUNC(AlertLayer)

    AlertLayer();

    ~AlertLayer();

protected:
    virtual void initLayer();
    //设置回调
public:
    void setAlertType(AlertTypeEnum typeEnum, bool autoClose = true);    //设置提示框类型
    void setText(std::string content ,std::string title = "提示");   //设置内容
    void setCallback(cocos2d::Node *okTarget, SEL_CallFuncN okSelector, cocos2d::Node *cancelTarget = NULL, SEL_CallFuncN cancelSelector = NULL);   //设置回调
    virtual Node *GetLayer();   //获取Layer


protected:
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);  //按钮事件

};


#endif //COCOSTUDIO_MAHJONG_ALERTLAYER_H
