#include "Entity.h"

CEntity::CEntity(void)
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 1;
	rect.h = 1;

	// position is centre of rectangle
	position = new Vector2D(0,0);
	sprite = NULL;
	animate = NULL;
	physics = NULL;
	collision = NULL;

	draw = false;
	debug = false;
}

CEntity::CEntity(const int x, const int y, const int w, const int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	// position is centre of rectangle
	position = new Vector2D(0,0);
	UpdatePosition();
	sprite = NULL;
	animate = NULL;
	physics = NULL;
	collision = NULL;

	draw = false;
	debug = false;
}

CEntity::~CEntity(void)
{
	delete position;
	delete sprite;
	delete animate;
	delete physics;
	delete collision;
}

void CEntity::ADD_Sprite(CSprite* csprite)
{
	sprite = csprite;
	draw = true;
}

void CEntity::ADD_Text(CSprite* csprite)
{
	sprite = csprite;
	draw = true;
	sprite->ToggleDel();
}

void CEntity::ADD_Animation(CAnimate* canimate)
{
	animate = canimate;

	if (sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}

	ADD_Text(new CText());

	draw = true;
}

void CEntity::ADD_Physics(CPhysics* cphysics)
{
	physics = cphysics;
}

void CEntity::ADD_Collision(CCollision* ccollision)
{
	collision = ccollision;
	collision->SetColBoxPos(*position);
}

void CEntity::DELETE_Animation()
{
	delete animate;
	animate = NULL;
}

void CEntity::DELETE_Sprite()
{
	delete sprite;
	sprite = NULL;
}

void CEntity::DELETE_Physics()
{
	delete physics;
	physics = NULL;
}

void CEntity::DELETE_Collision()
{
	delete collision;
	collision = NULL;
}

bool CEntity::SolveCollisions(CEntity* e[])
{
	int size = sizeof(e) / sizeof(e[0]);
	for (int i = 0; i < size; i++)
	{
		for (int j = i+1; j < size; j++)
		{
			e[i]->HasCollided(e[j]);
		}
	}
	

	return false;
}

bool CEntity::HasCollided(CEntity* e)
{
	if (!e->GetCollision()->GetInsCol()) // outside collision
	{
		if ( (collision->GetX()+collision->GetW()) >= e->GetCollision()->GetX() && 
			 (collision->GetX()+collision->GetW()) <= e->GetCollision()->GetX()+e->GetCollision()->GetW() ||
			 (collision->GetX()) >= e->GetCollision()->GetX() && 
			 (collision->GetX()) <= e->GetCollision()->GetX()+e->GetCollision()->GetW() )
		{
			if ( (collision->GetY()+collision->GetH()) >= e->GetCollision()->GetY() && 
				 (collision->GetY()+collision->GetH()) <= e->GetCollision()->GetY()+e->GetCollision()->GetH() ||
				 (collision->GetY()) >= e->GetCollision()->GetY() && 
				 (collision->GetY()) <= e->GetCollision()->GetY()+e->GetCollision()->GetH() )
			{
				if (e->GetCollision()->GetBoundry())
				{

				}
				return true;
			}		
		} 

		return false;
	} else // inside collision
	{
		if ( ((collision->GetX()) <= e->GetCollision()->GetX() || (collision->GetX()+collision->GetW()) >= e->GetCollision()->GetX()+e->GetCollision()->GetW()) ||
		     ((collision->GetY()) <= e->GetCollision()->GetY() || (collision->GetY()+collision->GetH()) >= e->GetCollision()->GetY()+e->GetCollision()->GetH()) )
		{
			return true;		
		}
		return false;
	}
}

void CEntity::Draw(SDL_Renderer* pass_renderer)
{
	if (draw)
	{
		UpdateRectPos();
		if (animate != NULL)
		{	
			SDL_RenderCopy(pass_renderer, animate->GetTEX(), &animate->GetCROP(), &rect);
			if (debug)
			{
				// display collision box
				if (collision != NULL)
				{
					SDL_RenderCopy(pass_renderer, collision->GetColTex()->GetTEX(), NULL, &collision->GetColBox());
				}

				// display animation frames
				string message = to_string((long long)animate->GetRow());
				message += " ";
				message += to_string((long long)animate->GetFrame());

				sprite->Print(pass_renderer, message, 0,0,0);

				SDL_Rect pos = rect;
				pos.y += pos.h;




				SDL_RenderCopyEx (pass_renderer, sprite->GetTEX(), NULL, &pos, pos.x%360, NULL, SDL_FLIP_NONE);				
			}
		} else if (sprite != NULL)
		{
			SDL_RenderCopy(pass_renderer, sprite->GetTEX(), sprite->GetCROP(), &rect);
			if (debug && collision != NULL)
			{
				// display collision box
				SDL_RenderCopy(pass_renderer, collision->GetColTex()->GetTEX(), NULL,  &collision->GetColBox());
			}
		}
	}
}



