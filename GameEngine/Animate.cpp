
#include "Animate.h"

CAnimate::CAnimate(SDL_Renderer* pass_renderer, SDL_Texture* image, SDL_Texture* errorTex)
{
	tex = NULL;
	tex = image;

	if (tex == NULL)
	{
		printf("ERROR: CAnimate - %s\n", SDL_GetError());
	}

	// uses error tex
	// dont animatte
	if (this->tex == errorTex)
	{
		error = true;
	} else
	{
		error = false;
	}

	SDL_QueryTexture(tex, NULL, NULL, &texWidth, &texHeight);

	crop.x = 0;
	crop.y = 0;
	crop.w = texWidth;
	crop.h = texHeight;
	ANIM_currentFrame = 0;

	ANIM_DELAY = 0;
	ANIM_SPEED = 128;
	IDLE_beginFrame = 0;
	IDLE_endFrame = 2; 
	
	ANIM_ROW_SIZE = 4;
	ANIM_COLUMN_SIZE = 4;
}

CAnimate::~CAnimate(void)
{
	tex = NULL;
	delete tex;
}

void CAnimate::Animation(int type)
{
	if (!error)
	{
		switch (type)
		{
		case 1:
			PerformAnimation(0);
			break;
		case 2:
			PerformAnimation(1);
			break;
		case 3:
			PerformAnimation(2);
			break;
		case 4:
			PerformAnimation(3);
			break;
		case 5:
			PerformAnimation(4);
			break;
		default:
			break;
		}
	} 
}

void CAnimate::PerformAnimation(int animType)
{
	if (ANIM_DELAY + ANIM_SPEED < SDL_GetTicks())
	{
		if (IDLE_endFrame < ANIM_currentFrame)
		{
			ANIM_currentFrame = IDLE_beginFrame;
		} else
		{
			ANIM_currentFrame++;
		}

		crop.x = ANIM_currentFrame * (texWidth/ANIM_ROW_SIZE);
		crop.y = animType * (texHeight/ANIM_COLUMN_SIZE);
		crop.w = texWidth/ANIM_ROW_SIZE;
		crop.h = texHeight/ANIM_COLUMN_SIZE;

		UpdateDelay();
	}
}

void CAnimate::UpdateDelay()
{
	ANIM_DELAY = SDL_GetTicks();
}

SDL_Texture* CAnimate::GetTEX() const
{
	return tex;
}

SDL_Rect CAnimate::GetCROP() const
{
	return crop;
}

int CAnimate::GetFrame() const
{
	return ANIM_currentFrame;
}

int CAnimate::GetRow() const
{
	return crop.y / (texHeight/ANIM_COLUMN_SIZE);
}