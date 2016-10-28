#include "BezierDemonstration.h"

BezierDemonstration::BezierDemonstration()
{
	m_app_window = new sf::RenderWindow(sf::VideoMode(c_width, c_heigth), "Bezier", sf::Style::Titlebar | sf::Style::Close);
	m_app_window->setFramerateLimit(30);
	m_app_window->resetGLStates();

	m_window = sfg::Window::Create();

	m_font.loadFromFile(c_font_file_path);
	m_color = sf::Color::White;

	m_is_now_create = false;
	m_is_present_curve = false;
	CreateInterface();
	m_current_angle = 0;
	m_current_position = { 0, 0 };
	m_current_scale = 1;
	m_max_scale = 5;
};


BezierDemonstration::~BezierDemonstration()
{
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
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				AddPoint();

			m_window->HandleEvent(event);
		}

		ChangeStateLabel();
		JoinThread();
	
		m_window->Update(0.f);
		m_app_window->clear();
		DisplayCurve();
		DisplayPoints();
		m_sfgui.Display(*m_app_window);
		m_app_window->display();
	}
};


void BezierDemonstration::DisplayPoints()
{
	for (int i = 0; i < m_points.size(); ++i)
	{
		sf::Text text;
		text.setPosition(m_points[i] - sf::Vector2f(5 , 20));
		text.setString("P" + std::to_string(i));
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Blue);
		text.setStyle(sf::Text::Bold);
		text.setFont(m_font);

		sf::CircleShape shape(3);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(m_points[i] - sf::Vector2f(1, 1));
		m_app_window->draw(shape);
		m_app_window->draw(text);
	}
};

void BezierDemonstration::DisplayCurve()
{
	if (m_is_now_create || m_is_present_curve)
	{
		sf::Texture texture;
		if (m_is_now_create)
		{
			//Update pixels
			UpdateImage();
		}
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

	sfg::SpinButton::Ptr max_scale = sfg::SpinButton::Create(5.f, 100.f, 1.f);
	max_scale->SetDigits(0);
	max_scale->SetRequisition(sf::Vector2f(50.f, 0.f));
	max_scale->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BezierDemonstration::ChangeMaxScaleSignal, this, max_scale->GetValue()));

	control_box->Pack(draw_button);
	control_box->Pack(max_scale);
	control_box->Pack(clear_button);
	control_box->Pack(exit_button);

	sfg::Box::Ptr state_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_state_label = sfg::Label::Create();

	state_box->Pack(m_state_label);

	main_box->Pack(control_box);
	main_box->Pack(state_box);
};

void BezierDemonstration::JoinThread()
{
	if (m_is_present_curve) 
	{
		if (std::fabs(m_curr_t.load() - 1.0) <= 0.000001)
		{
			m_creating_thread->join();
			delete m_creating_thread;
			m_curr_t.exchange(0);
			m_is_now_create = false;
		}
	}
};

void BezierDemonstration::AddPoint()
{
	if (!m_is_present_curve)
	{
		if (sf::Mouse::getPosition(*m_app_window).x > 0 && sf::Mouse::getPosition(*m_app_window).x < m_app_window->getSize().x
			&&	sf::Mouse::getPosition(*m_app_window).y > 0 && sf::Mouse::getPosition(*m_app_window).y < m_app_window->getSize().y
			&&	!(sf::Mouse::getPosition(*m_app_window).x > m_window->GetAllocation().left 
				&& sf::Mouse::getPosition(*m_app_window).x < m_window->GetAllocation().left + m_window->GetAllocation().width
				&&	sf::Mouse::getPosition(*m_app_window).y > m_window->GetAllocation().top
				&& sf::Mouse::getPosition(*m_app_window).y < m_window->GetAllocation().top + m_window->GetAllocation().height))
		{
			m_points.push_back(sf::Vector2f(sf::Mouse::getPosition(*m_app_window).x, sf::Mouse::getPosition(*m_app_window).y));
		}
	}
};


void BezierDemonstration::DrawSignal()
{
	m_image.create(m_app_window->getSize().x, m_app_window->getSize().x);
	float step = 1.0 / (m_max_scale * 
		[&]()->float 
	{
		float length = 0;
		for (int i = 0; i < m_points.size(); ++i)
		{
			length += std::sqrt(std::pow(m_points[i].x, 2) + std::pow(m_points[i].y, 2));
		}
		return length;
	}());
	
	m_curve.resize(static_cast<int>((1 / step)));
	//Create pointer to thread
	std::vector<float> coefs = get_bin_coefs(m_points.size() - 1);
	m_creating_thread = new std::thread(bezier_curve, m_points, std::ref(m_curve), step, coefs, std::ref(m_curr_t));

	m_is_now_create = true;
	m_is_present_curve = true;
};


void BezierDemonstration::ClaerSignal()
{
	m_app_window->clear();
	m_is_present_curve = false;
	m_is_now_create = false;
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
	buf_string += "Amount of points: " + std::to_string(m_points.size()) + "\n";

	buf_string += "Current t: " + std::to_string(m_curr_t.load()) + "\n";
	//TODO:: add other info
	

	m_state_label->SetText(buf_string);
};

void BezierDemonstration::UpdateImage()
{
	int n = m_curve.size();
	sf::Vector2f buf;
	for (int i = 0; i < n; i++)
	{
		buf = m_curve.get_value(i);
		m_image.setPixel(buf.x, buf.y, m_color);
	}
};