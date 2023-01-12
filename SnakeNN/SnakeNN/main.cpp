#include <Windows.h>
#include <string>
#include <cassert>
#include "Snake\Snake.h"
#include "NN\Utils.h"
#include "NN\Network.h"
#include "NN\DataStruct.h"
#include "NN\TrainingData.h"

//#define TRAINXOR
#define TRAINSNAKE

constexpr int WND_WIDTH = 500;
constexpr int WND_HEIGHT = 500;

constexpr int NUM_DRAWN_SNAKES = 1;

std::vector<Network*> g_NNs;


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BITMAPINFO info;
	static Snake* snake;

	switch (msg)
	{
	case WM_CREATE:
	{

		int rows = 40;
		int cols = 40;

		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = cols;
		info.bmiHeader.biHeight = rows;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = 0;
		info.bmiHeader.biXPelsPerMeter = 0;
		info.bmiHeader.biYPelsPerMeter = 0;
		info.bmiHeader.biClrUsed = 0;
		info.bmiHeader.biClrImportant = 0;

#ifdef TRAINSNAKE
		snake = g_NNs[0]->GetSnake();
#else
		snake = new Snake(rows, cols);
#endif // TRAINSNAKE

		if (!SetTimer(hWnd, ID_TIMER, 80, NULL))
		{
			MessageBox(hWnd, L"Could not set timer!", L"Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(1);
		}

		break;
	}
	case WM_PAINT:
	{
		RECT rcClient;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rcClient);
		snake->DrawBitmap(hdc, &rcClient, info);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_TIMER:
	{
		RECT rcClient;
		HDC hdc = GetDC(hWnd);

		GetClientRect(hWnd, &rcClient);
		//update the nn to work with 
		//snake->Update(hWnd);

		//train all nn
		for (auto& nn : g_NNs)
		{
			if (!nn->GetSnake()->IsDead())
			nn->UpdateSnake();
		}

		//check if all dead -> reset all
		if (snake->IsDead())
		{
			int bestScoreIdx{ 0 };
			int bestScore{ INT_MIN };
			for (unsigned i = 0; i < g_NNs.size(); i++)
			{
				int score = g_NNs[i]->GetSnake()->GetScore();
				if (score > bestScore)
				{
					bestScore = score;
					bestScoreIdx = i;
				}
			}

			std::vector<Layer> bestLayers = g_NNs[bestScoreIdx]->GetLayerInfo();

			for (unsigned i = NUM_DRAWN_SNAKES; i < g_NNs.size(); i++)
			{
				g_NNs[i]->SetLayerInfo(bestLayers);
				g_NNs[i]->ResetSnake();
			}

			snake = g_NNs[0]->ResetSnake();
		}

		//while main nn not dead draw it
		if (!snake->IsDead())
		snake->DrawBitmap(hdc, &rcClient, info);

		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_KEYDOWN:
		snake->HandleKey(wParam);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
#ifndef TRAINSNAKE
		delete snake;
#endif // !TRAINSNAKE


		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

#ifdef TRAINXOR
	TrainingData data(L"TrainingData/trainingData.txt");
	std::vector<unsigned> topology;

	data.GetTopology(topology);
	Network nn(topology, 1);


	std::vector<float> inputs, targets, results;

	int currentTrainingPass{};

	while (!data.IsEof())
	{
		++currentTrainingPass;
		std::wstring output = L"Pass: " + std::to_wstring(currentTrainingPass) + L"\n";
		OutputDebugString(output.c_str());

		if (data.GetNextInputs(inputs) != topology[0])
			break;

		Utils::ShowVectorVals(L"Inputs: \n", inputs);
		nn.FeedForward(inputs);

		nn.GetResults(results);
		Utils::ShowVectorVals(L"Outputs: \n", results);

		data.GetTargetOutputs(targets);
		Utils::ShowVectorVals(L"Targets: \n", targets);

		assert(targets.size() == topology.back());

		nn.BackProp(targets);
		output = L"Net recent average error: " + std::to_wstring(nn.GetRecentAverageError()) + L"\n";
		OutputDebugString(output.c_str());
	}

	OutputDebugString(L"Done\n");



#endif
#ifdef TRAINSNAKE
	g_NNs.push_back(new Network(std::vector<unsigned>{Inputs::NrOfInputs, 4, 4, Outputs::NrOfOutputs}));

	for (int i = 0; i < 30; i++)
	{
		g_NNs.push_back(new Network(std::vector<unsigned>{Inputs::NrOfInputs, 4, 4, Outputs::NrOfOutputs}, false));
	}
#endif // TRAINSNAKE


	const TCHAR szClassName[] = TEXT("MyClass");

	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	hWnd = CreateWindow(szClassName,
		TEXT("Snake"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* this window style prevents window resizing */
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WND_WIDTH,
		WND_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}