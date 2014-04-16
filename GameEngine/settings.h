// Global constants to be used
// ** can be changed in menus?? **

namespace settings
{
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const Uint32 INPUT_DELAY = 23; // Msec
	// set to ratio of screen resolution
	const int TILE_COLUMN = 32;
	const int TILE_ROW = 18;
	// this sets the scale of the rendering to the size of the Box2D world
	const int TILE_SCALE = 4;  

	const int TILE_COLUMN_CALC = SCREEN_WIDTH/TILE_COLUMN;
	const int TILE_ROW_CALC = SCREEN_HEIGHT/TILE_ROW;
	const int TILE_COLUMN_CALC_R = SCREEN_WIDTH%TILE_COLUMN;
	const int TILE_ROW_CALC_R = SCREEN_HEIGHT%TILE_ROW;
	const int TILE_PIXEL_SIZE = SCREEN_HEIGHT/TILE_ROW;
	const int TILE_PIXEL_METER = TILE_PIXEL_SIZE/TILE_SCALE;

	const string IMAGE_LOCATION = "resources/images/";
	const string SOUND_LOCATION = "resources/sounds/";
	const string MUSIC_LOCATION = "resources/music/";
	const string LEVEL_LOCATION = "resources/levels/";
}