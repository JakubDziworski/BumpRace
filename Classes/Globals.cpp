#include "Globals.h"
#include "cocos2d.h"
#include "VisibleRect.h"
#include "Macros.h"
#include "Paths.h"
#include "UI/CocosGUI.h"
#include "dialogReader.h"
#include "SingleGateWorld.h"
#include "SingleEliminationWorld.h"
#include "EndlessWorld.h"
#include "DbReader.h"
std::string G_bgFilePath="";
std::string G_flatTopFilePath="";
std::string G_drzewkaFilePath="";
cocos2d::Texture2D *G_faceBookAvatarTex = NULL;
cocos2d::Vector<Sprite*> G_spritesUsingFBImage(2);
int G_endlessGateNumber = 5;
int G_odlegloscmiedzyBramkami = 7000;
cocos2d::Director *G_director = NULL;
cocos2d::Vec2 G_srodek = cocos2d::Vec2(0,0);
float G_prevXgravity = 0;
float G_Currangle = 0;
float G_myCos = 0;
float G_mySin = 0;
const float G_radToAngle = 180.0f / M_PI;
const float Globals_radWsp = M_PI / 180.0f;
int G_maxVelocity = 150;
int G_maxVelConstant = 125;
int G_maxVelAddition = 300;
int G_powerUpOdleglos = 500;
int G_powerUpOdlegloscVar = 3000;
int G_powerUpsNumbers = 2;
std::string G_playersDefaultNames[4] = { "", "", "", "" };
bool FB_connected = false;
bool FB_loginListenerExist = false;
extern cocos2d::ActionManager *slowActionManager;
extern cocos2d::ActionManager *normalActionManager;
cocos2d::Dictionary *G_strings;
const int G_ALLboxesNumber = 6;
const cocos2d::Color3B G_colors[6] = { cocos2d::Color3B(115, 207, 231), cocos2d::Color3B(178, 210, 53), cocos2d::Color3B(130, 85, 127), cocos2d::Color3B(244, 191, 26), cocos2d::Color3B(226, 54, 39), cocos2d::Color3B(115, 207, 231) };
void G_setCurrAngle(float angle)
{
	G_Currangle = angle;
	G_myCos = cos(G_Currangle*Globals_radWsp);
	G_mySin = sin(G_Currangle*Globals_radWsp);
}
float G_wF(float inp)
{
	return inp *  G_srodek.x / 480.0f;
}
float G_hF(float inp)
{
	return inp * G_srodek.y / 320.0f;
}
cocos2d::Vec2 G_wV(cocos2d::Vec2 inp)
{
	return inp *  G_srodek.x / 480.0f;
}
cocos2d::Vec2 G_hV(cocos2d::Vec2 inp)
{
	return inp * G_srodek.y / 320.0f;
}
cocos2d::Director * G_dir()
{
	if (G_director == NULL) G_director = cocos2d::Director::getInstance();
	return G_director;
}
const char* G_form_str(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	return cocos2d::String::createWithFormat(format, ap)->getCString();
}
void G_initLanguage()
{
	G_srodek = VR::center();
	cocos2d::LanguageType currentLanguageType = cocos2d::Application::getInstance()->getCurrentLanguage();
	if (currentLanguageType == cocos2d::LanguageType::POLISH)
	{
		G_strings = cocos2d::Dictionary::createWithContentsOfFile("multilanguage/Polish.plist");
	}
	else
	{
		G_strings = cocos2d::Dictionary::createWithContentsOfFile("multilanguage/English.plist");
	}
	G_strings->retain();
}
std::string G_str(const std::string& input)
{
	std::string str = G_strings->valueForKey(input)->getCString();
	if (str == "") return input;
	return str;
}
cocos2d::Color3B G_getRandomColor()
{
	int tab[3];
	for (int i = 0; i < 3; i++)
	{
		tab[i] = rand() % 155 + 100;
	}
	return cocos2d::Color3B(tab[0], tab[1], tab[2]);
}
void G_scaleToFitScreen(cocos2d::Node *spr)
{
	spr->setScale(VR::right().x / spr->getContentSize().width, VR::top().y / spr->getContentSize().height);
}
World* G_getWorld()
{
	return (World*)G_director->getRunningScene()->getChildByTag(LAYER_GAMEPLAY);
}
Hud* G_getHud()
{
	return (Hud*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD);
}
float G_getFTimeScale(float val)
{
	return G_director->getScheduler()->getTimeScale()*val;
}
cocos2d::ParticleSystemQuad* G_getParticleFromFile(const std::string &filename, cocos2d::ParticleSystemQuad::PositionType type/*= cocos2d::ParticleSystemQuad::PositionType::RELATIVE*/)
{
	auto particle = cocos2d::ParticleSystemQuad::create(filename.c_str());
	auto val = G_srodek.x / 256.0f;
	particle->setBlendAdditive(false);
	particle->setPosVar(cocos2d::Vec2(particle->getPosVar().x*val, particle->getPosVar().y*val));
	particle->setEndSize(val*particle->getEndSize());
	particle->setEndSizeVar(val*particle->getEndSizeVar());
	particle->setStartSize(val*particle->getStartSize());
	particle->setStartSizeVar(val*particle->getStartSizeVar());
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
	{
		particle->setSpeed(val*particle->getSpeed());
		particle->setSpeedVar(val*particle->getSpeedVar());
	}
	particle->setPositionType(type);
	particle->setAutoRemoveOnFinish(true);
	return particle;
}
void G_displayCorrectLevelStarter(int level,cocos2d::Node *parent)
{
	auto setUpDialog = [parent](int levelnum, int levelType, int opponentsnumber, int diffLevel, int gatesNumb)
	{
		//LEVEL LABEL
		std::string cocosfile = "";
		std::string levelStr = G_str("Level") + " " + std::to_string(levelnum) + " - ";
		std::string objStr = "";
		switch (levelType)
		{
			case 1:
				levelStr += G_str("Gate_Collector");
				cocosfile = "gateWorldLevelIntro.json";
				objStr = G_str("ObjGate");
				break;
			case 2:
				levelStr += G_str("Elimination");
				cocosfile = "endlessWorldLevelIntro.json";
				objStr = G_str("ObjElim");
				break;
			case 3:
				levelStr += G_str("Endless");
				cocosfile = "gateWorldLevelIntro.json";
				objStr = G_str("ObjEndless");
				break;
		}
		//DIFF LEVEL
		std::string diffStr = " " + G_str("Opponents");
		switch (diffLevel)
		{
			case 0:
				diffStr = G_str("stupid") + diffStr;
				break;
			case 1:
				diffStr = G_str("medium") + diffStr;
				break;
			case 2:
				diffStr = G_str("smart") + diffStr;
				break;
		}
		//OPPONENTS
		std::string oppStr = std::to_string(opponentsnumber) +" "+ G_str("Opponents");
		//GATES
        std::string gatesStr = std::to_string(gatesNumb) + " ";
        if(gatesNumb >4)  gatesStr+= G_str("Gatesow");
        else gatesStr+= G_str("Gates");
		//assign texts
		DialogReader::getInstance()->getMainWidgetFromJson(cocosfile, parent);
		((cocos2d::ui::Text*)DialogReader::getInstance()->getWidget(cocosfile, "levelTitleText"))->setString(levelStr);
		((cocos2d::ui::Text*)DialogReader::getInstance()->getWidget(cocosfile, "numberOfOpponentsText"))->setString(oppStr);
		((cocos2d::ui::Text*)DialogReader::getInstance()->getWidget(cocosfile, "aiSmartnessText"))->setString(diffStr);
		((cocos2d::ui::Text*)DialogReader::getInstance()->getWidget(cocosfile, "goalText"))->setString(objStr);
		if (levelType != 2) ((cocos2d::ui::Text*)DialogReader::getInstance()->getWidget(cocosfile, "gatesNumberText"))->setString(gatesStr);
		//listener
		DialogReader::getInstance()->addActionHideAndSomething(cocosfile, "okBtn", [levelnum, levelType, opponentsnumber, diffLevel, gatesNumb]()
															   {
																   cocos2d::Scene *scena = NULL;
																   if (levelType == 1) scena = SingleGateWorld::createScene(opponentsnumber, gatesNumb, diffLevel);
																   else if (levelType == 2) scena = SingleEliminationWorld::createScene(opponentsnumber, diffLevel);
																   else
																   {
																	   scena = EndlessWorld::createScene(opponentsnumber,diffLevel,false);
																	   EndlessWorld *world = (EndlessWorld*)scena->getChildByTag(LAYER_GAMEPLAY);
																	   world->setSinglePlayer(Player::create(R_Box[0], "kuba", world->getGravitySpace(), G_colors[0]));
																	   world->setMinGates(gatesNumb);
																   }
																   World *world = (World*)scena->getChildByTag(LAYER_GAMEPLAY);
																   if(levelType != 3) world->setSinglePlayer(Player::create(R_Box[0], "kuba", world->getGravitySpace(), G_colors[0]));
																   world->setCarrierLevel(levelnum);
																   DialogReader::getInstance()->flush();
																   G_dir()->replaceScene(scena);
															   });
	};
	switch (level)
	{
		case 1://level,typ,przeciwnicy,trudnosc,bramki
			setUpDialog(1,1,4,0, 7);
			break;
		case 2:
			setUpDialog(2,2, 5, 0, 7);
			break;
		case 3:
			setUpDialog(3, 3, 4, 0, 7);
			break;
		case 4:
			setUpDialog(4,1, 5, 1, 12);
			break;
		case 5:
			setUpDialog(5, 2, 5, 1, 12);
			break;
		case 6:
			setUpDialog(6, 3,5, 1, 12);
			break;
		case 7:
			setUpDialog(7, 1, 6, 2, 20);
			break;
		case 8:
			setUpDialog(8, 2, 8, 2, 12);
			break;
		case 9:
			setUpDialog(9, 3, 8, 2, 20);
			break;
		default:
			break;
	}
}
void G_enableShadow(cocos2d::Label *lbl)
{
	const float offset = lbl->getSystemFontSize();
	lbl->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(offset / 20.0f, offset / 20.0f));
}
void G_enableShadow(cocos2d::ui::Text *lbl)
{
	const float offset = lbl->getFontSize();
	lbl->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(offset / 20.0f, offset / 20.0f));
}
//**********************************FB********************************//
void FB_setLoginCallBack(std::function <void(bool isLoggedIn)> fun,cocos2d::Node *caller)
{
    Session::getActiveSession()->setStatusCallback([fun](Session *session,SessionError *err)
    {
		if (session->getState() == Session::State::OPENED)
		{
			if (!Session::getActiveSession()->hasPermission("publish_actions"))
			{
				Session::getActiveSession()->requestPublishPermissions({ "publish_actions" });
			}

			Request::requestForMe([](int error, GraphUser *user)
			{
				if (!user) return;
				FB_loadPhoto(user->getId(), 160.0f / G_dir()->getContentScaleFactor());
				DbReader::getInstance()->setPlayerDefaultName(1, user->getFirstName());
				CCLOG("score %ld", user->getScore());
			})->execute();
            if(fun != nullptr) fun(true);
		}
		else if (session->getState() == Session::State::CLOSED || session->getState() == Session::State::INVALID)
        {
            if(fun != nullptr) fun(false);
        }
	});
}
void FB_login()
{
	if (!Session::getActiveSession()->isOpened())
	 Session::getActiveSession()->open(true, { "user_friends" },
	 DefaultAudience::PUBLIC,
	 LoginBehavior::WITH_FALLBACK_TO_WEBVIEW);
}
void FB_autLogin()
{
	if (Session::getActiveSession()->getState() == Session::State::CREATED_TOKEN_LOADED) {
	Session::getActiveSession()->open(false);
	}
}
void FB_logOut()
{
	if (Session::getActiveSession()->getState() != Session::State::CLOSED)
	Session::getActiveSession()->close();
}
void FB_addDownloadFinishListener(cocos2d::Node *eventDispatcherNode, std::function<void(cocos2d::Texture2D* sprite)> fun)
{
	EventListenerCustom *listener = EventListenerCustom::create(PhotoLoaderLoadedNotification, [=](EventCustom *event){
	PhotoLoaderEvent *ev = (PhotoLoaderEvent *)event;
	G_faceBookAvatarTex = PhotoLoader::getInstance()->loadTexture(ev->getUid());
	//TO DO delete this
	G_faceBookAvatarTex->retain();
	for (auto spr : G_spritesUsingFBImage)
	{
        spr->setTexture(G_faceBookAvatarTex);
        spr->setTextureRect(Rect(0, 0, 47.0f, 47.0f));
	}
    if(fun!=nullptr) fun(G_faceBookAvatarTex);
	});
	G_dir()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, eventDispatcherNode);
}
void FB_loadPhoto(const std::string& uid, const int size)
{
	PhotoLoader::getInstance()->download(uid,size);
}
void sharePost(const std::string &name,const std::string &caption,const std::string &descr)
{
    ShareDialogParams *params = ShareDialogParams::create();
    params->setLink("http://www.cocos2d-x.org/");
    params->setName(name);
    params->setCaption(caption);
    params->setDescription(descr);
    //  params->setFriends({"100008289311268"});
    //    params->setDataFailuresFatal(false);
    if (Dialog::canPresent(params)) {
        Dialog::present(params, [](GraphObject *result, int error){
            CCLOG("Share link dialog callback: result = %s\nerror = %d", result ? result->getValue().getDescription().c_str() : "(null)", error);
        });
    } else {
        CCLOG("Cannot show share dialog, fallback to webview");
        FeedDialogBuilder *fbd = new FeedDialogBuilder();
        fbd->setLink(params->getLink())->setDescription(params->getDescription());
        fbd->setTo("100008289311268");
        
        fbd->setCallback([](int error, const string &rid){
            CCLOG("Share link web dialog result: error = %d, rid = %s", error, rid.c_str());
        });
        fbd->build()->show();
        delete fbd;
    }
}
void FB_shareGame()
{
   sharePost(G_str("ShrGameName"),G_str("ShrGameCaption"),G_str("ShrGameDescr"));
}
void FB_shareLevelCompletedPost(const int level)
{
   sharePost(G_str("LvlCmplName")+to_string(level),G_str("LvlCmplCaption"),G_str("LvlCmplDescr"));
}
extern void FB_postBestScore(int score)
{
	if (Session::getActiveSession()->isOpened())
	{
		if (!Session::getActiveSession()->hasPermission("publish_actions"))
		{
			Session::getActiveSession()->requestPublishPermissions({ "publish_actions" });
		}
		else
		{
			Facebook::getInstance()->postScore(score);
		}
	}
	else 
	{
		FB_login();
	}
}
void FB_showScores(cocos2d::Node *nodeToAttach)
{
	auto main = DialogReader::getInstance()->getMainWidgetFromJson("FbBestScores.json", nodeToAttach);
	auto listView = (cocos2d::ui::ListView*)DialogReader::getInstance()->getWidget("FbBestScores.json", "listView");
	listView->setItemsMargin(10);
	listView->setItemModel((cocos2d::ui::Layout*)DialogReader::getInstance()->getWidget("FbBestScores.json", "record"));
	Request::requestForScores([listView](int error, const Vector<GraphScore *> &scores)
	{
		int i = 0;
		for (auto s : scores) 
		{
			listView->pushBackDefaultItem();
			((cocos2d::ui::Text*)listView->getItem(i)->getChildByName("playername"))->setString(s->getUser()->getName().c_str());
			((cocos2d::ui::Text*)listView->getItem(i)->getChildByName("playerScore"))->setString(std::to_string(s->getScore()).c_str());
			i++;
		}
	})->execute();
}
