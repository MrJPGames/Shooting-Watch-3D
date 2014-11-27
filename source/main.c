#include <3ds.h>
#include <math.h>
#include "gfx.h"

#include "bgtop_bin.h"
#include "numbers_bin.h"

int hscore=0;
int avgScore=0;
int playTimes=0;
bool inGame=false;
bool inMenu=true;
bool inGameOver=false;
int score=0; //Score of current game
int gametime=0; //seconds into game
int h=0; //hunderds
int t=0; //tens
int o=0; //ones

void resetVars(){
	score=0; //Score of current game
	gametime=0; //seconds into game
	h=0; //hunderds
	t=0; //tens
	o=0; //ones
}

void renderNumber(int x, int y, int dinum){
	gfxDrawSpriteAlpha(GFX_TOP, GFX_LEFT, (u8*)numbers_bin+2448*dinum, 34, 18, y, x); //change around to be more intuitive
}

void render(){
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)bgtop_bin, 240, 400, 0, 0); //Render Background!
	h=floor(score/100);
	t=floor(score/10-h*10);
	o=score-t*10-h*100;
	if (h != 0){renderNumber(234, 152, h);}
	if (t != 0 || h != 0){renderNumber(256, 152, t);}
	renderNumber(278, 152, o);
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

		// Your code goes here

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		if (inGame){
			if (kDown & KEY_A)
				score++;
			gametime++;
			if (gametime > 600){
				inGame=false;
				inGameOver=true;
				if (playTimes == 0){
					avgScore=score; //No need to filter out "normal code" as it will still work!
				}else{
					avgScore=(avgScore*playTimes+score)/(playTimes+1);
				}
				playTimes++;
			}
		}
		if (inMenu){
			if (kDown & KEY_A){
				inMenu=false;
				inGame=true;
				resetVars();
			}
		}
		if (inGameOver){
			if (score > hscore){hscore=score;}
			if (kDown & KEY_B){
				inGameOver=false;
				inMenu=true;
			}
		}

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
