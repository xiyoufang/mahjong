//
// Created by farmer on 2018/7/8.
//

#include "GameConfig.h"

static GameConfig* m_pGameConfig = NULL;

GameConfig *GameConfig::getInstance() {     //单例创建全局唯一的GameConfig
    if (m_pGameConfig == NULL){
        m_pGameConfig = new GameConfig();
    }
    return m_pGameConfig;
}

GameConfig::GameConfig() {
    m_EffectsVolume = 0;
    loadConfig();
}

GameConfig::~GameConfig() {

}

/**
 * 加载配置
 */
void GameConfig::loadConfig() {
    m_EffectsVolume = UserDefault::getInstance()->getFloatForKey("EffectsVolume", 0.8);
}

/**
 * 保存配置
 */
void GameConfig::saveConfig() {
    UserDefault::getInstance()->setFloatForKey("EffectsVolume", m_EffectsVolume);
}

