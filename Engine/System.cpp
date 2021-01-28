#include "System.h"
#include "Engine.h"
#include <iostream>

void Input_System(My_ecs& instance)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			//SDL_Quit();
		}
		/*else if (event.type == SDL_KEYDOWN)
		{
			instance.for_each<Cmp_InputReceiver_UpDown, Cmp_Velocity>(
				[event](auto &entity, Cmp_InputReceiver_UpDown &input, Cmp_Velocity &vel) mutable
			{
				vel.DeltaY = -((event.key.keysym.sym == input.MoveUpButton) -
					(event.key.keysym.sym == input.MoveDownButton)) * vel.MaxSpeed;
			});
		}
		if (event.type == SDL_KEYUP)
		{
			instance.for_each<Cmp_InputReceiver_UpDown, Cmp_Velocity>(
				[event](auto &entity, Cmp_InputReceiver_UpDown &input, Cmp_Velocity &vel) mutable
			{
				if (event.key.keysym.sym == input.MoveUpButton || event.key.keysym.sym == input.MoveDownButton)
					vel.DeltaY = 0;
			});
		}*/

		instance.for_each<Cmp_InputReceiver_UpDown, Cmp_Velocity>(
			[event](auto &entity, Cmp_InputReceiver_UpDown &input, Cmp_Velocity &vel) mutable
		{
		if (event.type == SDL_KEYDOWN)
		{
			vel.DeltaY = -((event.key.keysym.sym == input.MoveUpButton) -
				(event.key.keysym.sym == input.MoveDownButton)) * vel.MaxSpeed;
		}
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == input.MoveUpButton || event.key.keysym.sym == input.MoveDownButton)
				vel.DeltaY = 0;
		}
		});
	}
}

void Movement_System(My_ecs& instance, const float deltatime)
{
	instance.for_each<Cmp_Position, Cmp_Velocity, Cmp_Sprite>(
		[deltatime](auto &entity,
					Cmp_Position &pos,
					Cmp_Velocity &vel,
					Cmp_Sprite &spr) mutable
	{
		pos.X += vel.DeltaX * deltatime;
		pos.Y += vel.DeltaY * deltatime;

		spr.Rect.x = pos.X;
		spr.Rect.x = spr.Rect.x % (Engine::WinUtils.Width - spr.Rect.w);
		spr.Rect.y = pos.Y;
		spr.Rect.y = spr.Rect.y % (Engine::WinUtils.Height - spr.Rect.h);
	});
}

void Sprite_Renderer_System(SDL_Renderer* renderer, My_ecs& instance)
{
	instance.for_each<Cmp_Sprite>(
		[renderer](auto, auto &spr) mutable
	{
		SDL_SetRenderDrawColor(renderer, spr.r, spr.g, spr.b, spr.a);
		SDL_RenderFillRect(renderer, &spr.Rect);
	});
}

void BouncingBall_System(My_ecs& instance)
{
	instance.for_each<Cmp_BouncingBall, Cmp_Sprite, Cmp_Position, Cmp_Velocity>(
		[](auto entity,
		   Cmp_BouncingBall &bb,
		   Cmp_Sprite &spr,
		   Cmp_Position &pos,
		   Cmp_Velocity &vel) mutable
	{
		// Half ball size W/H -> 20 | Window Width -> 512 | Window Height -> 512
		int offset = 20;

		if (pos.Y <= bb.TopLimit || pos.Y >= bb.BottomLimit) vel.DeltaY = -vel.DeltaY;
		if (pos.X <= bb.LeftLimit || pos.X >= bb.RightLimit)
		{
			pos.X = Engine::WinUtils.Width / 2;
			pos.Y = Engine::WinUtils.Height / 2;
			vel.DeltaX = -vel.DeltaX;
		}
	});
}

void Collision_System(My_ecs& instance)
{
	instance.for_each<Cmp_BallPlayersCollision, Cmp_Sprite, Cmp_Position, Cmp_Velocity>(
		[instance](auto &entity,
				   Cmp_BallPlayersCollision &bc,
				   Cmp_Sprite &spr,
				   Cmp_Position &pos,
				   Cmp_Velocity &vel) mutable
	{

		if (pos.X <= Engine::WinUtils.Player1StartX + spr.Rect.w)
		{
			auto p1pos = instance.Get<Cmp_Position>(bc.player1_entity);

			if (pos.Y >= p1pos.Y - (spr.Rect.h / 2) && pos.Y <= p1pos.Y + 160)
			{
				vel.DeltaX = -vel.DeltaX;
				std::cout << "bounce!" << std::endl;
			}
		}
		else if (pos.X >= Engine::WinUtils.Player2StartX - spr.Rect.w)
		{
			auto p2pos = instance.Get<Cmp_Position>(bc.player2_entity);

			if (pos.Y >= p2pos.Y - (spr.Rect.h / 2) && pos.Y <= p2pos.Y + 160)
			{
				vel.DeltaX = -vel.DeltaX;
				std::cout << "bounce!" << std::endl;
			}
		}

	});

	//instance.for_each<Cmp_PlayerCollision, Cmp_Sprite, Cmp_Position, Cmp_Velocity>(
	//[instance](auto &entity,
	//		   Cmp_PlayerCollision &pc,
	//		   Cmp_Sprite &spr,
	//		   Cmp_Position &pos,
	//		   Cmp_Velocity &vel) mutable
	//{
	//	//auto ballpos = instance.Get<Cmp_Position>(pc.other_entity);
	//});

}