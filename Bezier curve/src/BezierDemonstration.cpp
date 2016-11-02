#include "BezierDemonstration.h"

BezierDemonstration::BezierDemonstration()
{
	m_app_window = new sf::RenderWindow(sf::VideoMode(c_width, c_heigth), "Bezier", sf::Style::Titlebar | sf::Style::Close);
	m_app_window->setFramerateLimit(c_frame_limlit);
	m_app_window->resetGLStates();

	m_window = sfg::Window::Create();

	m_font.loadFromFile(c_font_file_path);
	m_color = sf::Color::White;

	m_is_now_change = false;
	m_is_present_curve = false;
	m_is_thread_done = true;
	m_is_move_event = false;
	m_is_rotate_event = false;
	m_is_need_update = false;

	CreateInterface();
	m_current_angle = 0;
	m_current_position = { 0, 0 };
	m_current_scale = 1;
	m_max_scale = 5;
};


BezierDemonstration::~BezierDemonstration()
{
	JoinThreads();
	delete m_app_window;
};


void BezierDemonstration::Run()
{
	sf::Event event;
	while (m_app_window->isOpen())
	{
		while (m_app_window->pollEvent(event))
		{
			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed)
				m_app_window->close();
			MouseEvents(event);
			m_window->HandleEvent(event);
		}

		ChangeStateLabel();
	
		m_window->Update(0.f);
		m_app_window->clear();
		DisplayCurve();
		DisplayPoints();
		m_sfgui.Display(*m_app_window);
		m_app_window->display();

		JoinThreads();
	}
};


void BezierDemonstration::DisplayPoints()
{
	for (int i = 0; i < m_points.size(); ++i)
	{
		sf::Text text;
		text.setPosition(m_points.get_value(i) - sf::Vector2f(5 , 20));
		text.setString("P" + std::to_string(i));
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Blue);
		text.setStyle(sf::Text::Bold);
		text.setFont(m_font);

		sf::CircleShape shape(3);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(m_points.get_value(i) - sf::Vector2f(1, 1));
		m_app_window->draw(shape);
		m_app_window->draw(text);
	}
};

void BezierDemonstration::DisplayCurve()
{
	if (m_is_now_change || m_is_present_curve)
	{
		sf::Texture texture;
		//Update pixels
		UpdateImage();
		texture.loadFromImage(m_image);
		sf::Sprite sprite(texture);
		m_app_window->draw(sprite);
	}
};

void BezierDemonstration::CreateInterface()
{
	sfg::Box::Ptr main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_window->Add(main_box);

	sfg::Box::Ptr control_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

	sfg::Button::Ptr draw_button = sfg::Button::Create("Draw");
	draw_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&BezierDemonstration::DrawSignal, this));

	sfg::Button::Ptr clear_button = sfg::Button::Create("Clear");
	clear_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&BezierDemonstration::ClaerSignal, this));
	
	sfg::Button::Ptr exit_button = sfg::Button::Create("Exit");
	exit_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&sf::RenderWindow::close, m_app_window));

	control_box->Pack(draw_button);
	control_box->Pack(clear_button);
	control_box->Pack(exit_button);

	sfg::Box::Ptr max_scale_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

	sfg::Label::Ptr max_scale_label = sfg::Label::Create("Max scale: ");
	sfg::SpinButton::Ptr max_scale = sfg::SpinButton::Create(5.f, 50.f, 1.f);
	max_scale->SetDigits(0);
	max_scale->SetRequisition(sf::Vector2f(50.f, 0.f));
	max_scale->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BezierDemonstration::ChangeMaxScaleSignal, this, max_scale->GetValue()));

	max_scale_box->Pack(max_scale_label);
	max_scale_box->Pack(max_scale);

	sfg::Box::Ptr state_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_state_label = sfg::Label::Create();

	state_box->Pack(m_state_label);

	main_box->Pack(control_box);
	main_box->Pack(max_scale_box);
	main_box->Pack(state_box);
};

void BezierDemonstration::JoinThreads()
{
	if (m_is_present_curve) 
	{
		if (m_is_thread_done)
		{
			m_action_thread->join();
			delete m_action_thread;
			m_is_now_change = false;
			m_is_thread_done = false;
		}
	}
};

void BezierDemonstration::MouseEvents(sf::Event event)
{
	//Check present mouse in Render window
	if (sf::Mouse::getPosition(*m_app_window).x > 0 && sf::Mouse::getPosition(*m_app_window).x < m_app_window->getSize().x
		&&	sf::Mouse::getPosition(*m_app_window).y > 0 && sf::Mouse::getPosition(*m_app_window).y < m_app_window->getSize().y
		&& !(sf::Mouse::getPosition(*m_app_window).x > m_window->GetAllocation().left
			&& sf::Mouse::getPosition(*m_app_window).x < m_window->GetAllocation().left + m_window->GetAllocation().width
			&&	sf::Mouse::getPosition(*m_app_window).y > m_window->GetAllocation().top
			&& sf::Mouse::getPosition(*m_app_window).y < m_window->GetAllocation().top + m_window->GetAllocation().height))
	{
		if (!m_is_present_curve)
		{
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_points.size() < c_max_points)
			{
				AddPoint();
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !m_is_move_event && !m_is_rotate_event)
		{
			m_click_pos = sf::Vector2f(sf::Mouse::getPosition());
			m_is_move_event = true;
		}
		else if (event.type == sf::Event::MouseWheelMoved && m_current_scale + event.mouseWheel.delta / 25.0 <= m_max_scale)
		{
			m_image.create(m_app_window->getSize().x, m_app_window->getSize().y);
			float previous_scale = m_current_scale;
			m_current_scale += event.mouseWheel.delta / 25.0;
			Scaling(m_current_scale / previous_scale);
		}
		else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !m_is_move_event && !m_is_rotate_event)
		{
			m_click_pos = sf::Vector2f(sf::Mouse::getPosition());
			m_is_rotate_event = true;
		}
		
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && m_is_move_event)
		{
			m_image.create(m_app_window->getSize().x, m_app_window->getSize().y);
			m_current_position += sf::Vector2f(sf::Mouse::getPosition()) - m_click_pos;
			Moving(sf::Vector2f(sf::Mouse::getPosition()) - m_click_pos);
			m_is_move_event = false;
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && m_is_rotate_event)
		{
			m_image.create(m_app_window->getSize().x, m_app_window->getSize().y);
			m_current_angle += (sf::Vector2f(sf::Mouse::getPosition()).x - m_click_pos.x) / 500.0;
			Rotation((sf::Vector2f(sf::Mouse::getPosition()).x - m_click_pos.x) / 500.0);
			m_is_rotate_event = false;
		}
	}
};

void BezierDemonstration::AddPoint()
{
	m_points.push_back(sf::Vector2f(sf::Mouse::getPosition(*m_app_window).x, sf::Mouse::getPosition(*m_app_window).y));
}

void BezierDemonstration::Scaling(float scale_factor)
{
	if (!m_is_now_change)
	{
		m_action_thread = new std::thread(
			scale,
			std::ref(m_curve),
			scale_factor,
			std::ref(m_is_thread_done));

		scale(m_points, scale_factor, std::atomic_bool());

		m_is_now_change = true;
		m_is_need_update = true;
	}
};

void BezierDemonstration::Moving(sf::Vector2f offset)
{
	if (!m_is_now_change)
	{
		m_action_thread = new std::thread(
			moving,
			std::ref(m_curve),
			offset,
			std::ref(m_is_thread_done));

		moving(m_points, offset, std::atomic_bool());

		m_is_now_change = true;
		m_is_need_update = true;
	}
};

void BezierDemonstration::Rotation(float angle)
{
	if (!m_is_now_change)
	{
		m_action_thread = new std::thread(
			rotation,
			std::ref(m_curve),
			angle,
			std::ref(m_is_thread_done));

		rotation(m_points, angle, std::atomic_bool());

		m_is_now_change = true;
		m_is_need_update = true;
	}
};

void BezierDemonstration::DrawSignal()
{
	if (m_points.size() >= 2) {
		m_image.create(m_app_window->getSize().x, m_app_window->getSize().y);
		float step = 1.0 / (m_max_scale *
			[&]()->float
		{
			float length = 0;
			for (int i = 0; i < m_points.size(); ++i)
			{
				length += std::sqrt(std::pow(m_points.get_value(i).x, 2) + std::pow(m_points.get_value(i).y, 2));
			}
			return length;
		}());

		m_curve.resize(static_cast<int>((1 / step)));
		std::vector<float> coefs = get_bin_coefs(m_points.size() - 1);
		//Set thread parameters
		m_action_thread = new std::thread(
			bezier_curve,
			std::ref(m_points),
			std::ref(m_curve),
			step, coefs,
			std::ref(m_curr_t),
			std::ref(m_is_thread_done));

		m_is_now_change = true;
		m_is_present_curve = true;
		m_is_need_update = true;
	}
};


void BezierDemonstration::ClaerSignal()
{
	m_app_window->clear();
	m_is_present_curve = false;
	m_is_now_change = false;
	m_is_need_update = false;
	m_points.clear();
	m_curve.clear();
	m_current_angle = 0;
	m_current_position = { 0, 0 };
	m_current_scale = 1;
	m_curr_t.exchange(0);
};

void BezierDemonstration::ChangeMaxScaleSignal(float value)
{
	m_max_scale = value;
};

void BezierDemonstration::ChangeStateLabel()
{
	std::string buf_string;

	//Mouse position info
	buf_string += "Mouse pos: ";
	if (sf::Mouse::getPosition(*m_app_window).x > 0 && sf::Mouse::getPosition(*m_app_window).x < m_app_window->getSize().x
		&&	sf::Mouse::getPosition(*m_app_window).y > 0 && sf::Mouse::getPosition(*m_app_window).y < m_app_window->getSize().y)
	{
		buf_string += std::to_string(sf::Mouse::getPosition(*m_app_window).x);
		buf_string += "  ";
		buf_string += std::to_string(sf::Mouse::getPosition(*m_app_window).y);
	}
	else
	{
		buf_string += "  0    0";
	}
	buf_string += "\n";
	//Points info
	buf_string += "Amount of points: " + std::to_string(m_points.size()) + "/" + std::to_string(c_max_points) + "\n";

	buf_string += "Current t: " + std::to_string(m_curr_t.load()) + "\n";
	buf_string += "Rotation: " + std::to_string(m_current_angle) + "\n";
	buf_string += "Scale: " +  std::to_string(m_current_scale) + "\n";
	buf_string += "Position: " + std::to_string(m_current_position.x) + "  " + std::to_string(m_current_position.y) + "\n";

	//Current action status
	buf_string += "Status: ";
	if (!m_is_present_curve)
	{
		buf_string += "Clean screen.\n";
	}
	else if (m_is_present_curve)
	{
		buf_string += "Bezier curve is on screen,\n";
	}

	buf_string += "Event: ";

	if (m_is_move_event)
	{
		buf_string += "move to " + std::to_string(sf::Vector2f(sf::Mouse::getPosition()).x - m_click_pos.x) + "  " + 
			std::to_string(sf::Vector2f(sf::Mouse::getPosition()).y - m_click_pos.y) + "\n";
	}
	else if (m_is_rotate_event)
	{
		buf_string += "rotate to " + std::to_string((sf::Vector2f(sf::Mouse::getPosition()).x - m_click_pos.x) / 500.0) + "\n";
	}
	else
	{
		buf_string += "none\n";
	}
	
	m_state_label->SetText(buf_string);
};

void BezierDemonstration::UpdateImage()
{
	if (m_is_need_update) 
	{
		int n = m_curve.size();
		sf::Vector2f buf;
		sf::Vector2f prev_pixel = m_curve.get_value(0);
		for (int i = 0; i < n; i++)
		{
			buf = m_curve.get_value(i);
			if (std::fabs(buf.x - prev_pixel.x) <= 1 && std::fabs(buf.y - prev_pixel.y) && (std::fabs(buf.x - prev_pixel.x) >= 0.8 || std::fabs(buf.y - prev_pixel.y) >= 0.8))
			{
				if (buf.x >= 0 && buf.x < m_image.getSize().x && buf.y >= 0 && buf.y < m_image.getSize().y)
				{
					m_image.setPixel(buf.x, buf.y, m_color);
				}
				prev_pixel = buf;
			}
		}
	}
	if (m_is_thread_done && m_is_now_change && m_is_need_update)
	{
		m_is_need_update = false;
	}
};