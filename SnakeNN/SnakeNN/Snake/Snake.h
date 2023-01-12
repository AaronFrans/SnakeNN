#ifndef SNAKE_CLASS
#define SNAKE_CLASS
#include "framework.h"
#include <cstdio>
#include <ctime>


#include <deque>
#include <vector>
#include "NN\DataStruct.h"
#include "Cell.h"
constexpr auto ID_TIMER = 1;


class Snake
{
public:
	Snake(int rows, int cols);


	void Update(HWND hWnd);
	void Update();
	void DrawBitmap(HDC hdc, RECT* rect, BITMAPINFO info);
	void HandleKey(WPARAM wParam);
	int GetScore();


	Inputs GetState();
	Outputs GetBestDir();
	bool IsDead() const { return m_IsDead; };

	void UpdateDir(Outputs nnResult);
private:
	enum Direction { UP = 0, DOWN, LEFT, RIGHT };
	enum CellType { EMPTY = 0, SNAKE, FOOD };

	std::vector<BYTE> m_Pixels;
	std::deque<Cell> m_Snake;

	int m_Rows;
	int m_Cols;
	int m_FoodRow;
	int m_FoodCol;
	int m_Dir;

	bool m_IsDead{ false };


	void MakeFood();
	void SetCell(int row, int col, CellType type);
	CellType GetCellType(int row, int col);
	Cell Move(const Cell& cell, HWND hWnd);
	Cell Move(const Cell& cell);
	void GameOver(HWND hWnd);
};
#endif