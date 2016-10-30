#pragma once
#include <BezierCurve.h>
#include <Transform.h>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <memory>

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
	bool						m_is_now_change;
	std::atomic_bool			m_is_thread_done;
	bool						m_is_move_event;
	bool						m_is_rotate_event;
	bool						m_is_need_update;

	std::atomic<float>			m_curr_t;

	sf::Image					m_image;
	
	ts_vector<sf::Vector2f>		m_points;
	ts_vector<sf::Vector2f>		m_curve;
	float						m_max_scale;
	float						m_current_scale;
	float						m_current_angle;
	sf::Vector2f				m_current_position;

	//For transformation
	sf::Vector2f				m_click_pos;

	sf::Font					m_font;
	sf::Color					m_color;

	std::thread*				m_action_thread;

	const unsigned int			c_width = 800;
	const unsigned int			c_heigth = 600;
	const unsigned int			c_frame_limlit = 30;
	const std::string			c_font_file_path = "Content\\Font\\Y2K Neophyte Italic.ttf";

	void DisplayPoints();
	void DisplayCurve();
	void CreateInterface();

	void JoinThreads();

	void MouseEvents(sf::Event event);
	void AddPoint();
	void Scaling(float scale_factor);
	void Moving(sf::Vector2f offset);
	void Rotation(float angle);


	void DrawSignal();
	void ClaerSignal();
	void ChangeMaxScaleSignal(float vlaue);
	void ChangeStateLabel();
	void UpdateImage();
};