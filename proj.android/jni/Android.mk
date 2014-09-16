LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/AIOpponent.cpp \
                   ../../Classes/Boxx.cpp \
                   ../../Classes/Checkpoint.cpp \
                   ../../Classes/EndlessHud.cpp \
                   ../../Classes/EndlessWorld.cpp \
                   ../../Classes/Globals.cpp \
                   ../../Classes/Hud.cpp \
                   ../../Classes/myLayout.cpp \
                   ../../Classes/myListView.cpp \
                   ../../Classes/MyMenu.cpp \
                   ../../Classes/Player.cpp \
                   ../../Classes/SingleElimHUD.cpp \
                   ../../Classes/SingleEliminationWorld.cpp \
                   ../../Classes/SingleHud.cpp \
                   ../../Classes/U.cpp \
                   ../../Classes/World.cpp \
                   ../../Classes/SingleGateWorld.cpp \
                   ../../Classes/VisibleRect.cpp \
                   ../../Classes/DbReader.cpp \
                   ../../Classes/PowerUp.cpp \
                   ../../Classes/soundManager.cpp \
                   ../../Classes/DialogReader.cpp \
                   ../../Classes/GlobalAdManager.cpp \
				   ../../Classes/AdsAndroidManager.cpp \
				   ../../Classes/emptyDefinitions.cpp \
				   ../../Classes/SplashScreen.cpp
				   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos/ui

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += screw_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,screw)
$(call import-module,extensions)
