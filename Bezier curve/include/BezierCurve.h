#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/*
	Return pixels positions, precision defines max scaling wihout loss good looks
*/
std::vector<sf::Vector2f> bezier_curve(std::vector<sf::Vector2f> points, float precision = 1.0);