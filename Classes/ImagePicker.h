//
//  ImagePicker.hpp
//  Puzzles
//
//  Created by xujw on 15/12/9.
//  TopGame
/*  跨平台调用桥梁 图片选择器
 *  iOS平台使用时把ImagePicker.cpp改为ImagePicker.mm 安卓平台使用.cpp
 *  iOS平台在AppController.mm里调用setViewController(void* viewController)把rootView传进来初始化m_viewController
 */

#ifndef ImagePicker_hpp
#define ImagePicker_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

class ImagePicker
{
public:
    static ImagePicker* getInstance();
    //销毁
    static void destroyInstance();
public:
    //显示本地相册与相机选择器
    void callImagePickerWithPhotoAndCamera(const std::function<void(std::string)> & callback);
    
    //设置监听
    void setListener(const std::function<void(std::string)> & callback);
    
    //移除监听
    void removeListener();
    
    //打开相机
    void openCamera();
    
    void openCamera(const std::function<void(std::string)> &callback);
    
    //打开相册
    void openPhoto();
    
    void openPhoto(const std::function<void(std::string)> &callback);
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //设置AppController iOS平台在AppController.mm里调用
    void setViewController(void* viewController);
#endif
    
    
protected:
    bool init();
    
    ImagePicker();
    
protected:
    std::function<void(std::string)> m_callback;
    static ImagePicker *s_instance;
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void* m_viewController;
#endif
};















#endif /* ImagePicker_hpp */
