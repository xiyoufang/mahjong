//
// Created by farmer on 2018/7/4.
//

#ifndef COCOSTUDIO_MAHJONG_UIHELPER_H
#define COCOSTUDIO_MAHJONG_UIHELPER_H
#include "cocos2d.h"

using namespace cocos2d;

class UIHelper {

public:
    /**
     * 通过Tag查找子节点
     * */
    static Node *seekNodeByTag(Node *root, int tag);
    /**
     * 通过Name查找子节点
     * @param root
     * @param name
     * @return
     */
    static Node *seekNodeByName(Node *root, const std::string &name);

    /**
     * 获取子节点
     * @param root
     * @param name
     * @param children
     * @return
     */
    static std::vector<Node*>  getChildren(Node* root, const std::string& name , std::vector<Node*>& children);
};


#endif //COCOSTUDIO_MAHJONG_UIHELPER_H
