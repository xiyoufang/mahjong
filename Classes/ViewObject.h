//
// Created by farmer on 2018/6/24.
//

#ifndef PROJ_ANDROID_VIEWOBJECT_H
#define PROJ_ANDROID_VIEWOBJECT_H

#include "cocos2d.h"

using namespace cocos2d;

#define VIEW_SWITCH_MAIN_LAYER "SwitchMainLayer" //切换主界面


/**
 * 视图控制用来传递的对象
 */
class ViewObject : public Ref {

public:
    std::string m_MainString;   //主指令
    std::string m_subString;    //子指令
public:
    void *m_pObjectData;        //数据
    unsigned int m_iDataSize;   //数据大小
    bool init();                //初始化

public:

    /**
     * 创建
     * @param mainString
     *  主指令
     * @param subString
     *  子指令
     * @return
     */
    static ViewObject *create(std::string subString);

    /**
     * 创建
     * @param mainString
     *  主指令
     * @param subString
     *  子指令
     * @return
     */
    static ViewObject *create(std::string mainString, std::string subString);

    /**
     * 创建
     * @param mainString
     *  主指令
     * @param subString
     *  子指令
     * @param pData
     *  数据指针
     * @param iSize
     *  数据大小
     * @return
     */
    static ViewObject *create(std::string mainString, std::string subString, void *pData, unsigned int iSize);

    CREATE_FUNC(ViewObject);    //自动管理内存
};


#endif //PROJ_ANDROID_VIEWOBJECT_H
