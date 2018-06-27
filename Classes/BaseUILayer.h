//
// Created by farmer on 2018/6/27.
//

#ifndef COCOSTUDIO_MAHJONG_BASEUILAYER_H
#define COCOSTUDIO_MAHJONG_BASEUILAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

/**
 * UI Layer 基类
 */
class BaseUILayer : public Layer {

protected:
    /**
     * Cocostudio UI节点
     */
    Node *m_pLayer; //层节点

    /**
     * 设置触屏监听
     * @param pNode
     */
    void setTouchEventListener(Node *pNode);

    /**
     * 触屏事件
     * @param ref
     * @param eventType
     */
    void onTouch(Ref *ref, ui::Widget::TouchEventType eventType);

    /**
    * 触屏开始
    * @param pWidget
    * @param pName
    */
    virtual void onTouchBegan(ui::Widget *pWidget, const char *pName);

    /**
     * 触屏移动
     * @param pWidget
     * @param pName
     */
    virtual void onTouchMoved(ui::Widget *pWidget, const char *pName);

    /**
     * 触屏取消
     * @param pWidget
     * @param pName
     */
    virtual void onTouchCanceled(ui::Widget *pWidget, const char *pName);

    /**
     * 触屏结束
     * @param pWidget
     * @param pName
     */
    virtual void onTouchEnded(ui::Widget *pWidget, const char *pName);

public:

    virtual Node *createLayer() = 0;

};


#endif //COCOSTUDIO_MAHJONG_BASEUILAYER_H
