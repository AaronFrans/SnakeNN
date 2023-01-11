#ifndef SNAKE_CLASS
#define SNAKE_CLASS
#include "framework.h"
#include <cstdio>
#include <ctime>


#include <deque>
#include <vector>
#include "Cell.h"
constexpr auto ID_TIMER = 1;


class Snake
{
public:
	Snake(int rows, int cols);
	~Snake();
	void Update(HWND hWnd);
	void DrawBitmap(HDC hdc, RECT* rect, BITMAPINFO info);
	void HandleKey(WPARAM wParam);
	int GetScore();
private:
	enum Direction { UP = 0, DOWN, LEFT, RIGHT };
	enum CellType { EMPTY = 0, SNAKE, FOOD };

	std::vector<BYTE> pixels;
	std::deque<Cell> snake;

	int rows;
	int cols;
	int dir;

	void MakeFood();
	void SetCell(int row, int col, CellType type);
	CellType GetCellType(int row, int col);
	Cell Move(const Cell& cell);
};
#endif