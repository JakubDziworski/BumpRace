#ifndef __MACROSMY__H___
#define __MACROSMY__H___

typedef enum //MENU
{
	//*LAYOUTS*//	//*BUTTONS*//
	L_MAINMENU, B_PLAYSINGLE, B_PLAYMULTI, B_OPTIONS, B_BACK,
		L_PLAYSINGLE, LAB_SINGLEPLAYER, B_CARRER, B_FREERUN, B_BESTSCORERUN,
			L_CARRER, LAB_CARRER, 
			L_FREERUN, LAB_FREERUN, LAB_CHOOSEMODE, PG_CHOOSEMODE, B_OPPONENTSSLIDE, B_DIFFICULTYSLIDER, LAB_OPPONENTSNUMBERSLIDER, B_FREERUNACCEPTANDPLAY, LAB_DIFFICULTYLABEL,LAB_GATESNUMBER,B_GATESLIDER,B_CONTINUETOCHOSEBOX,
				L_CHOOSENAMES, LAB_CHOSENAMES, PG_PLAYERBOX,
		L_PLAYMULTI,
			L_MULTIFREELOCALRUN,LAB_FREERUNMULTI,LAB_M_OPPONENTSNUMBER,LAB_M_PLAYERSNUMBER,LAB_PLAYERSNUMBER,PG_MULTICHOSEMODE,B_M_PLAYERSLIDER,B_M_OPPONENTSSLIDER,B_M_DIFFICULTYSLIDER,LAB_M_DIFFLABELSLIDER,B_M_GATESLIDER,LAB_M_GATESLIDER,B_M_CONTINUETOBOXCHOOSE,
				L_M_CHOOSENAMES,LAB_M_CHOSENAMES,T_PLAYER1NAME,T_PLAYER2NAME,T_PLAYER3NAME,T_PLAYER4NAME,PG_PLAYER1BOX,PG_PLAYER2BOX,PG_PLAYER3BOX,PG_PLAYER4BOX,B_M_PLAYNOW,E_DIALOGREPAIR,
		L_OPTIONS,
		E_TUTORIALDIALOG
};
typedef enum //HUD
{
	B_PAUSE
};
typedef enum //KINDASINGLETON
{
	LAYER_HUD,LAYER_GAMEPLAY
};
typedef enum //MISC
{
	CPCOLIDEWITHBOXES = 1, //LAYER FLOOR
	CPUNCOLIDEWITHBOXES = 2,
	CPFLOORCOLIDER = 3,
	COLLISONTYPEBOX = 4,
	COLLISIONTYPEFLOOR = 5
};
#endif