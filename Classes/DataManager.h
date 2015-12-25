//
//  DataManager.hpp
//  Puzzles
//
//  Created by xujw on 15/12/22.
//
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

#define kMusicFlag "musicOn"
#define kEffectFlag "effectOn"
#define kShowFidFlag "isShowFid"

class DataManager
{
public:
    DataManager();
    static DataManager* getInstance();
    static void destroyInstance();
    void saveData();
    void readData();
    bool getMusicOn();
    bool getEffectOn();
    bool getShowFid();
    void setMusicOn(bool flag);
    void setShowFid(bool flag);
    
private:
    void initData();
    
private:
    static DataManager *s_instance;
    ValueMap _dataMap;
};



#endif /* DataManager_hpp */
