/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.security.spec.MGF1ParameterSpec;
import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHandler;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxVideoHelper;

import com.android.vending.billing.IInAppBillingService;
import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.Libraries.CBLogging.Level;
import com.flurry.android.FlurryAgent;
import com.screw.facebook.*;
import com.startapp.android.publish.Ad;
import com.startapp.android.publish.Ad.AdState;
import com.startapp.android.publish.AdDisplayListener;
import com.startapp.android.publish.AdEventListener;
import com.startapp.android.publish.StartAppAd;
import com.startapp.android.publish.StartAppSDK;
import com.startapp.android.publish.StartAppAd.AdMode;
import com.startapp.android.publish.banner.Banner;
import com.startapp.android.publish.banner.banner3d.Banner3D;

import android.R.layout;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.sax.StartElementListener;
import android.test.suitebuilder.TestSuiteBuilder.FailedToCreateTests;
import android.text.Layout;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

public class AppActivity extends Cocos2dxActivity implements AdDisplayListener,AdEventListener {
	private static AppActivity me;
	
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		me = this;
		Intent serviceIntent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
		serviceIntent.setPackage("com.android.vending");
		bindService(serviceIntent, mServiceConn, Context.BIND_AUTO_CREATE);
		Chartboost.startWithAppId(me,"545265c30d60255df529d478","37ab571c15f89939d444cd5a7e8bc9123dff8066");
		Chartboost.setLoggingLevel(Level.ALL);
		Chartboost.onCreate(me);
		StartAppSDK.init(me, "109722583", "210264246", true);
		me.interestialAd = new StartAppAd(me);
		me.moreGamesAd = new StartAppAd(me);
		Facebook.onActivityCreate(this, savedInstanceState);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		if(interestialAd != null)
		{
			interestialAd.onResume();
			moreGamesAd.onResume();
			Chartboost.onResume(me);
		}
		Facebook.onActivityResume();
	}
	@Override
	protected void onStart () {
		super.onStart ();
		FlurryAgent.onStartSession(me,"6GFZG5MR25NNMVZ4TKTC");
	}
	@Override
	protected void onStop() {
		super.onStop();
		FlurryAgent.onEndSession(me);
	}
	@Override
	protected void onPause() {
		super.onPause();
		if(interestialAd!=null)
		{
			interestialAd.onPause();
			moreGamesAd.onPause();
			Chartboost.onPause(me);
		}
		Facebook.onActivityPause();
	}
	@Override
	public void onDestroy() {
	    super.onDestroy();
	    Chartboost.onDestroy(me);
	    if (mService != null) 
	    {
	        unbindService(mServiceConn);
	    }
	}
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        Facebook.onActivityResult(requestCode, resultCode, data);
        //IN APP
        if (requestCode == 1097) 
        {           
            if (resultCode == RESULT_OK) 
            {
              //TU ZE KUPILES REMOVE ADS
            	purchasedAds();
            }
         }
        if (requestCode == 1098)
        {
        	 if (resultCode == RESULT_OK) 
             {
               //TU ZE KUPILES POZIOMY
        		purchasedLevels();
             }
        }
    }
	private static boolean checkActv(Intent aint){
    	try{
    		me.startActivity(aint);
    		return true;
    	}
    	catch(Exception e){
    		 return false;
    	}
    }
	//*****OPEN LINK*******//
	public static void openURL(final String url){
		me.runOnUiThread(new Runnable() {
			@Override
			public void run() {
		    	//normal Link
				if(url.contains("http"))
				{
					//market
					Intent i = new Intent(Intent.ACTION_VIEW);
					i.setData(Uri.parse(url));
					checkActv(i);
				}
				else
				{
					checkActv(getOpenFacebookIntent(url));
				}
			}
		});
    }
	public static Intent getOpenFacebookIntent(String str) {
		   try {
		    me.getPackageManager().getPackageInfo("com.facebook.katana", 0);
		    return new Intent(Intent.ACTION_VIEW, Uri.parse("fb://page/"+str));
		   } catch (Exception e) {
		    return new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/"+str));
		   }
		}
	
	//*************ADS**************//
	enum CurAdWanted{INTERESTIAL,MOREGAMES,BANNER};
	public CurAdWanted curAdWanted;
	public StartAppAd interestialAd;
	public StartAppAd moreGamesAd;
	public Banner bannerAd;
	boolean failedToLoad = false;
	
	public static void showInteristial()
	{
		me.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				me.curAdWanted = CurAdWanted.INTERESTIAL;
				if(me.failedToLoad)
				{
					me.failedToShowStartApp();
				}
				else
				{
					me.interestialAd.showAd(me);
				}
			}
		});
		
	}
	public static void showExitAd()
	{
		me.interestialAd.onBackPressed();
	}
	public static void showMoreGames()
	{
		me.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				logFlurry("clicked More Games Button");
				me.curAdWanted = CurAdWanted.MOREGAMES;
				if(me.failedToLoad)
				{
					me.failedToShowStartApp();
				}
				else
				{
					me.moreGamesAd.showAd(me);
				}
			}
		});
	}
	public static void hideBanner()
	{
		me.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				if(me.bannerAd!=null)
				me.bannerAd.hideBanner();
			}
		});
	}
	public static void showBanner()
	{
		me.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				FrameLayout mainLayout = (FrameLayout) me.getWindow().getDecorView().findViewById(android.R.id.content);
				if(me.bannerAd!=null)
				{
				mainLayout.removeView(me.bannerAd);
				}
				me.curAdWanted = CurAdWanted.BANNER;
				me.bannerAd = new Banner(me);
				FrameLayout.LayoutParams bannerParameters =
			            new FrameLayout.LayoutParams(
			                        FrameLayout.LayoutParams.WRAP_CONTENT,
			                        FrameLayout.LayoutParams.WRAP_CONTENT
			                        );
				bannerParameters.gravity = Gravity.CENTER | Gravity.BOTTOM;
				// Add to main Layout
				mainLayout.addView(me.bannerAd, bannerParameters);
			}
		});
	}
	private void reloadInterestial()
	{
		interestialAd = new StartAppAd(me);
		interestialAd.loadAd(AdMode.FULLPAGE,me);
	}
	private void reloadMoreGames()
	{
		moreGamesAd = new StartAppAd(me);
		moreGamesAd.loadAd(AdMode.OFFERWALL,me);
	}
	public static void loadAds()
	{
		me.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.v("ADS","started loading ads");
				Chartboost.onStart(me);
				Chartboost.cacheMoreApps(CBLocation.LOCATION_MAIN_MENU);
				me.reloadMoreGames();
				me.reloadInterestial();
			}
		});
	}
	private void failedToShowStartApp()
	{
		if(curAdWanted == CurAdWanted.MOREGAMES)
	    {
			Chartboost.showMoreApps(CBLocation.LOCATION_MAIN_MENU);
			reloadMoreGames();
	    }
	    else if(curAdWanted == CurAdWanted.INTERESTIAL)
	    {
	    	Chartboost.showInterstitial(CBLocation.LOCATION_GAMEOVER);
			reloadInterestial();
	    }
	}
	@Override
	public void onFailedToReceiveAd(Ad arg0) 
	{
		Log.v("ADS","ad failed");
		failedToLoad= true;
	}
	@Override
	public void onReceiveAd(Ad arg0) {
		failedToLoad = false;
		Log.v("ADS","ad recieved");
	}
	@Override
	public void adClicked(Ad arg0) {
		Log.v("ADS","ad clicked");
	}
	@Override
	public void adDisplayed(Ad arg0) {
		Log.v("ADS","ad displayed");
	}
	@Override
	public void adHidden(Ad arg0) {
		Log.v("ADS","ad hidden");
		if(curAdWanted == CurAdWanted.MOREGAMES)
	    {
			reloadMoreGames();
	    }
	    else if(curAdWanted == CurAdWanted.INTERESTIAL)
	    {
			reloadInterestial();
	    }
	}
	
	//IN APP PURCHASE
	static IInAppBillingService mService;
	public static native void purchasedAds();
	public static native void purchasedLevels();
	ServiceConnection mServiceConn = new ServiceConnection() 
	{
		   @Override
		   public void onServiceDisconnected(ComponentName name) 
		   {
			   Log.v("ADS","IN APP SERV DISCONNECTED");
		       mService = null;
		   }

		   @Override
		   public void onServiceConnected(ComponentName name, IBinder service) 
		   {
			   Log.v("ADS","IN APP SERV CONNCTD");
		       mService = IInAppBillingService.Stub.asInterface(service);
		   }
	};
	public static void inAppUnlocked()
	{
		me.runOnUiThread(new Runnable() {
			@Override
			public void run() 
			{
				if(mService!=null)
				{
					try 
					{
						 Log.v("ADS","STARTED FETCHING IN APP PURCHASES");
						Bundle ownedItems = mService.getPurchases(3, me.getPackageName(), "inapp", null);
						int response = ownedItems.getInt("RESPONSE_CODE");
						
						if (response == 0) {
							
						   ArrayList<String> ownedSkus = ownedItems.getStringArrayList("INAPP_PURCHASE_ITEM_LIST");
						   if(ownedSkus.size() == 0) return;
						   if(ownedSkus.size() == 2)
						   {
							   Log.v("ADS","BOTH APPS BOUGHT");
							   purchasedLevels();
							   Log.v("ADS","AFTER SETTING PURCHASED LEVELS");
							   purchasedAds();
							   Log.v("ADS","AFTER SETTING PURCHASED ADS");
						   }
						   else if(ownedSkus.get(0).equals("com.bumprace.removeadsrelease"))
							   {
							   purchasedAds();
							   Log.v("ADS","AFTER SETTING PURCHASED ADS ALONE");
							   }
						   else if(ownedSkus.get(0).equals("com.bumprace.unlocklevelrelease"))
							   {
							   purchasedLevels();
							   Log.v("ADS","AFTER SETTING PURCHASED LEVELS ALONE");
							   }
						}
					} 
					catch (RemoteException e) 
					{
						e.printStackTrace();
					}
				}
			}
		});
	}
	public static void unlockLevels()
	{
//		me.runOnUiThread(new Runnable() {
//			
//			@Override
//			public void run() {
//				try {
//					if(mService == null) return;
//					Bundle buyIntentBundle = mService.getBuyIntent(3, me.getPackageName(),
//							"com.bumprace.unlocklevelrelease", "inapp", "unlockLevelPayLoad");
//					PendingIntent pendingIntent = buyIntentBundle.getParcelable("BUY_INTENT");
//					try {
//						me.startIntentSenderForResult(pendingIntent.getIntentSender(),
//								   1098, new Intent(), Integer.valueOf(0), Integer.valueOf(0),
//								   Integer.valueOf(0));
//					} catch (SendIntentException e) {
//						e.printStackTrace();
//					}
//				} catch (RemoteException e) {
//					e.printStackTrace();
//				}
//			}
//		});
	}
	public static void removeAd()
	{
//		me.runOnUiThread(new Runnable() {
//			
//			@Override
//			public void run() {
//				try {
//					if(mService == null) return;
//					Bundle buyIntentBundle = mService.getBuyIntent(3, me.getPackageName(),
//							"com.bumprace.removeadsrelease", "inapp", "removeAdsPayLoad");
//					PendingIntent pendingIntent = buyIntentBundle.getParcelable("BUY_INTENT");
//					try {
//						me.startIntentSenderForResult(pendingIntent.getIntentSender(),
//								   1097, new Intent(), Integer.valueOf(0), Integer.valueOf(0),
//								   Integer.valueOf(0));
//					} catch (SendIntentException e) {
//						e.printStackTrace();
//					}
//				} catch (RemoteException e) {
//					e.printStackTrace();
//				}
//			}
//		});
	}
	//FLURRY
	public static void logFlurry(String str)
	{
		FlurryAgent.logEvent("3rd "+str);
	}
}
