#include "AbstractGame.hpp"

AbstractGame::~AbstractGame()
{
}

void AbstractGame::Exit( int exit_code_ )
{
	application_exit_code = exit_code_;
}
