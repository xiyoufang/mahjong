//
// Created by farmer on 2018/7/4.
//

#include "UIHelper.h"


/**
 * 根据Tag获取节点
 * @param root
 * @param tag
 * @return
 */
Node *UIHelper::seekNodeByTag(Node *root, int tag) {
    if (!root) {
        return nullptr;
    }
    if (root->getTag() == tag) {
        return root;
    }
    const auto &arrayRootChildren = root->getChildren();
    ssize_t length = arrayRootChildren.size();
    for (ssize_t i = 0; i < length; i++) {
        Node *child = dynamic_cast<Node *>(arrayRootChildren.at(i));
        if (child) {
            Node *res = seekNodeByTag(child, tag);
            if (res != nullptr) {
                return res;
            }
        }
    }
    return nullptr;
}

/**
 * 根据名字搜索节点
 * @param root
 * @param name
 * @return
 */
Node *UIHelper::seekNodeByName(Node *root, const std::string &name) {
    if (!root) {
        return nullptr;
    }
    if (root->getName() == name) {
        return root;
    }
    const auto &arrayRootChildren = root->getChildren();
    for (auto &subWidget : arrayRootChildren) {
        Node *child = dynamic_cast<Node *>(subWidget);
        if (child) {
            Node *res = seekNodeByName(child, name);
            if (res != nullptr) {
                return res;
            }
        }
    }
    return nullptr;
}

/**
 * 获取全部指定名字子节点
 * @param root
 * @param name
 * @param children
 * @return
 */
std::vector<Node *> UIHelper::getChildren(Node *root, const std::string &name, std::vector<Node *> &children) {
    if (!root) {
        return children;
    }
    if (root->getName() == name) {
        children.push_back(root);
    }
    const auto &arrayRootChildren = root->getChildren();
    for (auto &subWidget : arrayRootChildren) {
        Node *child = dynamic_cast<Node *>(subWidget);
        if (child) {
            getChildren(child, name, children);
        }
    }
    return children;
}