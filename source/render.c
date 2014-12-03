#include <math.h>
#include "gfx.h"
#include "scorenumbers_bin.h"
#include "numbers_bin.h"
#include "smallnumbers_bin.h"

void renderNumber(int x, int y, int dinum,gfxScreen_t screen){
	gfxDrawSpriteAlpha(screen, GFX_LEFT, (u8*)numbers_bin+2448*dinum, 34, 18, y, x); //change around to be more intuitive
}

void renderSmallNumber(int x, int y, int tinum){
	gfxDrawSpriteAlpha(GFX_TOP, GFX_LEFT, (u8*)smallnumbers_bin+480*tinum, 15, 8, y, x); //make small
}

void renderNiceNumber(int x, int y, int dinum,gfxScreen_t screen){
	gfxDrawSprite(screen, GFX_LEFT, (u8*)scorenumbers_bin+561*dinum, 17, 11, y, x); //change around to be more intuitive
}

void renderTimer(int timer){
	int remtimer=ceil(10-timer/60);
	if (remtimer == 10){
		renderSmallNumber(307, 157, 1);
		renderSmallNumber(317, 157, 0);
	}else{
		renderSmallNumber(317, 157, remtimer);
	}
}

void renderScore(int score){
	int h=floor(score/100);
	int t=floor(score/10-h*10);
	int o=score-t*10-h*100;
	if (h != 0){renderNumber(234, 154, h, GFX_TOP);}
	if (t != 0 || h != 0){renderNumber(256, 154, t, GFX_TOP);}
	renderNumber(278, 154, o, GFX_TOP);
}

void renderBottomScreen(int hscore, int avrScore){
	int nummove1=0;
	if (hscore < 10){nummove1=2;}else if (hscore < 100){nummove1=1;}
	int nummove2=0;
	if (avrScore < 10){nummove2=2;}else if (avrScore < 100){nummove2=1;}
	int h1=floor(hscore/100);
	int t1=floor(hscore/10-h1*10);
	int o1=hscore-t1*10-h1*100;
	int h2=floor(avrScore/100);
	int t2=floor(avrScore/10-h2*10);
	int o2=avrScore-t2*10-h2*100;
	if (h1 != 0){renderNiceNumber(171, 175, h1, GFX_BOTTOM);}
	if (t1 != 0 || h1 != 0){renderNiceNumber(183-(nummove1*13), 175, t1, GFX_BOTTOM);}
	renderNiceNumber(196-(nummove1*13), 175, o1, GFX_BOTTOM);
	if (h2 != 0){renderNiceNumber(171, 143, h2, GFX_BOTTOM);}
	if (t2 != 0 || h2 != 0){renderNiceNumber(183-(nummove2*13), 143, t2, GFX_BOTTOM);}
	renderNiceNumber(196-(nummove2*13), 143, o2, GFX_BOTTOM);

	
}