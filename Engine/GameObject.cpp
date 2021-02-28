#include "GameObject.h"

int GameObject::Entities = 0;

GameObject::GameObject()
{
	Entity = ++Entities;
}

GameObject::~GameObject()
{

}