#include "pch.h"
#include "MyPage.h"

using namespace Windows::Foundation;
using namespace Windows::UI;
using Windows::UI::Color;

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
