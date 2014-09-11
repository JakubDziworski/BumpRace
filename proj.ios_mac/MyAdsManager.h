//
//  MyAdsManager.h
//  BumpRace
//
//  Created by kuba on 04.09.2014.
//
//

#ifndef _MyAdsMang__
#define _MyAdsMang__

#import <Foundation/Foundation.h>
#import <Chartboost/Chartboost.h>
#import <StartApp/StartApp.h>
#import <StoreKit/StoreKit.h>
typedef enum
{
    BANER,
    INTERISITAL,
    MOREGAMES
} CurrAdWanted;

@interface MyAdsManager :  UIViewController <ChartboostDelegate,STADelegateProtocol,STABannerDelegateProtocol,SKProductsRequestDelegate,SKPaymentTransactionObserver>
{
     STAStartAppAd* interistialStartAppAd;
     STAStartAppAd* moreGamesStartAppAd;
    STABannerView* banner;
     CurrAdWanted adWanted;
    SKProductsRequest *_productsRequest;
}
@property (nonatomic, strong) NSArray *products;
+ (MyAdsManager*) getInstance;
- (void) preloadALL;
- (void)showInterstitial;
- (void) showMoreGames;
- (void) showBanner;
- (void) hideBanner;
- (void) openURLPage : (NSString*)str;
//in app
-(void) rmvAdsIos;
-(void) unlockLevelIos;
-(int) checkPurchasesIos;
@end
#endif
