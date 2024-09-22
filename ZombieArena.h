#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.h"

using namespace sf;

int createBackground(VertexArray &rVA, IntRect Arena);

Zombie* createHorde(int numZombie, IntRect arena);
