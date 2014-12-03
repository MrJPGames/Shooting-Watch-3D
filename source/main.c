#include <3ds.h>
#include "gfx.h"
#include "render.h"
#include "touch.h"

#include "bgtop_bin.h"
#include "bgbottom_bin.h"
#include "button_selected_bin.h"
#include "button_unselected_bin.h"
#include "button_exit_bin.h"

int hscore[2] = {0};
int avgScore[2] = {0};
int playTimes[2] = {0};

int gamemid=0; //Game mode ID (0=A 1=B)

bool inGame=false;
bool inMenu=true;
bool inGameOver=false;
bool inGameTypeB=false;

bool readyTypeA=false;
bool readyTypeB=true;

int score=0; //Score of current game
int gametime=0; //seconds into game
int gotimer=0; //timer for game over screen!

touchPosition touch;

void resetVars(){
	score=0; //Score of current game
	gametime=0; // 1/60 of seconds into game
	gotimer=0;
}

void render(){
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)bgtop_bin, 240, 400, 0, 0); //Render Background!
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)bgbottom_bin, 240, 320, 0,0); //Render Background Bottom screeen!
	renderBottomScreen(hscore[gamemid], avgScore[gamemid]);
	if (inGame || inGameOver){
		renderScore(score);
		if (inGame){
			renderTimer(gametime);
		}
	}else{
		renderScore(hscore[gamemid]);
	}

	//render buttons
	gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_exit_bin, 36, 133, 35, 20);
	if (readyTypeA || (inGame && !inGameTypeB)){
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_selected_bin, 36, 133, 181, 20);
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_unselected_bin+19152, 36, 133, 140, 20);
	}else if (readyTypeB || inGameTypeB){
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_selected_bin+19152, 36, 133, 140, 20);
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_unselected_bin, 36, 133, 181, 20);
	}else{
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_unselected_bin, 36, 133, 181, 20);
		gfxDrawSpriteAlpha(GFX_BOTTOM, GFX_LEFT, (u8*)button_unselected_bin+19152, 36, 133, 140, 20);
	}
}

int main()
{
	// Initialize services
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInit();
	resetVars();
	//gfxSet3D(true); // uncomment if using stereoscopic 3D

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		hidTouchRead(&touch);
		gamemid=readyTypeB;
		if (score > hscore[readyTypeB])
			hscore[readyTypeB]=score;
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if (inGame){
			if (kDown & KEY_A)
				score++;
			if (kDown & KEY_B && inGameTypeB)
				score++;
			gametime++;
			if (gametime > 600){
				inGame=false;
				inGameOver=true;
				if (playTimes[gamemid] == 0){
					avgScore[gamemid]=score; //No need to filter out "normal code" as it will still work!
				}else{
					avgScore[gamemid]=(avgScore[gamemid]*playTimes[gamemid]+score)/(playTimes[gamemid]+1);
				}
				playTimes[gamemid]++;
			}
		}
		if (inMenu){
			if (readyTypeA){
				if (kDown & KEY_A){
					inMenu=false;
					inGame=true;
					inGameTypeB=false;
					resetVars();
				}
			}else if (readyTypeB){
				if (kDown & KEY_A || kDown & KEY_B){
					inMenu=false;
					inGame=true;
					inGameTypeB=true;
					resetVars();
				}
			}
			if (touchInBox(touch, 20, 23, 153, 59)){
				readyTypeA=true;
				readyTypeB=false;
			}
			if (touchInBox(touch, 20, 64, 153, 100)){
				readyTypeA=false;
				readyTypeB=true;
			}
		}
		if (inGameOver){
			gotimer++;
			if (kDown & KEY_B || gotimer > 120){
				inGameOver=false;
				inMenu=true;
			}
		}

		if (touchInBox(touch, 20, 169, 153, 206))
			break;
		render();
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}

	// Exit services
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}