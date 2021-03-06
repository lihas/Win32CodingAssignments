﻿//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml;

namespace App1
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

    private:
        void button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    public:
        void OnButtonDownCreateFile(Object^ sender, RoutedEventArgs^ args);
    };
}
