//
//  AudioHandler.cpp
//  Puzzles
//
//  Created by xujw on 15/12/21.
//
//

#include "AudioHandler.h"
#include "DataManager.h"

void AudioHandler::playBgMusic(const std::string &file)
{
    if(!DataManager::getInstance()->getMusicOn())
    {
        return;
    }
    AudioEngine->playBackgroundMusic(file.c_str(),true);
}

void AudioHandler::playEffect(const std::string &file)
{
    if(!DataManager::getInstance()->getEffectOn())
    {
        return;
    }
    AudioEngine->playEffect(file.c_str());
}

void AudioHandler::stopBgMusic()
{
    AudioEngine->stopBackgroundMusic();
}