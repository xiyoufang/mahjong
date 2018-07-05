//
// Created by farmer on 2018/7/4.
//

#include "UIHelper.h"


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
