#include "pch.h"
#include "Hello.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI;
using namespace Platform;

void MyCallbackMethod(Windows::UI::Xaml::ApplicationInitializationCallbackParams^ params)
{
	App^ app = ref new App();
}

int main(Array<String^>^ args)
{
	ApplicationInitializationCallback^ callback = ref new ApplicationInitializationCallback(MyCallbackMethod);
	Application::Start(callback);
	return 0;
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
	TextBlock^ textBlock = ref new TextBlock();
	textBlock->Text = "Hello World";
	textBlock->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	textBlock->FontSize = 120;
	textBlock->FontWeight = Text::FontWeight(Text::FontWeights::Bold);
	
	textBlock->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Gold);
	textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

	Window::Current->Content = textBlock;
	Window::Current->Activate();
}