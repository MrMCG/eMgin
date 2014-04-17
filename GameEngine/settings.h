// Global constants to be used
// ** can be changed in menus?? **
#pragma once

namespace settings
{
	// changeable settings 
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern int SCREEN;

	const Uint32 INPUT_DELAY = 5; // Msec
	// set to ratio of screen resolution
	const int TILE_COLUMN = 32;
	const int TILE_ROW = 18;
	// this sets the scale of the rendering to the size of the Box2D world
	const int TILE_SCALE = 4;  

	// calculated settings
	extern int TILE_COLUMN_CALC;
	extern int TILE_ROW_CALC;
	extern int TILE_COLUMN_CALC_R;
	extern int TILE_ROW_CALC_R;
	extern int TILE_PIXEL_SIZE;
	extern int TILE_PIXEL_METER;

	const string IMAGE_LOCATION = "resources/images/";
	const string SOUND_LOCATION = "resources/sounds/";
	const string MUSIC_LOCATION = "resources/music/";
	const string LEVEL_LOCATION = "resources/levels/";
	const string SETTINGS_LOCATION = "resources/";
}