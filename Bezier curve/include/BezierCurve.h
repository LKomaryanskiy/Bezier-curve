#pragma once
#include <SFML/Graphics.hpp>
#include <ts_vector.h>
#include <atomic>


void bezier_curve(	const std::vector<sf::Vector2f> points,
					ts_vector<sf::Vector2f>& curve,
					float step,
					const std::vector<float> coefs,
					std::atomic<float>& curr_t);

std::vector<float> get_bin_coefs(int n);

int factorial(int n);