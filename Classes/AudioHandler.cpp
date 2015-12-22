//
//  AudioHandler.cpp
//  Puzzles
//
//  Created by xujw on 15/12/21.
//
//

#include "AudioHandler.h"

void AudioHandler::playBgMusic(const std::string &file)
{
    AudioEngine->playBackgroundMusic(file.c_str());
}

void AudioHandler::playEffect(const std::string &file)
{
    AudioEngine->playEffect(file.c_str());
}