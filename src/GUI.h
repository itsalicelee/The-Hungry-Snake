#ifndef DSAP_GUI_H
#define DSAP_GUI_H

#include "Config.h"

#ifdef ENABLE_GUI

#include "Game.h"
#include "PlayerController.h"

int StartPlayerMode(Game& game, const std::shared_ptr<PlayerController>& playerController);
int StartCustomMode(Game& game, const std::shared_ptr<ISnakeController>& playerController);

#endif
#endif // DSAP_GUI_H
