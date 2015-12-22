//
//  AudioHandler.hpp
//  Puzzles
//
//  Created by xujw on 15/12/21.
//
//

#ifndef AudioHandler_hpp
#define AudioHandler_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define AudioEngine (SimpleAudioEngine::getInstance())

#define kMusic_Bg "sounds/bground.mp3"
#define kEffect_Touch "sounds/touch.mp3"
#define kEffect_Touch2 "sounds/touch2.mp3"
#define kEffect_Success "sounds/success.mp3"


class AudioHandler
{
public:
    static void playBgMusic(const std::string &file);
    static void playEffect(const std::string &file);
};

#endif /* AudioHandler_hpp */
