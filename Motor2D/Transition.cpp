#include "Transition.h"
#include "TransitionManager.h"
#include "j1App.h"
#include "p2Log.h"


Transition::Transition()
{
}

Transition::Transition(float transition_time)
{
	this->transition_time = transition_time;

	//start timer
	current_time = new Timer();
	current_time->Start();

	state = TransitionState::ENTERING;

}


Transition::~Transition()
{
	delete current_time;
}

void Transition::PreUpdate()
{
}

void Transition::Update()
{

}

void Transition::PostUpdate()
{
	switch (state)
	{
	case Transition::TransitionState::NONE:
		break;
	case Transition::TransitionState::ENTERING:
		Entering();
		break;
	case Transition::TransitionState::ACTION:
		Action();
		break;
	case Transition::TransitionState::EXITING:
		Exiting();
		break;
	default:
		break;
	}
}

void Transition::Entering()
{
	if (current_time->ReadSec() >= transition_time)
	{
		state = TransitionState::ACTION;
	}
}

void Transition::Action()
{
	current_time->Start();

	state = TransitionState::EXITING;
}

void Transition::Exiting()
{
	if (current_time->ReadSec() >= transition_time)
	{
		state = TransitionState::NONE;
		App->transition_manager->DestroyTransition(this);
	}
}

float Transition::LerpValue(float percent, float start, float end)
{
	return start + percent * (end - start);
}

iPoint Transition::Lerp(float percent, iPoint origin, iPoint destination)
{
	return { (int)(origin.x + percent * (destination.x - origin.x)),  (int)(origin.y + percent * (destination.y - origin.y)) };
}
