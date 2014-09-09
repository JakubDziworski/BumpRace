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

typedef enum
{
    BANER,
    INTERISITAL,
    MOREGAMES
} CurrAdWanted;

@interface MyAdsManager :  UIViewController <ChartboostDelegate,STADelegateProtocol,STABannerDelegateProtocol>
{
     STAStartAppAd* interistialStartAppAd;
     STAStartAppAd* moreGamesStartAppAd;
    STABannerView* banner;
     CurrAdWanted adWanted;
}
+ (MyAdsManager*) getInstance;
- (void) preloadALL;
- (void)showInterstitial;
- (void) showMoreGames;
- (void) showBanner;
- (void) hideBanner;
- (void) openURLPage : (NSString*)str;

@end
#endif
