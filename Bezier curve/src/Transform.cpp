#include <Transform.h>

void scale(ts_vector<sf::Vector2f>& points, float scale_factor, std::atomic_bool& done)
{
	done = false;
	int n = points.size();
	for (int i = 0; i < n; ++i)
	{
		points.set_value(i, points.get_value(i) * scale_factor);
	}
	done = true;
};

void moving(ts_vector<sf::Vector2f>& points, sf::Vector2f offset, std::atomic_bool& done)
{
	done = false;
	int n = points.size();
	for (int i = 0; i < n; ++i)
	{
		points.set_value(i, points.get_value(i) + offset);
	}
	done = true;
};

void rotation(ts_vector<sf::Vector2f>& points, float angle, std::atomic_bool& done)
{
	done = false;
	int n = points.size();
	sf::Vector2f buf;
	for (int i = 0; i < n; ++i)
	{
		buf = points.get_value(i);
		buf = {buf.x * std::cos(angle) - buf.y * std::sin(angle), buf.x * std::sin(angle) + buf.y * std::cos(angle)};
		points.set_value(i, buf);
	}
	done = true;
};
