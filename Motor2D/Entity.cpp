#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "p2Log.h"
#include "Stat.h"
#include "Entity.h"


Entity::Entity()
{
}


Entity::~Entity()
{
}

Entity::Entity(pugi::xml_node entity_node, fPoint position, Faction faction)
{
	this->faction = faction;
	this->position = position;

	LoadAnimations(entity_node);
}

bool Entity::PostUpdate()
{
	Draw();

	return true;
}

void Entity::DecreaseLife(float damage, bool piercing)
{
	float damage_received = CalculateDamage(damage, 0);
	stats.find("health")->second->DecreaseStat(damage_received);
	if (stats.find("health")->second->GetValue() <= 0)
		Die();
}
void Entity::SetDebug(bool value)
{
	debug = value;
}
bool Entity::IsAlive()
{
	return (stats.find("health")->second->GetValue() > 0);
}

fPoint Entity::GetPosition()
{
	fPoint ret;
	switch (pivot)
	{
	case PVT_BOTTOM_CENTER:
		ret = { position.x - current_frame.w*0.5f, position.y - current_frame.h };
		break;
	case PVT_CENTER:
		ret = { position.x - current_frame.w*0.5f, position.y - current_frame.h * 0.5f };
		break;
	default:
		ret = position;
		break;
	}

	return ret;
}
void Entity::SetPivot(PivotType type)
{
	pivot = type;
}
fPoint Entity::GetCenterPosition()
{
	fPoint ret;
	switch (pivot)
	{
	case PVT_BOTTOM_CENTER:
		ret = { position.x, position.y + current_frame.h * 0.5f };
		break;
	case PVT_CENTER:
		ret = position;
		break;
	default:
		ret = position;
		break;
	}

	return ret;
}
float Entity::CalculateDamage(float attack, float defense)
{
	return (attack - ((attack * defense) / 25));
}

void Entity::LoadAnimations(pugi::xml_node anim_config)
{
	//load animations
	pugi::xml_node animation;
	for (animation = anim_config.child("animations").first_child(); animation; animation = animation.next_sibling())
	{
		Animation anim;
		pugi::xml_node frame;
		for (frame = animation.child("sprite"); frame; frame = frame.next_sibling("sprite"))
		{
			anim.PushBack({ frame.attribute("x").as_int(), frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });
		}
		anim.speed = animation.attribute("speed").as_float();
		anim.base_speed = anim.speed;
		anim.loop = animation.attribute("loop").as_bool(true);
		std::string animation_name = animation.attribute("name").as_string();

		animations.insert({ animation_name, anim });
	}
}

void Entity::Draw()
{
	fPoint render_position = GetPosition();
	App->render->Blit(sprite, render_position.x, render_position.y, &current_frame);
	if(debug)
		App->render->DrawCircle(position.x, position.y, 5, 255, 0, 255);
}