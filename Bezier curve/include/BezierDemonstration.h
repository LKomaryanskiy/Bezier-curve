#pragma once
#include <BezierCurve.h>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

/*
	The main class, which defines main application logic and functionality.
*/
class BezierDemonstration
{
public:
	BezierDemonstration();
	~BezierDemonstration();
	void Run();

private:
	sf::RenderWindow*			m_app_window;
	sfg::Window::Ptr			m_window;
	sfg::SFGUI					m_sfgui;
	bool						m_is_present_curve;
	std::vector<sf::Vector2f>	m_points;
	std::vector<sf::Vector2f>	m_curve;
	float						m_current_scale;
	float						m_current_angle;
	float						m_current_position;

	const  unsigned int			c_width = 800;
	const  unsigned int			c_heigth = 600;

	void CreateInterface();
	void AddPoint();
	void DrawSignal();
	void ClaerSignal();
	void ExitSignal();
};