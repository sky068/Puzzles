//
//  SpriteBlock.cpp
//  Puzzles
//
//  Created by xujw on 15/12/8.
//
//

#include "SpriteBlock.h"

SpriteBlock::SpriteBlock()
:_fid(-1)
{
    
}
SpriteBlock::~SpriteBlock()
{
    
}

SpriteBlock* SpriteBlock::create(const std::string &file, cocos2d::Rect ret, int fid)
{
    auto sb = new (std::nothrow) SpriteBlock();
    if (sb && sb->initWithFile(file, ret))
    {
        sb->setFid(fid);
        sb->setAnchorPoint(Vec2(0,1));
        sb->autorelease();
        
        return  sb;
    }
    CC_SAFE_DELETE(sb);
    return nullptr;
}

void SpriteBlock::setShowFid(bool flag)
{
    if (flag)
    {
        auto label = Label::createWithSystemFont(StringUtils::format("fid:%d",this->_fid), "arial", 7);
        label->setAnchorPoint(Vec2(0,0));
        label->setColor(Color3B::BLUE);
        label->setOpacity(200);
        this->addChild(label);
    }
    else
    {
        this->removeAllChildren();
    }
}




