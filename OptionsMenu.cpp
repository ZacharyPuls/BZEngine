#include "OptionsMenu.h"

OptionsMenu::~OptionsMenu() = default;

void OptionsMenu::Create()
{
	int count;
	const auto resolutionSettings = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	auto allPossibleResolutions = std::vector<GLFWvidmode>(resolutionSettings, resolutionSettings + count);
	const auto lastValue = std::unique(allPossibleResolutions.begin(), allPossibleResolutions.end(), [](GLFWvidmode a, GLFWvidmode b)
	{
		return a.width == b.width && a.height == b.height && a.refreshRate == b.refreshRate;
	});
	allPossibleResolutions.erase(lastValue, allPossibleResolutions.end());
	for (auto allPossibleResolution : allPossibleResolutions)
	{
		std::cout << allPossibleResolution.width << "x" << allPossibleResolution.height << "@" << allPossibleResolution.refreshRate << std::endl;
	}
	// { "1920x1080", "1280x960", "640x480" }
	formHelper_ = nanogui::FormHelper(this);
	auto window = formHelper_.addWindow(nanogui::Vector2i(0, 0), "Options Menu");
	formHelper_.addGroup("Graphics");
	std::string resolution;
	Resolution res;
	Quality quality;
	formHelper_.addVariable("Resolution", res, false)->setItems({ "1920x1080", "1280x960", "640x480" });
	formHelper_.addVariable("Quality", quality, false)->setItems({ "Low", "Medium", "High", "Ultra" });
	int idx = resolution.find("x");
	settings_.ResolutionX = std::atoi(resolution.substr(0, idx).c_str());
	settings_.ResolutionY = std::atoi(resolution.substr(idx + 1, resolution.length()).c_str());
	formHelper_.addVariable("Quality", quality, true)->setItems({ "1920x1080", "1280x960", "640x480" });
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
