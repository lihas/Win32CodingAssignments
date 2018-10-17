#include "DFT.h"
#include <Windows.h>
#include <map>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>
#include <gdiplus.h>
#include <iomanip>

//custom messages
#define UM_STEM (WM_USER+1)

//function prototypes
LRESULT CALLBACK figWndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI MessageLoop(LPVOID lpParameter);

struct classFigure
{
    WNDCLASSEX m_wndclass;
    HWND m_windowHandle;
    int m_figNum;
    PLOT_TYPE m_plotType;
    int m_tick_step_size;
    bool m_dataValid;

    struct data
    {
        std::vector<long double> X;
        std::vector<long double> Y;
        unsigned int tick_step_size;

    }m_data;

    classFigure(HWND hParent, int figNum) : m_windowHandle(NULL), m_figNum(figNum)
    {
        m_plotType = PLOT_TYPE::PLOT_TYPE_STEM;//default
        m_dataValid = false;

        static TCHAR m_szAppName[] = TEXT("DFT-Server-classFigure");
        static bool windowClassRegistered = false;

        if (!windowClassRegistered)
        {
            m_wndclass = {};
            m_wndclass.cbClsExtra = 0;
            m_wndclass.cbWndExtra = 0;
            m_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
            m_wndclass.lpfnWndProc = figWndProc;
            m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
            m_wndclass.hInstance = GetModuleHandle(NULL);
            m_wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
            m_wndclass.lpszMenuName = NULL;
            m_wndclass.lpszClassName = m_szAppName;
            m_wndclass.cbSize = sizeof(WNDCLASSEX);

            if (!RegisterClassEx(&m_wndclass))
            {
                MessageBox(NULL, TEXT("RegisterClass() error"), m_szAppName, MB_ICONERROR);
            }
            else
            {
                windowClassRegistered = true;
                //start message loop
                CreateThread(nullptr, 0, MessageLoop, nullptr, 0, nullptr);
            }
        }

        m_windowHandle = CreateWindow(m_szAppName, (std::string("Figure ") + std::to_string(figNum)).c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, NULL, GetModuleHandle(NULL), this);
        ShowWindow(m_windowHandle, SW_SHOW);
        UpdateWindow(m_windowHandle);

    }
};

DWORD WINAPI MessageLoop(LPVOID lpParameter)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT DrawPlotStemOnWindow(HWND hwnd, classFigure *classPtr)
{
    auto hDC = GetDC(hwnd);

    auto X = classPtr->m_data.X;
    auto Y = classPtr->m_data.Y;
    const PLOT_TYPE plotType = classPtr->m_plotType;
    const int tick_step = classPtr->m_tick_step_size;
    int num_samples = X.size();

    auto normalize = [](std::vector<long double>& v, int rangeMin, int rangeMax) -> void
    {
        //first scale to 0 to rangeMax-rangeMin range, then translate to rangeMin to rangeMax range
        long double maxV = *std::max_element(std::begin(v), std::end(v));
        long double minV = *std::min_element(std::begin(v), std::end(v));

        for (auto& ele : v)
        {
            ele -= minV;
        }
        //now the vector is in range 0 to maxV-minv. Now scale to [0, rangeMax-rangeMin] range
        long double mFactor = (1 / (maxV - minV))*(rangeMax - rangeMin);
        for (auto& ele : v)
        {
            ele *= mFactor;
        }
        //now translate to [rangeMi,rangeMax]
        for (auto& ele : v)
        {
            ele += rangeMin;
        }

        //https://rosettacode.org/wiki/Map_range
        //mentions a compact formula for this - 
    };
    auto XOLD = X;
    normalize(X, 0, 1000);
    normalize(Y, 0, 500);

    Gdiplus::Point *points = new Gdiplus::Point[num_samples];

    std::vector<long double> XOldticks, XNewTicks;

    for (int i = 0; i < num_samples; i++)
    {

        if (plotType == PLOT_TYPE::PLOT_TYPE_STEM)
        {
            MoveToEx(hDC, X[i], 0, NULL);
            LineTo(hDC, X[i], Y[i]);
        }
        else if(plotType == PLOT_TYPE::PLOT_TYPE_PLOT)
        {
            points[i] = Gdiplus::Point(X[i], Y[i]);
        }
        else
        {
            MessageBoxA(NULL, "DrawPlotStemOnWindow() - incorrect plot type", "Error", MB_ICONERROR);
            return E_INVALIDARG;
        }

        if (i%tick_step == 0)
        {
            XOldticks.push_back(XOLD[i]);
            XNewTicks.push_back(X[i]);
            //TextOut(hDC, X[i], 10, "+", 1);
            //TextOut(hDC, 10, Y[i], "-", 1);
        }
    }
    if (plotType == PLOT_TYPE::PLOT_TYPE_PLOT)
    {
        Gdiplus::Pen pen(Gdiplus::Color(0, 0, 255));
        Gdiplus::Graphics graphics(hDC);
        graphics.DrawCurve(&pen, points, num_samples);
    }

    int i = tick_step;
    bool odd = true;
    for(int j = 0; j < XOldticks.size(); j++)
    {
        auto loc = XNewTicks[j];
        std::ostringstream ss;
        ss << std::fixed;
        ss << std::setprecision(0);
        ss << XOldticks[j];
        int dl = 5;
        if (odd)
        {
            dl = 5;
            odd = false;
        }
        else
        {
            dl = 20;
            odd = true;
        }
        TextOut(hDC, loc, dl, ss.str().c_str(), ss.str().length());
        TextOut(hDC, 10, i, "-", -1);
        i += tick_step;
    }
    ReleaseDC(hwnd, hDC);

}

LRESULT CALLBACK figWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static std::map<HWND, classFigure*> wndMap;
    static ULONG_PTR gdiPlusToken;
    static Gdiplus::GdiplusStartupInput gdiPlusStartupInput;
    static UINT windowReferenceCount = 0;
    switch (msg)
    {
    case WM_CREATE:
    {
        windowReferenceCount++;
        if (windowReferenceCount == 1)
        {
            Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);
        }

        auto cs = (LPCREATESTRUCT)lParam;
        classFigure* classPtr = (classFigure *)cs->lpCreateParams;
        wndMap.insert(std::make_pair(hwnd, classPtr));
    }
    break; 
    case WM_PAINT:
    {
        auto it = wndMap.find(hwnd);
        if (!it->second->m_dataValid)
        {
            break;
        }
    }
    case UM_STEM:
    {
        OutputDebugString("figWndProc::UM_STEM");
        auto it = wndMap.find(hwnd);
        if (it != wndMap.end())
        {
            DrawPlotStemOnWindow(hwnd, it->second);
        }
        else
        {
            MessageBoxA(NULL, "classPtr is null", "Error - figWndProc", MB_OK);
            break;
        }
    }
    break;
    case WM_DESTROY:
    {
        windowReferenceCount--;
        auto it = wndMap.find(hwnd);
        if (it == wndMap.end())
        {
            MessageBoxA(NULL, "WM_DESTROY - hwnd not fount in wndMap", "Error - figWndProc", MB_OK);
        }
        else
        {
            wndMap.erase(it);
        }

        if (windowReferenceCount == 0)
        {
            Gdiplus::GdiplusShutdown(gdiPlusToken);
        }
        PostQuitMessage(0);
        return 0;
    }
    break;
    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

std::map<int, std::unique_ptr<classFigure>> figMap;
int lastFigure = 0;
HRESULT Figure(HWND hParent, int figNum)
{
    if(figMap.find(figNum) == figMap.end())
    {
        std::unique_ptr<classFigure> fig(new classFigure(hParent, figNum));
        figMap.insert(std::make_pair(figNum, std::move(fig)));
    }

    lastFigure = figNum;
    return S_OK;
}

HRESULT Stem(const std::vector<long double> X, const std::vector<long double> Y, const unsigned int tick_step_size, const PLOT_TYPE plotType)
{
    if (figMap.empty())
    {
        MessageBoxA(NULL, "call Figure() before stem", "Error", MB_OK);
        return E_ILLEGAL_STATE_CHANGE;
    }

    auto it = figMap.find(lastFigure);

    if ( it == figMap.end())
    {
        MessageBoxA(NULL, "Unexpected figure number", "Internal Error", MB_OK);
        return E_ILLEGAL_STATE_CHANGE;
    }

    if (!(X.size() && (X.size() == Y.size())))
    {
        MessageBoxA(NULL, "constraint : X.size() && (X.size() == Y.size()", "Error", MB_OK);
        return E_INVALIDARG;
    }

    it->second->m_data.X = X;
    it->second->m_data.Y = Y;
    it->second->m_data.tick_step_size = tick_step_size;
    it->second->m_plotType = plotType;
    it->second->m_tick_step_size = tick_step_size;
    it->second->m_dataValid = true;
    PostMessage(it->second->m_windowHandle, UM_STEM, NULL, NULL);
}

HRESULT Plot(const std::vector<long double> X, const std::vector<long double> Y, const unsigned int tick_step_size)
{
    return Stem(X, Y, tick_step_size, PLOT_TYPE::PLOT_TYPE_PLOT);
}