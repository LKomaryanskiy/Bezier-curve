#pragma once
#include <BezierCurve.h>

#include <string>
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
	sfg::Label::Ptr				m_state_label;

	bool						m_is_present_curve;
	bool						m_is_now_create;
	std::atomic<float>			m_curr_t;

	sf::Image					m_image;
	
	std::vector<sf::Vector2f>	m_points;
	ts_vector<sf::Vector2f>		m_curve;
	float						m_max_scale;
	float						m_current_scale;
	float						m_current_angle;
	sf::Vector2f				m_current_position;

	sf::Font					m_font;
	sf::Color					m_color;

	std::thread*				m_action_thread;

	const  unsigned int			c_width = 800;
	const  unsigned int			c_heigth = 600;
	const std::string			c_font_file_path = "Content\\Font\\Y2K Neophyte Italic.ttf";

	void DisplayPoints();
	void DisplayCurve();
	void CreateInterface();

	void JoinThread();

	void AddPoint();

	void DrawSignal();
	void ClaerSignal();
	void ChangeMaxScaleSignal(float vlaue);
	void ChangeStateLabel();
	void UpdateImage();
};