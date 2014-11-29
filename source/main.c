#include <3ds.h>
#include "gfx.h"
#include "render.h"

#include "bgtop_bin.h"
#include "bgbottom_bin.h"

int hscore=0;
int avgScore=0;
int playTimes=0;
bool inGame=false;
bool inMenu=true;
bool inGameOver=false;
int score=0; //Score of current game
int gametime=0; //seconds into game
int gotimer=0; //timer for game over screen!

void resetVars(){
	score=0; //Score of current game
	gametime=0; // 1/60 of seconds into game
	gotimer=0;
}

void render(){
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)bgtop_bin, 240, 400, 0, 0); //Render Background!
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)bgbottom_bin, 240, 320, 0, 0); //Render Background Bottom screeen!
	renderBottomScreen(hscore, avgScore);
	if (inGame || inGameOver){
		renderScore(score);
		if (inGame){
			renderTimer(gametime);
		}
	}else{
		renderScore(hscore);
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

		// Your code goes here

		if (score > hscore){hscore=score;}
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
			gotimer++;
			if (kDown & KEY_B || gotimer > 120){
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
