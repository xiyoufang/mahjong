//
// Created by farmer on 2018/7/8.
//

#ifndef COCOSTUDIO_MAHJONG_GAMECONFIG_H
#define COCOSTUDIO_MAHJONG_GAMECONFIG_H

#include "cocos2d.h"

using namespace cocos2d;

class GameConfig {

public: //音量
    float m_EffectsVolume;

private:
    GameConfig();
    ~GameConfig();

public:
    void loadConfig();  //重新加载配置
    void saveConfig();  //保存配置

public:
    /**
     * 获取游戏配置单例
     * @return
     */
    static GameConfig *getInstance();
};


#endif //COCOSTUDIO_MAHJONG_GAMECONFIG_H
