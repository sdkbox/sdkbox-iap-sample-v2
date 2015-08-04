#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginIAP/PluginIAP.h"

class HelloWorld : public cocos2d::CCLayer, public sdkbox::IAPListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void onShowAds(CCObject* sender);
    void onRequestIAP(CCObject* sender);
    void onRestoreIAP(CCObject* sender);
    void onIAP(CCObject* sender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    void updateIAP(const std::vector<sdkbox::Product>& products);
    
    virtual void onInitialized(bool success);
    
    virtual void onSuccess(sdkbox::Product const& p);
    
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg);
    
    virtual void onCanceled(sdkbox::Product const& p);
    
    virtual void onRestored(sdkbox::Product const& p);
    
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products);
    
    virtual void onProductRequestFailure(const std::string &msg);
    
    cocos2d::CCMenu* _iapMenu;
    std::vector<sdkbox::Product> _products;
    cocos2d::CCLabelTTF* _txtCoin;
    int _coinCount;
};

#endif // __HELLOWORLD_SCENE_H__