#include "HelloWorldScene.h"
#include <random>

#define CARD_NUM 13
#define CARD_TYPE 4

#define CARD_1_POS_X 200
#define CARD_1_POS_Y 300
#define CARD_DISTANCE_X 140
#define CARD_DISTANCE_Y 200

#define BUTTON_POS_X 340
#define BUTTON_POS_Y 120

#define TIMER_POS_X 550
#define TIMER_POS_Y 120

#define ZORDER_SHOW_CARD 1
#define ZORDER_MOVING_CARD 2

#define TAG_TRUSH_CARD 11
#define TAG_BACK_CARD 12
#define TAG_TIMER_LABEL 13

#define MOVING_TIME 0.3


USING_NS_CC;
USING_NS_CC_EXT;

bool CardSprite::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    
    return true;
}

void CardSprite::onEnter()
{
    Sprite::onEnter();
    
    setTexture("card_back.png");
    
    setTag(_PosIndex.x + _PosIndex.y * 5 + 1);
    
    /*
    setTexture(getFileName(_card.type));
    
    showNumber();
    
    float PosX = CARD_1_POS_X + CARD_DISTANCE_X * _PosIndex.x;
    float PosY = CARD_1_POS_Y + CARD_DISTANCE_Y * _PosIndex.y;
    
    setPosition(PosX, PosY);
    setTag(_PosIndex.x + _PosIndex.y * 5 + 1);
     */
}

std::string CardSprite::getFileName(CardType cardType)
{
    std::string filename;
    switch (cardType) {
        case Clubs:
            filename = "card_clubs.png";
            break;
            
        case Diamonds:
            filename = "card_diamonds.png";
            break;

        case Hearts:
            filename = "card_hearts.png";
            break;

        case Spades:
            filename = "card_spades.png";
            break;
    }
    
    return filename;
}

void CardSprite::showNumber()
{
    std::string numberString;
    switch (_card.number) {
        case 1:
            numberString = "A";
            break;
            
        case 11:
            numberString = "J";
            break;
            
        case 12:
            numberString = "Q";
            break;
            
        case 13:
            numberString = "K";
            break;
            
        default:
            numberString = StringUtils::format("%d", _card.number);
            break;
    }
    
    Color4B textColor;
    switch (_card.type) {
        case Clubs:
        case Spades:
            textColor = Color4B::BLACK;
            break;
            
        case Hearts:
        case Diamonds:
            textColor = Color4B::BLACK;
            break;
    }
    
    auto number = Label::createWithSystemFont(numberString, "Arial", 96);
    number->setPosition(Point(getContentSize() / 2));
    number->setTextColor(textColor);
    addChild(number);
}

void CardSprite::moveBackToInitPos()
{
    float PosX = CARD_1_POS_X + CARD_DISTANCE_X * _PosIndex.x;
    float PosY = CARD_1_POS_Y + CARD_DISTANCE_Y * _PosIndex.y;
    auto move = MoveTo::create(MOVING_TIME, Point(PosX, PosY));
    
    runAction(move);
}

void CardSprite::moveToTrash()
{
    float PosX = CARD_1_POS_X + CARD_DISTANCE_X * 4;
    float PosY = CARD_1_POS_Y - CARD_DISTANCE_Y;
    
    auto move = MoveTo::create(MOVING_TIME, Point(PosX, PosY));
    
    auto func = CallFunc::create([&](){
        this->setTag(TAG_TRUSH_CARD);
    });
    
    auto seq = Sequence::create(move, func, NULL);
    
    runAction(seq);
}

void CardSprite::moveToInitPos()
{
    float PosX = CARD_1_POS_X + CARD_DISTANCE_X * _PosIndex.x;
    float PosY = CARD_1_POS_Y + CARD_DISTANCE_Y * _PosIndex.y;
    
    auto move = MoveTo::create(MOVING_TIME, Point(PosX, PosY));
 
    auto scale1 = ScaleTo::create(MOVING_TIME / 2, 0, 1);
    auto func1 = CallFunc::create([&](){
        setTexture(getFileName(_card.type));
        showNumber();
    });
    
    auto scale2 = ScaleTo::create(MOVING_TIME / 2, 1, 1);
    auto seq1 = Sequence::create(scale1, func1, scale2, NULL);
    
    auto spawn = Spawn::create(move, seq1, NULL);
    runAction(spawn);
}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(cocos2d::Layer::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    initGame();
    
    return true;
}

void HelloWorld::initCards()
{
    _cards.clear();
    
    for(int type = 0; type < CARD_TYPE; type++)
    {
        for (int number = 1; number <= CARD_NUM; number++) {
            Card card;
            card.number = number;
            card.type = (CardType)type;
            
            _cards.push_back(card);
        }
    }
}

Card HelloWorld::getCards()
{
    std::random_device rd;
    std::mt19937 rand = std::mt19937(rd());
    
    int index = std::uniform_int_distribution<int>(0, (int)_cards.size() - 1)(rand);
    
    auto card = _cards[index];
    
    _cards.erase(_cards.begin() + index);
    
    return card;
}

void HelloWorld::createCard(PosIndex posindex)
{
    float posX = CARD_1_POS_X;
    float posY = CARD_1_POS_Y - CARD_DISTANCE_Y;
    
    
    
//    auto card = Sprite::create("card_spades.png");
//    card->setPosition(CARD_1_POS_X + CARD_DISTANCE_X * posindex.x, CARD_1_POS_Y + CARD_DISTANCE_Y * posindex.y);
//    addChild(card, ZORDER_SHOW_CARD);
    auto card = CardSprite::create();
    card->setCard(getCards());
    card->setPosition(posX, posY);
    card->setPosIndex(posindex);
    card->moveToInitPos();
    addChild(card, ZORDER_SHOW_CARD);
    
}

void HelloWorld::showInitCards()
{
    for(int tag = 1; tag <= 10; tag++)
    {
        auto card = getChildByTag(tag);
        if(card)
        {
            card->removeFromParent();
        }
    }
    
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            PosIndex posIndex;
            posIndex.x = i;
            posIndex.y = j;
            
            createCard(posIndex);
        }
    }
}

CardSprite* HelloWorld::getTouchCard(Touch *touch)
{
    for(int tag = 1; tag <= 10; tag++)
    {
        auto card = (CardSprite*)getChildByTag(tag);
        if(card &&
           card != _firstCard &&
           card->getBoundingBox().containsPoint(touch->getLocation()))
        {
            return card;
        }
    }
    return nullptr;
}


bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    _firstCard = getTouchCard(touch);
    if(_firstCard)
    {
        _firstCard->setLocalZOrder(ZORDER_MOVING_CARD);
        
        return true;
    }
    
    return false;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
    _firstCard->setPosition(_firstCard->getPosition() + touch->getDelta());
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    bool success = false;
    
    auto _secondSprite = getTouchCard(touch);
    if(_secondSprite)
    {
        if(_firstCard->getCard().number + _secondSprite->getCard().number == 13)
        {
            success = true;
        }
    }
    else
    {
        if (_firstCard->getCard().number == 13) {
            success = true;
        }
    }
    
    if(success)
    {
        if((int)_cards.size() > 0)
        {
            createCard(_firstCard->getPosIndex());
        }
        
        _firstCard->moveToTrash();
        
        if(_secondSprite)
        {
            if((int)_cards.size() > 0)
            {
                createCard(_secondSprite->getPosIndex());
            }
            
            _secondSprite->moveToTrash();
            
            if((int)_cards.size() <= 0)
            {
                removeChildByTag(TAG_BACK_CARD);
            }
            
        }
    }
    else{
        _firstCard->moveBackToInitPos();
        _firstCard->setLocalZOrder(ZORDER_SHOW_CARD);
    }
    
    _firstCard = nullptr;
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch, unused_event);
    
}

void HelloWorld::showButton()
{
    auto button = ControlButton::create(Scale9Sprite::create("button.png"));
    button->setAdjustBackgroundImage(false);
    
    button->setPosition(BUTTON_POS_X, BUTTON_POS_Y);
    
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::onTapButton), Control::EventType::TOUCH_UP_INSIDE);
    button->setTitleForState("Start", Control::State::NORMAL);
    
    addChild(button);
}

void HelloWorld::initTrash()
{
    while(true)
    {
        auto card = getChildByTag(TAG_TRUSH_CARD);
        if(card)
        {
            card->removeFromParent();
        }
        else
        {
            break;
        }
    }
}

void HelloWorld::showBackCards()
{
    auto backcards = getChildByTag(TAG_BACK_CARD);
    
    if(!backcards)
    {
        float posX = CARD_1_POS_X;
        float posY = CARD_1_POS_Y - CARD_DISTANCE_Y;
        
        backcards = Sprite::create("card_back.png");
        backcards->setPosition(posX, posY);
        backcards->setTag(TAG_BACK_CARD);
        
        addChild(backcards, ZORDER_SHOW_CARD);
    }
}

void HelloWorld::onTapButton(Ref* sender, Control::EventType controlEvent)
{
    unscheduleUpdate();
    
    initCards();
    showInitCards();
    showBackCards();
    initTrash();
    showTimerLabel();
    
    scheduleUpdate();
}

void HelloWorld::showTimerLabel()
{
    _timer = 0;
    
    auto timerLabel = (Label*)getChildByTag(TAG_TIMER_LABEL);
    if(!timerLabel)
    {
        timerLabel = Label::createWithSystemFont("", "Arial", 48);
        timerLabel->setPosition(TIMER_POS_X, TIMER_POS_Y);
        timerLabel->setTag(TAG_TIMER_LABEL);
        addChild(timerLabel);
    }
    
    timerLabel->setString(StringUtils::format("%0.2fs", _timer));
}

void HelloWorld::update(float dt)
{
    _timer = _timer + dt;
    
    auto timerLabel = (Label*)getChildByTag(TAG_TIMER_LABEL);
    if(timerLabel)
    {
        timerLabel->setString(StringUtils::format("%0.2fs", _timer));
    }
    
    bool finish = true;
    
    for(int tag = 1; tag <= 10; tag++)
    {
        auto node = getChildByTag(tag);
        if(node)
        {
            finish = false;
            break;
        }
    }
    
    if(finish)
    {
        unscheduleUpdate();
    }
}

void HelloWorld::initGame()
{
    showBackCards();
    showTimerLabel();
    showButton();
    
    //initCards();
    //showInitCards();
}