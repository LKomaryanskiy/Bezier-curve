#pragma once
#include <ts_vector.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <atomic>

void scale(ts_vector<sf::Vector2f>& points, float scale_factor, std::atomic_bool& done);

void moving(ts_vector<sf::Vector2f>& points, sf::Vector2f offset, std::atomic_bool& done);

void rotation(ts_vector<sf::Vector2f>& points, float angle, std::atomic_bool& done);