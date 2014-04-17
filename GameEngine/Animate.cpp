#include "Animate.h"

CAnimate::CAnimate(SDL_Renderer* pass_renderer, CResources* resources, int index)
{
	tex = NULL;
	tex = resources->GetTex(index);

	if (tex == NULL)
	{
		printf("ERROR: CAnimate - %s\n", SDL_GetError());
	}

	// uses error tex
	// dont animatte
	if (this->tex == resources->GetTex(0))
	{
		error = true;
	} else
	{
		error = false;
	}

	SDL_QueryTexture(tex, NULL, NULL, &texWidth, &texHeight);

	// initialising values
	crop.x = 0;
	crop.y = 0;
	crop.w = texWidth;
	crop.h = texHeight;
	currentFrame = 0;
	delay = 0;
	prevAnimType = 0;
	
	speed = 128;
	
	rowSize = 4;
	columnSize = 4;

	resetFrame = false;
}

CAnimate::~CAnimate(void)
{
	tex = NULL;
	delete tex;
}

void CAnimate::PerformAnimation(int animType)
{
	if (animType != prevAnimType)
	{
		prevAnimType = animType;
		if (resetFrame)
		{
			currentFrame = 0;
		}
	}
	if (!error && delay + speed < SDL_GetTicks())
	{
		if (currentFrame >= columnSize-1)
		{
			currentFrame = 0;
		} else
		{
			currentFrame++;
		}

		crop.x = currentFrame * (texWidth/columnSize);
		crop.y = animType * (texHeight/rowSize);
		crop.w = texWidth/columnSize;
		crop.h = texHeight/rowSize;

		UpdateDelay();
	}
}

void CAnimate::UseFrame(int column, int row)
{
	crop.x = column * (texWidth/columnSize);
	crop.y = row * (texHeight/rowSize);
	crop.w = texWidth/columnSize;
	crop.h = texHeight/rowSize;
	currentFrame = column;
}

void CAnimate::UpdateDelay()
{
	delay = SDL_GetTicks();
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
	return currentFrame;
}

int CAnimate::GetRow() const
{
	return crop.y / (texHeight/columnSize);
}