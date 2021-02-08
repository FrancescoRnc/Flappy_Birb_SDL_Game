#pragma once
#include <SDL.h>
#include <functional>
#include <stdint.h>

class GameObject;
struct InputAction;

struct Component
{
	GameObject* Owner = nullptr;
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
		RotationAngleDegree = new double{0};

		
	};

	SDL_Surface* Surface = nullptr;
	SDL_Texture* Texture = nullptr;
	SDL_Rect SrcRect = {};
	SDL_Rect DstRect = {};
	double* RotationAngleDegree = nullptr;
	SDL_Point* Point = nullptr;
	SDL_RendererFlip FlipRule;
	int RenderPriority = 0;
};

struct AnimatorComponent : public Component
{
	AnimatorComponent(std::vector<SDL_Texture*> _txs) : textures{_txs}
	{
		frames = textures.size();
	}

	SpriteComponent* rMainSprite = nullptr;
	std::vector<SDL_Texture*> textures;
	float SpeedRate = 0;
	size_t frames = 1;
	bool bLoop = false;

	size_t current_frame = 0;
	double frame_current_time = 0;
	double frame_max_time = 0;
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
	//SDL_Rect* Rect = nullptr;
	std::vector<SDL_Rect*> Rects;
	double HMoveSpeed = 0;
	double VMoveSpeed = 0;

	void Move(const double deltatime)
	{
		for (auto rect : Rects)
		{
			rect->x += (int)(HMoveSpeed * deltatime);
			rect->y += (int)(VMoveSpeed * deltatime);
		}
	}
};

struct RotatorComponent : public Component
{
	double RotationRate = 0;
	double* Rotation = new double{0};
	double MinAngle = 0.f;
	double MaxAngle = 360.f;
};

struct GravityComponent : public Component
{
	MovableComponent* refMovable = nullptr;
	double GravityForce = 0;

	void GravityStep(const double deltatime)
	{
		//if (bActive)
		//{
			refMovable->VMoveSpeed += GravityForce * deltatime;
		//}
		//else refMovable->VMoveSpeed = 0;
	}
};

struct CollisionComponent : public Component
{
	SDL_Rect* Rect = nullptr;
	//virtual void Intersection(SDL_Rect *other) = 0;
	std::function<void(SDL_Rect*)> OnCollision;

	uint8_t CollisionPin = 0;
	uint8_t CollisionBitmask = 0;
};

struct FlapComponent : public Component
{
	//FlapComponent() { DoFlap = new InputAction(); }

	GravityComponent* Gravity = nullptr;
	double FlapForce = 0;
	SDL_KeyCode KeyCode;
	InputAction* DoFlap = nullptr;
};