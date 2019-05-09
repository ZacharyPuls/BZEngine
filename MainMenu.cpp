#include "MainMenu.h"

MainMenu::~MainMenu()
{
	std::clog << "[DEBUG] MainMenu::~MainMenu()" << std::endl;
}

void MainMenu::Create()
{
	formHelper_ = nanogui::FormHelper(this);
	auto window = formHelper_.addWindow(nanogui::Vector2i(0, 0), "Main Menu");
	formHelper_.addButton("Continue", []() { std::cout << "Continue" << std::endl; });
	formHelper_.addButton("New Game", []() { std::cout << "New Game" << std::endl; });
	formHelper_.addButton("Options", []() { 
		
	});
	formHelper_.addButton("Exit", [&]() { 
		parent_->Close();
	});

	setVisible(true);
	performLayout();
	window->center();
}
