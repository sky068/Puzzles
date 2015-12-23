#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SpriteBlock.h"
#include "ui/UIButton.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()


class GameLayer : public cocos2d::LayerColor
{
public:
    GameLayer();
    ~GameLayer();
    static cocos2d::Scene* createScene(int row=3,int col=3);

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameLayer);
    
    void initGrid();    //初始化格子，开始游戏
    
    void checkFinish(float dt);
    
    void randomPos();
    
    bool onTouchBegan(cocos2d::Touch *touch,cocos2d::Event *event);
   
    void setShowFid(bool flag); //是否显示提示
    
    void initMenuItems();
    
    void onCameraCallback(std::string path);
    
    void onEnter();

    void onExit();
    
    CC_SYNTHESIZE(bool, _showFid, IsShowFid);
private:
    std::string formateTimeString(long long t);
private:
    std::string _curImgFile;
    cocos2d::LayerColor *_bgLayer;
    SpriteBlock*** _matrix;
    float _scale;
    float _height;  //方块高度
    float _width;   //方块宽度
    bool _isCanTouch;
    cocos2d::Sprite *_spriteTip;
    
    CC_SYNTHESIZE(int, _col, ColNum);
    CC_SYNTHESIZE(int, _row, RowNum);
    
    //打乱顺序
    int _nullRow;
    int _nullCol;
    int _preMove;  //上次空白格移动的标示
    bool _isBeginGame;
    int _effectKind;
    
    Label *_lableTips;
        
};

#endif // __HELLOWORLD_SCENE_H__
