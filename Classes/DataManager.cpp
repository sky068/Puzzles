//
//  DataManager.cpp
//  Puzzles
//
//  Created by xujw on 15/12/22.
//
//

#include "DataManager.h"

DataManager::DataManager()
{}

DataManager* DataManager::s_instance = nullptr;

DataManager* DataManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new DataManager();
    }
    
    return s_instance;
}

void DataManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

void DataManager::readData()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + "puzzles.plist";
    _dataMap = FileUtils::getInstance()->getValueMapFromFile(path);
    if (0 == _dataMap.size())
    {
        initData();
    }
}

void DataManager::setMusicOn(bool flag)
{
    _dataMap[kMusicFlag] = Value(flag);
}
void DataManager::setShowFid(bool flag)
{
    _dataMap[kShowFidFlag] = Value(flag);
}

bool DataManager::getMusicOn()
{
    return _dataMap[kMusicFlag].asBool();
}

bool DataManager::getShowFid()
{
    return _dataMap[kShowFidFlag].asBool();
}

bool DataManager::getEffectOn()
{
    return _dataMap[kEffectFlag].asBool();
}

void DataManager::initData()
{
    _dataMap[kMusicFlag] = Value(true);
    _dataMap[kEffectFlag] = Value(true);
    _dataMap[kShowFidFlag] = Value(false);
}

void DataManager::saveData()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + "puzzles.plist";
    bool res = FileUtils::getInstance()->writeValueMapToFile(_dataMap, path);
    CCASSERT(res, "save data some wrong!");
}