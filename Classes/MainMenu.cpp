//
//  MainMenu.cpp
//  Puzzles
//
//  Created by xujw on 15/12/22.
//
//

#include "MainMenu.h"
#include "ui/UIButton.h"
#include "GameLayer.h"
using namespace ui;

bool MainMenu::init()
{
    if (!LayerColor::initWithColor(Color4B(0,183,239,255)))
    {
        return  false;
    }
    initMenuItems();
    
    return true;
}

void MainMenu::initMenuItems()
{
    Sprite *title = Sprite::create("title_select.png");
    title->setPosition(Vec2(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+VISIBLE_SIZE.height/6*5));
    
    Button *btnErtong = Button::create("btn_ertong.png");
    btnErtong->addClickEventListener([=](Ref*){
        Scene *scene = GameLayer::createScene(3,3);
        Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5, scene));
    });
    btnErtong -> setPosition(Vec2(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+VISIBLE_SIZE.height/6*4));
   
    Button *btnChaoqun = Button::create("btn_chaoqun.png");
    btnChaoqun->addClickEventListener([=](Ref*){
        Scene *scene = GameLayer::createScene(4,4);
        Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5, scene));
    });
    btnChaoqun -> setPosition(Vec2(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+VISIBLE_SIZE.height/6*3));
    
    Button *btnDashi = Button::create("btn_dashi.png");
    btnDashi->addClickEventListener([=](Ref*){
        Scene *scene = GameLayer::createScene(5,5);
        Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5, scene));
    });
    btnDashi -> setPosition(Vec2(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+VISIBLE_SIZE.height/6*2));
    
    Sprite *power = Sprite::create("power.png");
    power->setPosition(Vec2(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+power->getContentSize().height));

    addChild(btnErtong);
    addChild(btnChaoqun);
    addChild(btnDashi);
    addChild(power);
    addChild(title);
}

Scene* MainMenu::createMainMenuScene()
{
    Scene *scene = Scene::create();
    MainMenu *menu = MainMenu::create();
    scene->addChild(menu);
    return scene;
}