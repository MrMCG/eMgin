#include "Entity.h"
#include "stdafx.h"
#include "settings.h"

using namespace settings;

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

	draw = false;
	debug = false;


	angle = 0;
}

CEntity::CEntity(const int x, const int y, const int w, const int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	// position is centre of rectangle
	position = new Vector2D(0,0);
	SetPosToCentre();
	sprite = NULL;
	animate = NULL;
	physics = NULL;

	draw = false;
	debug = false;


	angle = 0;
}

CEntity::~CEntity(void)
{
	delete position;
	delete sprite;
	delete animate;
	delete physics;
}

void CEntity::ADD_Sprite(CSprite* csprite)
{
	DELETE_Sprite();
	sprite = csprite;
	draw = true;
}

void CEntity::ADD_Text(CSprite* csprite)
{
	DELETE_Sprite();
	sprite = csprite;
	draw = true;
	sprite->ToggleDel();
}

void CEntity::ADD_Animation(CAnimate* canimate)
{
	DELETE_Animation();
	animate = canimate;

	if (sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}

	ADD_Text(new CText());

	draw = true;
}

void CEntity::ADD_Physics(CPhysics* cphysics, bool flag)
{
	DELETE_Physics();
	physics = cphysics;

	if (flag)
	{
		UpdatePosition();

		UpdateImageSize();
	} 
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

bool CEntity::CollidesWith(CEntity* entity)
{
	if (physics != NULL)
	{
		for (b2ContactEdge* edge = physics->GetBody()->GetContactList(); edge; edge = edge->next)
		{
			if (edge->contact->IsTouching())
			{
				b2Body* bodyA = edge->contact->GetFixtureA()->GetBody();
				b2Body* bodyB = edge->contact->GetFixtureB()->GetBody();

				if ( (bodyA == physics->GetBody() && bodyB == entity->GetPhysics()->GetBody()) ||
					 (bodyB == physics->GetBody() && bodyA == entity->GetPhysics()->GetBody()))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CEntity::UpdatePosition()
{
	if (physics != NULL)
	{
		b2Vec2 newPosition = physics->GetBody()->GetPosition();
		float32 angle = physics->GetBody()->GetAngle();
		float x = TILE_PIXEL_METER * newPosition.x; // use TILE_PIXEL_METER to link box2D meters coor to SDL pixels
		float y = TILE_PIXEL_METER * (newPosition.y-(newPosition.y*2)); // SDL y axis is opposite to Box2D
		float test = TILE_PIXEL_METER * abs(newPosition.y);
		position->setVec(Vector2D(x,y));
				
		setAngle(angle);
	} else
	{
		printf_s("ERROR:CEntity:physics - physics null for update");
	}
}

void CEntity::SetRectFromPos()
{
	int X = (int)	position->getX();
	float decimalX = position->getX() - X;
	
	int Y = (int)	position->getY();
	float decimalY = position->getY() - Y;

	if (decimalX >= 0.5)
	{
		X++;
	}
	if (decimalY >= 0.5)
	{
		Y++;
	}

	rect.x = X - (rect.w / 2);
	rect.y = Y - (rect.h / 2);
}

void CEntity::UpdateImageSize(float scale)
{
	if (physics != NULL)
	{
		rect.w = (physics->GetWidth() * TILE_PIXEL_METER) * scale;
		rect.h = (physics->GetHeight() * TILE_PIXEL_METER) * scale;
	} else
	{
		printf_s("ERROR:CEntity:physics - physics null for update");
	}
}

void CEntity::UpdateImageSize(float scaleX, float scaleY)
{
	if (physics != NULL)
	{
		rect.w = (physics->GetWidth() * TILE_PIXEL_METER) * scaleX;
		rect.h = (physics->GetHeight() * TILE_PIXEL_METER) * scaleY;
	} else
	{
		printf_s("ERROR:CEntity:physics - physics null for update");
	}
}

void* CEntity::GetPhysicsData() const
{
	if (physics != NULL)
	{
		return physics->GetBody()->GetUserData();
	} 
}

bool CEntity::SolveCollisions(CEntity* e[])
{
	int size = sizeof(e) / sizeof(e[0]);
	for (int i = 0; i < size; i++)
	{
		for (int j = i+1; j < size; j++)
		{
			
		}
	}
	
	return false;
}

void CEntity::Draw(SDL_Renderer* pass_renderer)
{
	if (draw)
	{
		SetRectFromPos();
		if (animate != NULL)
		{	
			SDL_RenderCopyEx (pass_renderer, animate->GetTEX(), &animate->GetCROP(), &rect, abs(fmod(angle,360)), NULL, SDL_FLIP_NONE);
			if (debug)
			{
				// display collision box
				if (physics != NULL && physics->GetDebugSprite() != NULL)
				{
					physics->UpdateDebugPos();
					SDL_RenderCopyEx (pass_renderer, physics->GetDebugSprite()->GetTEX(), physics->GetDebugSprite()->GetCROP(), &physics->GetColBox(), abs(fmod(angle,360)), NULL, SDL_FLIP_NONE);
				}

				// display animation frames
				string message = to_string((long long)animate->GetRow());
				message += " ";
				message += to_string((long long)animate->GetFrame());

				sprite->Print(pass_renderer, message);

				SDL_Rect pos = rect;
				pos.y += pos.h;
				pos.w = 40;
				pos.h = 40;

				SDL_RenderCopy(pass_renderer, sprite->GetTEX(), NULL, &pos);				
			}
		} else if (sprite != NULL)
		{
			SDL_RenderCopyEx (pass_renderer, sprite->GetTEX(), sprite->GetCROP(), &rect, abs(fmod(angle,360)), NULL, SDL_FLIP_NONE);
			if (debug && physics != NULL && physics->GetDebugSprite() != NULL)
			{
				// display collision box
				physics->UpdateDebugPos();
				SDL_RenderCopyEx (pass_renderer, physics->GetDebugSprite()->GetTEX(), physics->GetDebugSprite()->GetCROP(), &physics->GetColBox(), abs(fmod(angle,360)), NULL, SDL_FLIP_NONE);
			}
		}
	}
}



