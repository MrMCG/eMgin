#pragma once

// Holds a texture (sprite) for simple image drawing

class CSprite
{
public:
	CSprite(SDL_Texture* image);
	CSprite();

	virtual ~CSprite(void);

	void SetTile(int x, int y, int w, int h);
	void SetTEX(SDL_Texture* texture);
	void ToggleDel();

	SDL_Texture* GetTEX() const;
	SDL_Rect* GetCROP() const;

	virtual void Print(SDL_Renderer* rend, string mssage, int r, int g, int b) {};
	virtual void Print(SDL_Renderer* rend, int number, int r, int g, int b) {};

protected:
	SDL_Texture* tex;
	SDL_Rect* crop;
	bool del;
};

