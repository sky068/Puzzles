//
//  SpriteBlock.hpp
//  Puzzles
//
//  Created by xujw on 15/12/8.
//
//

#ifndef SpriteBlock_hpp
#define SpriteBlock_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class SpriteBlock:public Sprite
{
public:
    SpriteBlock();
    ~SpriteBlock();
    static SpriteBlock* create(const std::string&file,Rect ret,int fid);
    CC_SYNTHESIZE(int , _fid, Fid);
    void setShowFid(bool flag);
};

#endif /* SpriteBlock_hpp */
