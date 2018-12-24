#pragma once

ref class App sealed : public Windows::UI::Xaml::Application
{
public:
	void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;
};
