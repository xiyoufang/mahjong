//
// Created by farmer on 2018/7/5.
//

#ifndef COCOSTUDIO_MAHJONG_IPLAYER_H
#define COCOSTUDIO_MAHJONG_IPLAYER_H

#include "GameEngine.h"

class IPlayer {

protected:
    bool m_Android; //是否机器人标识
    unsigned char m_ChairID; //椅子编号
    IGameEngineEventListener* m_pGameEngineEventListener; //游戏事件监听

public:
    IPlayer(bool android,IGameEngineEventListener* pGameEngineEventListener) {
        m_Android = android;
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
    void setChairID(unsigned char chairID) {
        m_ChairID = chairID;
    }
    /**
     * 获取椅子编号
     * @return
     */
    unsigned char getChairID() {
        return m_ChairID;
    }
    /**
     * 设置监听
     * @param pGameEngineEventListener
     */
    void setGameEngineEventListener(IGameEngineEventListener* pGameEngineEventListener){
        m_pGameEngineEventListener = pGameEngineEventListener;
    }
    /**
     * 获取监听
     * @return
     */
    IGameEngineEventListener* getGameEngineEventListener(){
        return m_pGameEngineEventListener;
    }
};


#endif //COCOSTUDIO_MAHJONG_IPLAYER_H
