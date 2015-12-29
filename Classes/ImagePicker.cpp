//
//  ImagePicker.cpp
//  Puzzles
//
//  Created by xujw on 15/12/9.
//
//

#include "ImagePicker.h"

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "ImagePickerViewController.h"
#import "RootViewController.h"
#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define JAVA_CLASS "org/cocos2dx/cpp/ImagePicker"
#define JAVA_FUNC_OPEN_PHOTO    "openPhoto"
#define JAVA_FUNC_OPEN_CAMERA   "openCamera"
#endif

ImagePicker* ImagePicker::s_instance = nullptr;
ImagePicker* ImagePicker::getInstance()
{
    if (nullptr == s_instance)
    {
        s_instance = new(std::nothrow) ImagePicker();
    }
    return s_instance;
}

void ImagePicker::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

ImagePicker::ImagePicker()
:m_callback(nullptr)
{
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("ImagePickerEvent", [=](EventCustom *event)
      {
          std::string *path = (std::string*)event->getUserData();
          if (path && m_callback != nullptr)
          {
              m_callback(*path);
          }
      });
}

void ImagePicker::callImagePickerWithPhotoAndCamera(const std::function<void (std::string)> &callback)
{
    s_instance->init();
    setListener(callback);
}

void ImagePicker::setListener(const std::function<void (std::string)> &callback)
{
    m_callback = callback;
}

void ImagePicker::removeListener()
{
    m_callback = nullptr;
}

void ImagePicker::openPhoto()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    ImagePickerViewController * imagePickerViewController = [[ImagePickerViewController alloc]initWithNibName:nil bundle:nil];
    
    RootViewController *_viewController = (RootViewController*)m_viewController;
    [_viewController.view addSubview:imagePickerViewController.view];
    [imagePickerViewController localPhoto];
    
   #endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,JAVA_CLASS,JAVA_FUNC_OPEN_PHOTO,"()V");
    if (ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
    
#endif
}
void ImagePicker::openPhoto(const std::function<void (std::string)> &callback)
{
    setListener(callback);
    openPhoto();
}

void ImagePicker::openCamera()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    ImagePickerViewController * imagePickerViewController = [[ImagePickerViewController alloc]initWithNibName:nil bundle:nil];
    
    RootViewController *_viewController = (RootViewController*)m_viewController;
    [_viewController.view addSubview:imagePickerViewController.view];
    [imagePickerViewController takePhoto];
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,JAVA_CLASS,JAVA_FUNC_OPEN_CAMERA,"()V");
    if (ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
    
#endif
}
void ImagePicker::openCamera(const std::function<void (std::string)> &callback)
{
    setListener(callback);
    openCamera();
}

bool ImagePicker::init()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor *m_layer = LayerColor::create(Color4B(0,0,0,125));
    m_layer->retain();
    
    //准备显示
    Director::getInstance()->getRunningScene()->scheduleOnce([=](float t)
    {
        Director::getInstance()->getRunningScene()->addChild(m_layer,INT_MAX);
        
        m_layer->release();
        
    }, 0.1, "ImagePickerScheduleOnce");
    
    //菜单背景
    Sprite *bg = Sprite::create("menu.png");
    cocos2d::Size csize = bg->getContentSize();
    m_layer->addChild(bg);
    bg->setPosition(origin + Vec2(visibleSize.width/2,visibleSize.height+csize.height/2));
    MoveBy *mby = MoveBy::create(0.5, Vec2(0,-csize.height+10));
    bg->runAction(EaseSineIn::create(mby));
    
    //按钮
    MenuItemFont::setFontName("arial");
    MenuItemFont::setFontSize(15);
    MenuItemFont *btnPhoto = MenuItemFont::create("相册", [=](Ref*p)
                                                  {
                                                      
                                                      openPhoto();
                                                      m_layer->removeFromParent();
                                                  });
    btnPhoto->setPosition(Vec2(csize.width/2-30,csize.height/2+10));
    
    MenuItemFont *btnCamera = MenuItemFont::create("相机", [=](Ref*p)
                                                   {
                                                       openCamera();
                                                       m_layer->removeFromParent();
                                                   });
    btnCamera->setPosition(Vec2(csize.width/2+30,csize.height/2+10));
    
    MenuItemFont *btnCancle = MenuItemFont::create("取消", [=](Ref*p)
                                                   {
                                                       MoveBy *mby = MoveBy::create(0.5, Vec2(0,csize.height+10));
                                                       bg->runAction(Sequence::create(EaseSineOut::create(mby),     CallFunc::create([m_layer](){                                                       m_layer->removeFromParentAndCleanup(true);
                                                        }), NULL));
                                                   });
    btnCancle->setPosition(Vec2(csize.width/2,csize.height/2-20));
    
    Menu *menu = Menu::create(btnPhoto,btnCamera,btnCancle, nullptr);
    menu->setPosition(Vec2::ZERO);
    bg->addChild(menu);
    
    //截断事件
    EventListenerTouchOneByOne * touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->setSwallowTouches(true);
    touchEvent->onTouchBegan = [=](Touch *touch,Event*event)
    {
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, m_layer);
    
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

void ImagePicker::setViewController(void *viewController)
{
    m_viewController = viewController;
}

#endif

//--------Java回调C++--------native 方法
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    void Java_org_cocos2dx_cpp_ImagePicker_onImageSaved(JNIEnv *env,jobject thiz,jstring path)
    {
        std::string strPath = JniHelper::jstring2string(path);
        //必须这样，否则由于线程问题会显示黑块无法正常创建精灵
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=,strPath]()mutable{
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ImagePickerEvent",&strPath);
        });
    }
}

#endif





















