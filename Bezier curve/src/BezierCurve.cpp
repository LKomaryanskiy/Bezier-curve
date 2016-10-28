#include <BezierCurve.h>

void bezier_curve(const std::vector<sf::Vector2f> points,
	ts_vector<sf::Vector2f>& curve,
	float step,
	const std::vector<float> coefs,
	std::atomic<float>& curr_t)
{
	curr_t = 0.0;
	int n = coefs.size();
	int n_curve = curve.size();
	//Create buf variable, in order to decrease the amounts of lock vector
	sf::Vector2f buf;

	for (int i = 0; i < n_curve; ++i)
	{
		buf = { 0, 0 };
		for (int i = 0; i < n; i++)
		{
			buf += points[i] * coefs[i] * std::powf((1 - curr_t), (n - i - 1)) * std::powf(curr_t, i);
		}
		curr_t.exchange(curr_t.load() + step);
		curve.set_value(i, buf);
	}
	curr_t.exchange(1.0);
};

std::vector<float> get_bin_coefs(int n)
{
	std::vector<float> ret_value(n + 1);
	int n_factorial = factorial(n);
	for (int i = 0; i <= n; ++i)
	{
		ret_value[i] = n_factorial / (factorial(i) * factorial(n - i));
	}
	return ret_value;
};


int factorial(int n)
{
	int value = 1;
	for (int i = 1; i <= n; ++i)
	{
		value *= i;
	}
	return value;
};
