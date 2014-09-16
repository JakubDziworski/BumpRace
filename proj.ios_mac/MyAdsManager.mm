//
//  MyAdsManager.m
//  BumpRace
//
//  Created by kuba on 04.09.2014.
//
//

#import "MyAdsManager.h"
#import <CommonCrypto/CommonDigest.h>
#import <StartApp/StartApp.h>
#include "GlobalAdManager.h"
#import "Flurry.h"

@implementation MyAdsManager

NSString *const rmvAdsID =@"com.frontmob.removeAdsBumpRace";
NSString *const unlckLevelsID=@"com.frontmob.buyLevelsBumpRace";

+ (MyAdsManager*) getInstance
{
    static MyAdsManager* me = nil;
    if(me == nil) me = [[MyAdsManager alloc] init];
    return me;
}
- (void) reinitInterestial
{
    interistialStartAppAd = [[STAStartAppAd alloc]  init];
    [interistialStartAppAd loadAd:STAAdType_FullScreen withDelegate:self];
}
- (void) reinitMoreGames
{
    moreGamesStartAppAd = [[STAStartAppAd alloc] init];
    [moreGamesStartAppAd loadAd:STAAdType_OfferWall withDelegate:self];
}
- (void) preloadALL
{
    //START APP
    [self fetchProducts];
    STAStartAppSDK* sdk = [STAStartAppSDK sharedInstance];
    sdk.appID = @"209130774";
    sdk.devID = @"109722583";
    interistialStartAppAd = [[STAStartAppAd alloc] init];
    [interistialStartAppAd loadAd:STAAdType_FullScreen withDelegate:self];
    moreGamesStartAppAd = [[STAStartAppAd alloc] init];
    [moreGamesStartAppAd loadAd:STAAdType_OfferWall withDelegate:self];
    //CHARTBOOST
    [Chartboost startWithAppId:@"540d9f5989b0bb78131a0acc"
                  appSignature:@"9d5d610e2b5a7749df0e54054f3a4df9090a4a28"
                      delegate:self];
    [Chartboost cacheMoreApps:CBLocationHomeScreen];
    [Flurry setCrashReportingEnabled:YES];
    [Flurry startSession:@"G4YR337XZDM6PH8DXYQ6"];
}
- (void) openURLPage : (NSString*) str
{
    NSURL *url = [NSURL URLWithString : str ];
    if([[UIApplication sharedApplication] canOpenURL:(url)])
    {
        [[UIApplication sharedApplication] openURL: url];
    }
    else
    {
        [self openFbPage : str];
    }
}
- (void) openFbPage : (NSString*) str
{
    NSString *newStr = [NSString stringWithFormat:@"%@%@" ,@"fb://profile/",str];
    NSURL *url = [NSURL URLWithString : newStr ];
    if([[UIApplication sharedApplication] canOpenURL:(url)])
    {
        [[UIApplication sharedApplication] openURL: url];
        return;
    }
    newStr = [NSString stringWithFormat:@"%@%@" ,@"https://www.facebook.com/",str];
    url = [NSURL URLWithString : newStr ];
    if([[UIApplication sharedApplication] canOpenURL:(url)])
    {
        [[UIApplication sharedApplication] openURL: url];
    }
}
-(void)showMoreGames
{
    GlobalAdManager::sendFlurryEvent("clicked more games button");
    adWanted = MOREGAMES;
    [moreGamesStartAppAd showAd];
}
- (void)showInterstitial
{
    adWanted = INTERISITAL;
    [interistialStartAppAd showAd];
}
- (void) showBanner
{	
    banner = [[STABannerView alloc] initWithSize:STA_AutoAdSize autoOrigin:STAAdOrigin_Bottom
                                        withView:[UIApplication sharedApplication].keyWindow.rootViewController.view withDelegate:self];
    [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:banner];
}
- (void) hideBanner
{
    [banner hideBanner];
}
//STARTAPP DELEGATES//
//STARTAPP FAILED -> SHOW CHARTBOOST
- (void) failedShowAd:(STAAbstractAd*)ad withError:(NSError *)error
{
    if(adWanted == MOREGAMES)
    {
        [Chartboost showMoreApps:CBLocationHomeScreen];
        [self reinitMoreGames];
    }
    else if(adWanted == INTERISITAL)
    {
        [Chartboost showInterstitial:CBLocationHomeScreen];
        [self reinitInterestial];
    }
}
//PRELOAD AGAIN
- (void) didShowAd:(STAAbstractAd*)ad
{
    if(adWanted == MOREGAMES)
    {
        GlobalAdManager::sendFlurryEvent("Did showed more apps");
    }
}
- (void) failedLoadAd:(STAAbstractAd*)ad withError:(NSError *)error
{
    
}
- (void) didLoadAd:(STAAbstractAd*)ad
{
    
}
- (void) didCloseAd:(STAAbstractAd*)ad
{
    if(adWanted == MOREGAMES)
    {
        GlobalAdManager::sendFlurryEvent("Did closed more apps");
        [self reinitMoreGames];
    }
    else if(adWanted == INTERISITAL)
    {
        [self reinitInterestial];
    }
}
- (void) didClickAd:(STAAbstractAd*)ad
{
    if(adWanted == MOREGAMES)
    {
        GlobalAdManager::sendFlurryEvent("Did clicked more apps");
    }
}

-(void) checkPurchasesIos
{
    //[[SKPaymentQueue defaultQueue]restoreCompletedTransactions];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}
//in app purchase
-(void) rmvAdsIos
{
    [self checkPurchasesIos];
    for(SKProduct * skProduct in _products)
    {
        if([skProduct.productIdentifier isEqualToString:rmvAdsID])
        {
            SKPayment *payment = [SKPayment paymentWithProduct:skProduct];
            [[SKPaymentQueue defaultQueue]addPayment: payment];
        }
    }
}
-(void) unlockLevelIos
{
    [self checkPurchasesIos];
    for(SKProduct * skProduct in _products)
    {
        if([skProduct.productIdentifier isEqualToString: unlckLevelsID])
        {
            SKPayment *payment = [SKPayment paymentWithProduct:skProduct];
            [[SKPaymentQueue defaultQueue]addPayment: payment];
        }
    }
}

-(void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for(SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased || SKPaymentTransactionStateRestored:
                if([transaction.payment.productIdentifier isEqualToString: rmvAdsID])
                {
                    GlobalAdManager::onBoughtRemoveAds();
                }
                    
                else if([transaction.payment.productIdentifier isEqualToString: unlckLevelsID])
                {
                    GlobalAdManager::onBoughtLevels();
                }
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;
        }
    }
}
-(void) fetchProducts
{
    NSSet *productIdentifiers = [NSSet setWithObjects:rmvAdsID,unlckLevelsID,nil ];
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
}
-(void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    _products = [[NSArray alloc]initWithArray:response.products];
    _productsRequest=nil;
}
//FLURRY
-(void) callFlurryIos : (NSString*)str
{
    [Flurry logEvent:str];
}
@end
