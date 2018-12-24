#pragma once
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Core;

ref class MyPage sealed: public Page
{
private:
	TextBlock^ textBlock;
public:
	MyPage();

	void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args);
	void OnButtonClick(Object^ sender, RoutedEventArgs^ args);
};

