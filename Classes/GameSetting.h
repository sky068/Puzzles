//
//  GameSetting.hpp
//  Puzzles
//
//  Created by xujw on 15/12/23.
//
//

#ifndef GameSetting_hpp
#define GameSetting_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class GameLayer;

class GameSetting:public LayerColor
{
public:
    GameSetting();
    bool init();
    CREATE_FUNC(GameSetting);
    void setGameLayer(GameLayer *gml);
private:
    MenuItemFont *_btnMusic;
    MenuItemFont *_btnHelp;
    GameLayer *_gameLayer;
};

#endif /* GameSetting_hpp */
