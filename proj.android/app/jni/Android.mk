LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                $(LOCAL_PATH)/../../../Classes/AIEngine.cpp \
                $(LOCAL_PATH)/../../../Classes/AIPlayer.cpp \
                $(LOCAL_PATH)/../../../Classes/AlertLayer.cpp \
                $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                $(LOCAL_PATH)/../../../Classes/BaseUILayer.cpp \
                $(LOCAL_PATH)/../../../Classes/GameEngine.cpp \
                $(LOCAL_PATH)/../../../Classes/GameLayer.cpp \
                $(LOCAL_PATH)/../../../Classes/GameLogic.cpp \
                $(LOCAL_PATH)/../../../Classes/GameSceneManager.cpp \
                $(LOCAL_PATH)/../../../Classes/HelloLayer.cpp \
                $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                $(LOCAL_PATH)/../../../Classes/RealPlayer.cpp \
                $(LOCAL_PATH)/../../../Classes/UIHelper.cpp \
                $(LOCAL_PATH)/../../../Classes/ViewControl.cpp \
                $(LOCAL_PATH)/../../../Classes/ViewObject.cpp \
                $(LOCAL_PATH)/../../../Classes/SetLayer.cpp \
                $(LOCAL_PATH)/../../../Classes/GameConfig.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
