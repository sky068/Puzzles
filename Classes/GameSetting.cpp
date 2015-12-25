//
//  GameSetting.cpp
//  Puzzles
//
//  Created by xujw on 15/12/23.
//
//

#include "GameSetting.h"
#include "DataManager.h"
#include "AudioHandler.h"
#include "GameLayer.h"

GameSetting::GameSetting()
:_btnHelp(nullptr)
,_btnMusic(nullptr)
{}

bool GameSetting::init()
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,125)))
    {
        return false;
    }
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //菜单背景
    Sprite *bg = Sprite::create("menu.png");
    cocos2d::Size csize = bg->getContentSize();
    this->addChild(bg);
    bg->setPosition(origin + Vec2(visibleSize.width/2,visibleSize.height+csize.height/2));
    MoveBy *mby = MoveBy::create(0.5, Vec2(0,-csize.height+10));
    bg->runAction(EaseSineIn::create(mby));
    
    //按钮
    MenuItemFont::setFontName("arial");
    MenuItemFont::setFontSize(13);
    std::string music = "关闭音乐";
    if (!DataManager::getInstance()->getMusicOn())
    {
        music = "开启音乐";
    }
    _btnMusic = MenuItemFont::create(music, [=](Ref*p)
                                                  {
                                                      bool isOn = DataManager::getInstance()->getMusicOn();
                                                      isOn = !isOn;
                                                      if (isOn) {
                                                          AudioEngine->setBackgroundMusicVolume(1);
                                                          AudioEngine->setEffectsVolume(1);
                                                          _btnMusic->setString("关闭音乐");
                                                      }
                                                      else
                                                      {
                                                          AudioEngine->setBackgroundMusicVolume(0);
                                                          AudioEngine->setEffectsVolume(0);
                                                          _btnMusic->setString("开启音乐");
                                                      }
                                                      DataManager::getInstance()->setMusicOn(isOn);
                                                  });
    _btnMusic->setPosition(Vec2(csize.width/2-30,csize.height/2+10));
    
    std::string showFid = "显示提示";
    if (DataManager::getInstance()->getShowFid())
    {
        showFid = "关闭提示";
    }
    _btnHelp = MenuItemFont::create(showFid, [=](Ref*p)
                                                   {
                                                       bool isShow = DataManager::getInstance()->getShowFid();
                                                       isShow = !isShow;
                                                       DataManager::getInstance()->setShowFid(isShow);
                                                       if (isShow)
                                                       {
                                                           _btnHelp->setString("关闭提示");
                                                       }
                                                       else
                                                       {
                                                           _btnHelp->setString("显示提示");
                                                       }
                                                       
                                                       _gameLayer->setShowFid(isShow);
                                                       
                                                   });
    _btnHelp->setPosition(Vec2(csize.width/2+30,csize.height/2+10));
    
    MenuItemFont *btnCancle = MenuItemFont::create("取消", [=](Ref*p)
                                                   {
                                                       MoveBy *mby = MoveBy::create(0.5, Vec2(0,csize.height+10));
                                                       bg->runAction(Sequence::create(EaseSineOut::create(mby),     CallFunc::create([=](){                                                       this->removeFromParentAndCleanup(true);
                                                       }), NULL));
                                                   });
    btnCancle->setPosition(Vec2(csize.width/2,csize.height/2-20));
    
    Menu *menu = Menu::create(_btnMusic,_btnHelp,btnCancle, nullptr);
    menu->setPosition(Vec2::ZERO);
    bg->addChild(menu);
    
    //截断事件
    EventListenerTouchOneByOne * touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->setSwallowTouches(true);
    touchEvent->onTouchBegan = [=](Touch *touch,Event*event)
    {
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
    
    
    return true;
}

void GameSetting::setGameLayer(GameLayer *gml)
{
    _gameLayer = gml;
}
