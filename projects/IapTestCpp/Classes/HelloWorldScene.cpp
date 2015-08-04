#include "HelloWorldScene.h"

USING_NS_CC;
using namespace sdkbox;

template <typename T> std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
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
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    int winWidth = CCDirector::sharedDirector()->getWinSize().width;
    
    IAP::init();
    IAP::setDebug(true);
    IAP::setListener(this);
    
    CCMenuItemFont::setFontSize(48);
    
    _coinCount = 0;
    
    _txtCoin = CCLabelTTF::create("0", "Arial", 32);
    _txtCoin->setAnchorPoint(cocos2d::CCPoint(0, 0));
    _txtCoin->setPosition(cocos2d::CCPoint(800, 500));
    addChild(_txtCoin);
    
    CCMenuItemFont *item = CCMenuItemFont::create("Load", this, menu_selector(HelloWorld::onRequestIAP));
    item->setPosition(0, 0);
    CCMenuItemFont *item2 = CCMenuItemFont::create("Restore", this, menu_selector(HelloWorld::onRestoreIAP));
    item->setPosition(0, 100);
    
    CCMenu *menu = CCMenu::create(item, item2, NULL);
    menu->setAnchorPoint(cocos2d::CCPoint(0, 0));
    menu->setPosition(cocos2d::CCPoint(200, 400));
    addChild(menu);
    
    _iapMenu = CCMenu::create();
    _iapMenu->setPosition(winWidth/2, 200);
    addChild(_iapMenu);
    
    //Close menu
    CCMenuItemFont* close_menu_item = CCMenuItemFont::create("Close", this, menu_selector(HelloWorld::menuCloseCallback));
    CCMenu *close_menu = CCMenu::create(close_menu_item, NULL);
    close_menu->setAnchorPoint(cocos2d::CCPoint(0, 0));
    close_menu->setPosition(cocos2d::CCPoint(800, 100));
    addChild(close_menu);
    
    
    return true;
}

void HelloWorld::onShowAds(CCObject* sender)
{
    CCLOG("Show Ads");
}

void HelloWorld::onRequestIAP(CCObject* sender)
{
    IAP::refresh();
}

void HelloWorld::onRestoreIAP(CCObject* sender)
{
    IAP::restore();
}

void HelloWorld::onIAP(CCObject* sender)
{
    CCNode* btn = static_cast<CCNode*>(sender);
    Product* p = (Product*)btn->getUserData();
    
    CCLOG("Start IAP %s", p->name.c_str());
    IAP::purchase(p->name);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::onInitialized(bool success)
{
    CCLOG("IAP initialized");
}

void HelloWorld::onSuccess(const Product &p)
{
    if (p.id == "com.cocos2dx.plugintest3") {
        _coinCount += 1000;
        _txtCoin->setString(tostr(_coinCount).c_str());
    }
    else if (p.id == "com.cocos2dx.plugintest2") {
        _coinCount += 5000;
        _txtCoin->setString(tostr(_coinCount).c_str());
    }
    else if (p.id == "com.cocos2dx.non1") {
        //Remove ads
    }
    
    CCLOG("Purchase Success: %s", p.id.c_str());
}

void HelloWorld::onFailure(const Product &p, const std::string &msg)
{
    CCLOG("Purchase Failed: %s", msg.c_str());
}

void HelloWorld::onCanceled(const Product &p)
{
    CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void HelloWorld::onRestored(const Product& p)
{
    CCLOG("Purchase Restored: %s", p.name.c_str());
}

void HelloWorld::updateIAP(const std::vector<sdkbox::Product>& products)
{
    //
    _iapMenu->removeAllChildren();
    _products = products;
    
    int posX = 0;
    int posY = 0;
    
    for (int i=0; i < _products.size(); i++)
    {
        CCLOG("iap: %s", _products[i].name.c_str());
        
        CCMenuItemFont* btn = CCMenuItemFont::create(_products[i].name.c_str(), this, menu_selector(HelloWorld::onIAP) );
        btn->setColor(ccWHITE);
        btn->setUserData(&_products[i]);
        btn->setPosition(CCPoint(posX, posY));
        _iapMenu->addChild(btn);
        posY += 50;
    }
}

void HelloWorld::onProductRequestSuccess(const std::vector<Product> &products)
{
    updateIAP(products);
}

void HelloWorld::onProductRequestFailure(const std::string &msg)
{
    CCLOG("Fail to load products");
}
