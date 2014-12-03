#include <3ds.h>

bool touchInBox(touchPosition touch, int x1, int y1, int x2, int y2){
	int tx=touch.px;
	int ty=touch.py;
	u32 kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x1 && tx < x2 && ty > y1 && ty < y2){
		return true;
	}else{
		return false;
	}
}