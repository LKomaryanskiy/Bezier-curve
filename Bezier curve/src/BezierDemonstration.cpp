#include "BezierDemonstration.h"

BezierDemonstration::BezierDemonstration()
{
	m_app_window = new sf::RenderWindow(sf::VideoMode(c_width, c_heigth), "Bezier", sf::Style::Titlebar | sf::Style::Close);
	m_app_window->resetGLStates();

	m_window = sfg::Window::Create();
	m_is_present_curve = false;
	CreateInterface();
};


BezierDemonstration::~BezierDemonstration()
{
	delete m_app_window;
};


void BezierDemonstration::Run()
{

};


void BezierDemonstration::CreateInterface()
{
	sfg::Box::Ptr control_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_window->Add(control_box);

	sfg::Button::Ptr draw_button = sfg::Button::Create();
	draw_button->SetLabel("Draw");
	draw_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&BezierDemonstration::DrawSignal, this));
	control_box->Pack(draw_button, false);

	//TODO:End this
};


void BezierDemonstration::AddPoint()
{
};


void BezierDemonstration::DrawSignal()
{
};


void BezierDemonstration::ClaerSignal()
{
};


void BezierDemonstration::ExitSignal()
{
};