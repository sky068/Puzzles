//
//  MainMenu.hpp
//  Puzzles
//
//  Created by xujw on 15/12/22.
//
//

#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>

#include "cocos2d.h"
USING_NS_CC;

class MainMenu:public LayerColor
{
public:
    bool init();
    CREATE_FUNC(MainMenu);
    static Scene* createMainMenuScene();
private:
    void initMenuItems();
};

#endif /* MainMenu_hpp */
