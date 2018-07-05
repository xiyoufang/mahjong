//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_VIEWCONTROL_H
#define PROJ_ANDROID_VIEWCONTROL_H

#include "cocos2d.h"

#define ccNd_ViewNotify "ccNd_ViewNotify"

using namespace cocos2d;

/**
 * 视图控制类
 */
class ViewControl : public Node{

public:
    ViewControl();
    ~ViewControl();

    /**
     * 通知到观察者
     * @param render
     */
    void onViewNotify(Ref* render);

};


#endif //PROJ_ANDROID_VIEWCONTROL_H
