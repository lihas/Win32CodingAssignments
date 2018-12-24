/*Read README.txt*/
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::UI;
using namespace Platform;
using namespace Windows::UI::Xaml;
using Windows::UI::Xaml::ApplicationInitializationCallback;
using Windows::UI::Color;


ref class MyPage sealed : public Page
{
private:
	TextBlock^ textBlock;
public:
	MyPage();

	void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args);
	void OnButtonClick(Object^ sender, RoutedEventArgs^ args);
};

MyPage::MyPage()
{
	Window::Current->CoreWindow->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MyPage::OnKeyDown);
	Grid^ grid = ref new Grid;

	textBlock = ref new TextBlock();
	textBlock->Text = "Hello World";
	textBlock->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	textBlock->FontSize = 120;
	textBlock->FontWeight = Text::FontWeight(Text::FontWeights::Bold);

	textBlock->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Gold);
	textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

	grid->Children->Append(textBlock);

	Button^ button = ref new Button;
	button->Content = "Press Me";
	button->Width = 400;
	button->Height = 200;
	button->BorderThickness = 12;
	button->BorderBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Gold);
	button->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Red);
	button->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	button->FontWeight = Windows::UI::Text::FontWeight(Windows::UI::Text::FontWeights::Bold);
	button->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	button->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;
	button->Click += ref new RoutedEventHandler(this, &MyPage::OnButtonClick);

	grid->Children->Append(button);
	this->Content = grid;
}

void MyPage::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
{
	textBlock->Text = "KeyPressed";
}

void MyPage::OnButtonClick(Object^ sender, RoutedEventArgs^ args)
{
	textBlock->Text = "Button Clicked";
}

ref class App sealed : public Windows::UI::Xaml::Application
{
public:
	void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;
};

void MyCallbackMethod(Windows::UI::Xaml::ApplicationInitializationCallbackParams^ params)
{
	App^ app = ref new App;
}

int main(Array<String^>^ args)
{
	Windows::UI::Xaml::ApplicationInitializationCallback^ callback = ref new Windows::UI::Xaml::ApplicationInitializationCallback(MyCallbackMethod);
	Application::Start(callback);
	return 0;
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
	MyPage^ page = ref new MyPage();
	Window::Current->Content = page;
	Window::Current->Activate();
}
