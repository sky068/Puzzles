#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SpriteBlock.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()


class GameLayer : public cocos2d::LayerColor
{
public:
    GameLayer();
    ~GameLayer();
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
    void initGrid();
    
    void checkFinish(float dt);
    
    void randomPos();
    
    bool onTouchBegan(cocos2d::Touch *touch,cocos2d::Event *event);
   
private:
    std::string _curImgFile;
    cocos2d::LayerColor *_bgLayer;
    SpriteBlock*** _matrix;
    float _scale;
    float _height;  //方块高度
    float _width;   //方块宽度
    bool _isCanTouch;
    cocos2d::Sprite *_tipSprite;
    
    CC_SYNTHESIZE(int, _col, ColNum);
    CC_SYNTHESIZE(int, _row, RowNum);
    
    //打乱顺序
    int _nullRow;
    int _nullCol;
    int _preMove;  //上次空白格移动的标示
    bool _isBeginGame;
    int _effectKind;
    
};

#endif // __HELLOWORLD_SCENE_H__
