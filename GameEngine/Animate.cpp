#include "Animate.h"

CAnimate::CAnimate(SDL_Renderer* pass_renderer, CResources* resources, int index)
{
	tex = NULL;
	tex = resources->GetTexResources(index);

	if (tex == NULL)
	{
		printf("ERROR: CAnimate - %s\n", SDL_GetError());
	}

	// uses error tex
	// dont animatte
	if (this->tex == resources->GetTexResources(0))
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
		currentFrame = 0;
	}
	if (!error && delay + speed < SDL_GetTicks())
	{
		if (columnSize-1 <= currentFrame)
		{
			currentFrame = 0;
		} else
		{
			currentFrame++;
		}

		crop.x = currentFrame * (texWidth/rowSize);
		crop.y = animType * (texHeight/columnSize);
		crop.w = texWidth/rowSize;
		crop.h = texHeight/columnSize;

		UpdateDelay();
	}
}

void CAnimate::UseFrame(int column, int row)
{
	crop.x = column * (texWidth/rowSize);
	crop.y = row * (texHeight/columnSize);
	crop.w = texWidth/rowSize;
	crop.h = texHeight/columnSize;
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