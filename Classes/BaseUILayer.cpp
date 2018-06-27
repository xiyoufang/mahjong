//
// Created by farmer on 2018/6/27.
//

#include "BaseUILayer.h"


void BaseUILayer::setTouchEventListener(Node *pNode) {
    if (pNode) {
        const auto &arrayRootChildren = pNode->getChildren();
        for (auto &subWidget : arrayRootChildren) {
            Node *child = dynamic_cast<Node *>(subWidget);
            ui::Widget *pWidget = dynamic_cast<ui::Widget*>(child);
            if (pWidget != NULL) {
                pWidget->addTouchEventListener(CC_CALLBACK_2(BaseUILayer::onTouch, this));
            }
            if (child) {
                setTouchEventListener(child);
            }
        }
    }
}

void BaseUILayer::onTouch(Ref *ref, ui::Widget::TouchEventType eventType) {
    ui::Widget* pWidget = dynamic_cast<ui::Widget*>(ref);
    if (pWidget != NULL){
        const char* pName = pWidget->getName().c_str();
        switch (eventType){
            case ui::Widget::TouchEventType::ENDED:
                onTouchEnded(pWidget,pName);
                break;
            default:
                break;
        }
    }
}

void BaseUILayer::onTouchBegan(ui::Widget *pWidget, const char *pName) {
    log("%s",pName);
}

void BaseUILayer::onTouchMoved(ui::Widget *pWidget, const char *pName) {

}

void BaseUILayer::onTouchCanceled(ui::Widget *pWidget, const char *pName) {

}

void BaseUILayer::onTouchEnded(ui::Widget *pWidget, const char *pName) {

}
