#include "pch.h"
#include "Hello.h"
#include "MyPage.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using Windows::UI::Xaml::ApplicationInitializationCallback;

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