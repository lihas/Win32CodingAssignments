//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
TextBlock^ textBlock;
MainPage::MainPage()
{
	InitializeComponent();
    Grid^ grid = ref new Grid();
    grid->Background = ref new SolidColorBrush(Colors::White);

    textBlock = ref new TextBlock();
    textBlock->Text = "Error Text";
    textBlock->FontFamily = ref new Media::FontFamily("Segoe UI");
    textBlock->FontSize = 120;
    textBlock->FontStyle = Text::FontStyle::Oblique;
    textBlock->FontWeight = Text::FontWeight(Text::FontWeights::Bold);
    textBlock->Foreground = ref new SolidColorBrush(Colors::Gold);
    textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
    textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

    grid->Children->Append(textBlock);

    Button^ btnCreateFile = ref new Button;
    btnCreateFile->Content = "Create File";
    btnCreateFile->Click += ref new RoutedEventHandler(this, &MainPage::OnButtonDownCreateFile);

    grid->Children->Append(btnCreateFile);

    this->Content = grid;

}


void MainPage::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    
}


void App1::MainPage::OnButtonDownCreateFile(Object^ sender, RoutedEventArgs^ args)
{
    textBlock->Text = "OnButtonDownCreateFile";

    auto asyncOp = Windows::Storage::DownloadsFolder::CreateFileAsync("WinrtAppFile.txt", Windows::Storage::CreationCollisionOption::FailIfExists);
    asyncOp->Completed = ref new AsyncOperationCompletedHandler<Windows::Storage::StorageFile^>([this](IAsyncOperation<StorageFile^>^ asyncOperation, AsyncStatus asyncstatus) {
        if (asyncstatus != AsyncStatus::Error)
        {
            auto file = asyncOperation->GetResults();
            FileIO::WriteTextAsync(file, "Hello from UWP");
        }
        else
        {
            //Window::Current->Dispatcher->
            auto d = ref new Windows::UI::Core::DispatchedHandler([this]() {
                textBlock->Text = "Error creating file. delete if already exists";
            });
            volatile auto cv1 = Window::Current;//->Dispatcher;// ->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, d);
            auto cv = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView();
            cv->CoreWindow->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, d);
        }
    });
}
