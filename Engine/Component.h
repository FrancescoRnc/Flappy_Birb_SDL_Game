#pragma once
#include <SDL.h>
#include <functional>
//#include "InputHandler.h"
#include <stdint.h>

class GameObject;
struct InputAction;

struct Component
{
	GameObject* Owner;
	bool bActive = true;
};

struct SpriteComponent : public Component
{
	SpriteComponent() {}
	SpriteComponent(SDL_Surface* _surface, SDL_Texture* _texture,
					SDL_Rect _srcRect, SDL_Rect _dstRect,
					SDL_Point* _point, SDL_RendererFlip _flipRule) :
		Surface{_surface}, Texture{_texture}, 
		SrcRect{_srcRect}, DstRect{_dstRect}, 
		Point{_point}, FlipRule{_flipRule} 
	{
		int w, h;
		auto query = SDL_QueryTexture(Texture, nullptr, nullptr, &w, &h);
		DstRect.w = SrcRect.w = w;
		DstRect.h = SrcRect.h = h;
	};

	SDL_Surface* Surface;
	SDL_Texture* Texture;
	SDL_Rect SrcRect;
	SDL_Rect DstRect;
	SDL_Point* Point;
	SDL_RendererFlip FlipRule;
	int RenderPriority;
};

struct AnimatorComponent : public Component
{
	AnimatorComponent(std::vector<SDL_Texture*> _textures, bool _loop) :
		textures{_textures}, bLoop{_loop} 
	{
		frames = textures.size();
	}
	std::vector<SDL_Texture*> textures;
	float SpeedRate;
	int frames = 1;
	bool bLoop = false;
};

struct RelocatableComponent : public Component
{
	std::vector<SDL_Rect*> Rects;
	int LimitX = 0;
	int LimitY = 0;
	std::function<bool(SDL_Rect*)> CheckPosition;
	std::function<void(SDL_Rect*)> RelocateSprites;
};

struct MovableComponent : public Component
{
	SDL_Rect* Rect;
	std::vector<SDL_Rect*> Rects;
	double HMoveSpeed = 0;
	double VMoveSpeed = 0;
};

struct RotatorComponent : public Component
{
	double RotationRate = 0;

};

struct GravityComponent : public Component
{
	MovableComponent* Movable;
	double GravityForce = 0;
};

struct CollisionComponent : public Component
{
	SDL_Rect* Rect;
	//virtual void Intersection(SDL_Rect *other) = 0;
	std::function<void(SDL_Rect*)> OnCollision;

	Uint8 CollisionPin;
	Uint8 CollisionBitmask;
};

struct FlapComponent : public Component
{
	//FlapComponent() { DoFlap = new InputAction(); }

	GravityComponent* Gravity;
	double FlapForce = 0;
	InputAction* DoFlap;
};