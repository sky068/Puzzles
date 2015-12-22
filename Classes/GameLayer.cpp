#include "GameLayer.h"
#include "ImagePicker.h"
#include "AudioHandler.h"
USING_NS_CC;

GameLayer::GameLayer()
:_curImgFile("demo.jpg")
,_bgLayer(nullptr)
,_row(3)
,_col(3)
,_scale(1)
,_height(0)
,_width(0)
,_isCanTouch(true)
,_tipSprite(nullptr)
,_nullCol(-1)
,_nullRow(-1)
,_preMove(-1)
,_isBeginGame(false)
,_effectKind(-1)
{
    
}
GameLayer::~GameLayer()
{
    for(int i=0; i<_row; i++)
    {
        delete [] _matrix[i];
    }
}

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B::GREEN))
    {
        return false;
    }

    auto selectImg = MenuItemFont::create("选取图片", [this](Ref*p)
    {
        ImagePicker::getInstance()->callImagePickerWithPhotoAndCamera([=](std::string path)
          {
              CCLOG("~~~~~writablePaht:%s",FileUtils::getInstance()->getWritablePath().c_str());
              CCLOG("~~~~~filepath:%s",path.c_str());
              bool isFileExist = FileUtils::getInstance()->isFileExist(path);
              if (isFileExist) {
                  CCLOG("~~~~图片文件存在！");
//                  Director::getInstance()->getTextureCache()->addImage(path);
              }else{
                  CCLOG("~~~~~图片文件不存在！");
              }
              _curImgFile = path;
              for(int i=0; i<_row; i++)
              {
                  delete [] _matrix[i];
              }
              CCLOG("~~~~texture:%s",Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
              
              this->initGrid();
          });
    });
    selectImg->setPosition(Vec2(ORIGIN.x + VISIBLE_SIZE.width - selectImg->getContentSize().width/2 ,
                                ORIGIN.y + selectImg->getContentSize().height/2+30));
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(ORIGIN.x + VISIBLE_SIZE.width - closeItem->getContentSize().width/2 ,
                                ORIGIN.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem,selectImg, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    auto lis = EventListenerTouchOneByOne::create();
    lis->onTouchBegan = std::bind(&GameLayer::onTouchBegan, this,std::placeholders::_1,std::placeholders::_2);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, this);
    
    initGrid();
    
    return true;
}

bool GameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!_isCanTouch)
    {
        return false;
    }
    _isBeginGame = true;
    
    for (int row=0; row<_row; row++)
    {
        for (int col=0; col<_col; col++)
        {
            SpriteBlock *spb = _matrix[row][col];
            if (spb && spb->getBoundingBox().containsPoint(_bgLayer->convertToNodeSpace(touch->getLocation())))
            {
                switch (_effectKind)
                {
                    case 0:
                        AudioHandler::playEffect(kEffect_Touch);
                        break;
                    case 1:
                        AudioHandler::playEffect(kEffect_Touch2);
                        break;
                    default:
                        break;
                }
                
                //检测上下左右是否有空的
                int tmpRow = row;
                int tmpRow2 = row;
                int tmpCol = col;
                int tmpCol2 = col;
                //上
                if (--tmpRow>=0 && _matrix[tmpRow][col]==nullptr)
                {
                    _isCanTouch = false;
                    _matrix[tmpRow][col] = spb;
                    _matrix[++tmpRow][col] = nullptr;
                    
                    auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX(),spb->getPositionY() + (_height+1)*_scale));
                    auto cfc = CallFunc::create([this](){checkFinish(0);});
                    spb->runAction(Sequence::create(mto,DelayTime::create(0.1f),cfc, NULL));
                    return true;
                }
                //下
                else if (++tmpRow2<_row && _matrix[tmpRow2][col]==nullptr)
                {
                    _isCanTouch = false;
                    _matrix[tmpRow2][col] = spb;
                    _matrix[--tmpRow2][col] = nullptr;
                    
                    auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX(),spb->getPositionY() - (_height+1)*_scale));
                    auto cfc = CallFunc::create([this](){checkFinish(0);});
                    spb->runAction(Sequence::create(mto,DelayTime::create(0.1f),cfc, NULL));
                    return true;
                }
                //左
                else if (--tmpCol>=0 && _matrix[row][tmpCol]==nullptr)
                {
                    _isCanTouch = false;
                    _matrix[row][tmpCol] = spb;
                    _matrix[row][++tmpCol] = nullptr;
                    
                    auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX() - (_width+1)*_scale,spb->getPositionY()));
                    auto cfc = CallFunc::create([this](){checkFinish(0);});
                    spb->runAction(Sequence::create(mto,DelayTime::create(0.1f),cfc, NULL));
                    return true;
                }
                //右
                else if (++tmpCol2<_col && _matrix[row][tmpCol2]==nullptr)
                {
                    _isCanTouch = false;
                    _matrix[row][tmpCol2] = spb;
                    _matrix[row][--tmpCol2] = nullptr;
                    
                    auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX() + (_width+1)*_scale,spb->getPositionY()));
                    auto cfc = CallFunc::create([this](){checkFinish(0);});
                    spb->runAction(Sequence::create(mto,DelayTime::create(0.1f),cfc, NULL));
                    return true;
                }
                
            }
        }
    }
    
    return true;
}

void GameLayer::initGrid()
{
    CCLOG("------------------------------");
    CCLOG("%s",Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    
    _effectKind = arc4random() % 2;
    
    if (_bgLayer) {
        _bgLayer->removeFromParent();
        _bgLayer = nullptr;
        _isBeginGame = false;
    }
    _bgLayer = LayerColor::create(Color4B::WHITE, VISIBLE_SIZE.width, VISIBLE_SIZE.width);
    _bgLayer->setPosition(ORIGIN.x,ORIGIN.y+VISIBLE_SIZE.height/2-VISIBLE_SIZE.width/2);
    this->addChild(_bgLayer);

    //----add tipsprite 并计算宽高缩放系数----
    if (_tipSprite)
    {
        _tipSprite->removeFromParent();
        _tipSprite = nullptr;
    }
    _tipSprite = Sprite::create(_curImgFile);
    this->addChild(_tipSprite,20);
    _tipSprite->setVisible(false);
    _tipSprite->setPosition(ORIGIN.x+VISIBLE_SIZE.width/2,ORIGIN.y+VISIBLE_SIZE.height/2);
    
    int min = MIN(_tipSprite->getContentSize().width, _tipSprite->getContentSize().height);
    _scale = VISIBLE_SIZE.width / min;
    
    _width = (float)(min-_col-1) / _col;
    _height = (float)(min-_row-1) / _row;
    
    //-----初始化容器-----
    _matrix = new SpriteBlock**[_row];
    for (int row=0; row<_row; row++)
    {
        _matrix[row] = new SpriteBlock*[_col];
    }
    for (int row=0; row<_row; row++)
    {
        for (int col=0; col<_col; col++)
        {
            _matrix[row][col] = nullptr;
        }
    }

    int fid = 0;
    for (int row=0; row<_row; row++)
    {
        for (int col=0; col<_col; col++)
        {
            if (row==(_row-1) && col==(_col-1))
            {
                //释放没用的纹理
                scheduleOnce([this](float dt)
                             {
                                 Director::getInstance()->getTextureCache()->removeUnusedTextures();
                             }, 0.5, "removeUnused");
                _nullRow = _row-1;
                _nullCol = _col-1;
                
                randomPos();

                return;
            }
            SpriteBlock* spb = SpriteBlock::create(_curImgFile, Rect(col*_width, row*_height,_width , _height),fid);
            spb->setPosition((1+col*(_width+1))*_scale,(_row-row)*(_height+1)*_scale);
            spb->setScale(_scale);
            _bgLayer->addChild(spb);
            _matrix[row][col] = spb;
            fid ++;
        }
    }
}

void GameLayer::checkFinish(float dt)
{
    _isCanTouch= true;

    int length = _row*_col - 1;
    std::vector<SpriteBlock*> allSpb;
   
    for (int row=0; row< _row; row++)
    {
        for (int col=0; col< _col; col++)
        {
            allSpb.push_back(_matrix[row][col]);
        }
    }
    
    bool isSuc = true;
    for (int i=0; i<(length-1); i++)
    {
        SpriteBlock *spb = allSpb.at(i);
        SpriteBlock *spbNext = allSpb.at(i+1);
        if (!spb || !spbNext || spb->getFid()>spbNext->getFid())
        {
            isSuc = false;
        }
    }
    if (isSuc)
    {
        AudioHandler::playEffect(kEffect_Success);
        MessageBox("success", "ok");
    }
    
}

void GameLayer::randomPos()
{
    if (_isBeginGame)
    {
        return;
    }
    int v = arc4random() % 4;
    switch (v) {
        case 0:
        {
            //空白格子上移
            int row = _nullRow - 1;
            int col = _nullCol;
            if (1==_preMove || row<0)
            {
                randomPos();
            }
            else
            {
                _preMove = 0;
                SpriteBlock *spb = _matrix[row][col];
                _matrix[_nullRow][_nullCol] = spb;
                _matrix[row][col] = nullptr;
                _nullRow = row;
                _nullCol = col;
                auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX(),spb->getPositionY() - (_height+1)*_scale));
                auto cfc = CallFunc::create([this](){this->randomPos();});
                spb->runAction(Sequence::create(mto,cfc, NULL));
            }
        }
            break;
        case 1:
        {
            //空白格子下移
            int row = _nullRow + 1;
            int col = _nullCol;
            if (0==_preMove || row>=_row)
            {
                randomPos();
            }
            else
            {
                _preMove = 1;
                SpriteBlock *spb = _matrix[row][col];
                _matrix[_nullRow][_nullCol] = spb;
                _matrix[row][col] = nullptr;
                _nullRow = row;
                _nullCol = col;
                auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX(),spb->getPositionY() + (_height+1)*_scale));
                auto cfc = CallFunc::create([this](){this->randomPos();});
                spb->runAction(Sequence::create(mto,cfc, NULL));
            }
        }
            break;
        case 2:
        {
            //空白格子左移
            int row = _nullRow;
            int col = _nullCol - 1 ;
            if (3==_preMove || col<0)
            {
                randomPos();
            }
            else
            {
                _preMove = 2;
                SpriteBlock *spb = _matrix[row][col];
                _matrix[_nullRow][_nullCol] = spb;
                _matrix[row][col] = nullptr;
                _nullRow = row;
                _nullCol = col;
                auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX() + (_width+1)*_scale,spb->getPositionY()));
                auto cfc = CallFunc::create([this](){this->randomPos();});
                spb->runAction(Sequence::create(mto,cfc, NULL));
            }
        }
            break;
        case 3:
        {
            //空白格子右移
            int row = _nullRow;
            int col = _nullCol + 1 ;
            if (2==_preMove || col>=_col)
            {
                randomPos();
            }
            else
            {
                _preMove = 3;
                SpriteBlock *spb = _matrix[row][col];
                _matrix[_nullRow][_nullCol] = spb;
                _matrix[row][col] = nullptr;
                _nullRow = row;
                _nullCol = col;
                auto mto = MoveTo::create(0.1f, Vec2(spb->getPositionX() - (_width+1)*_scale,spb->getPositionY()));
                auto cfc = CallFunc::create([this](){this->randomPos();});
                spb->runAction(Sequence::create(mto,cfc, NULL));
            }

        }
            break;
            
        default:
            break;
    }
}

void GameLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
