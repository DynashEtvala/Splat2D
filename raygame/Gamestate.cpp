#include "Gamestate.h"

GameState::GameState() {}
GameState::GameState(GameState const&) {}
void GameState::operator=(GameState const&) {}

GState GameState::getState()
{
	GameState& instance = GetInstance();
	return instance.state;
}

GState GameState::setState(GState newState)
{
	GameState& instance = GetInstance();
	instance.state = newState;
	return instance.state;
}

GameState& GameState::GetInstance()
{
	static GameState instance;
	return instance;
}

GameState::~GameState()
{

}