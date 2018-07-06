//
// Created by farmer on 2018/7/5.
//

#ifndef COCOSTUDIO_MAHJONG_IPLAYER_H
#define COCOSTUDIO_MAHJONG_IPLAYER_H

#include "GameEngine.h"
#include "GameSceneManager.h"

class IPlayer {

public:
    //玩家性别
    enum PlayerSex {
        MALE,            //男
        FEMALE           //女
    };

protected:
    bool m_Android;          //是否机器人标识
    uint8_t m_ChairID; //椅子编号
    IGameEngineEventListener *m_pGameEngineEventListener; //游戏事件监听
    PlayerSex m_Sex;     //性别
public:
    IPlayer(bool android, PlayerSex sex ,IGameEngineEventListener *pGameEngineEventListener) {
        m_Android = android;
        m_Sex = sex;
        m_pGameEngineEventListener = pGameEngineEventListener;
    };  //构造函数
    ~IPlayer() {

    }; //析构函数
    /**
     * 是否为机器人
     * @return
     */
    bool isAndroid() {
        return m_Android;
    };

    /**
     * 设置椅子
     * @param chairID
     */
    void setChairID(uint8_t chairID) {
        m_ChairID = chairID;
    }

    /**
     * 获取椅子编号
     * @return
     */
    uint8_t getChairID() {
        return m_ChairID;
    }

    /**
     * 设置监听
     * @param pGameEngineEventListener
     */
    void setGameEngineEventListener(IGameEngineEventListener *pGameEngineEventListener) {
        m_pGameEngineEventListener = pGameEngineEventListener;
    }

    /**
     * 获取监听
     * @return
     */
    IGameEngineEventListener *getGameEngineEventListener() {
        return m_pGameEngineEventListener;
    }

    /**
     * 获取性别
     * @return
     */
    PlayerSex getSex(){
        return m_Sex;
    }
};


#endif //COCOSTUDIO_MAHJONG_IPLAYER_H
