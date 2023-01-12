#include <algorithm>
#include "Snake.h"
#include "Utils.h"

Snake::Snake(int rows, int cols)
	: m_Rows(rows), m_Cols(cols)
{
	m_Pixels.resize(rows * cols * 4);
	srand(static_cast<unsigned int>(time(NULL)));
	Snake::MakeFood();
	m_Snake.push_back(Cell{ rows / 2, cols / 2 });
	m_Dir = UP;
}

void Snake::Update(HWND hWnd)
{
	m_Snake.push_back(Snake::Move(m_Snake.back(), hWnd));

	Cell head = m_Snake.back();
	bool eaten = false;

	if (Snake::GetCellType(head.row, head.col) == FOOD)
	{
		m_Snake.push_back(Snake::Move(head, hWnd));
		Cell newHead = m_Snake.back();
		Snake::SetCell(newHead.row, newHead.col, SNAKE);
		eaten = true;
	}
	else if (Snake::GetCellType(head.row, head.col) == SNAKE)
	{
		GameOver(hWnd);
	}

	Snake::SetCell(head.row, head.col, SNAKE);

	Cell tail = m_Snake.front();
	m_Snake.pop_front();
	Snake::SetCell(tail.row, tail.col, EMPTY);

	if (eaten) Snake::MakeFood();
}

void Snake::Update()
{
	m_Snake.push_back(Snake::Move(m_Snake.back()));

	if (m_IsDead)
		return;
	Cell head = m_Snake.back();
	bool eaten = false;

	if (Snake::GetCellType(head.row, head.col) == FOOD)
	{
		m_Snake.push_back(Snake::Move(head));
		if (m_IsDead)
			return;
		Cell newHead = m_Snake.back();
		Snake::SetCell(newHead.row, newHead.col, SNAKE);
		eaten = true;
	}
	else if (Snake::GetCellType(head.row, head.col) == SNAKE)
	{
		m_IsDead = true;
	}

	Snake::SetCell(head.row, head.col, SNAKE);

	Cell tail = m_Snake.front();
	m_Snake.pop_front();
	Snake::SetCell(tail.row, tail.col, EMPTY);

	if (eaten) Snake::MakeFood();
}

void Snake::DrawBitmap(HDC hdc, RECT* rect, BITMAPINFO info)
{
	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;

	StretchDIBits(hdc,
		0,
		0,
		width,
		height,
		0,
		0,
		m_Cols,
		m_Rows,
		&m_Pixels[0],
		&info,
		DIB_RGB_COLORS,
		SRCCOPY);
}

void Snake::HandleKey(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		m_Dir = UP;
		break;
	case VK_DOWN:
		m_Dir = DOWN;
		break;
	case VK_LEFT:
		m_Dir = LEFT;
		break;
	case VK_RIGHT:
		m_Dir = RIGHT;
		break;
	default:
		break;
	}
}

int Snake::GetScore()
{
	return static_cast<int>(m_Snake.size());
}

Inputs Snake::GetState()
{
	Inputs toReturn{};

	Cell& head = m_Snake.back();


	toReturn.DistToLeftWallNorm = head.col / (m_Cols - 1.f);
	toReturn.DistToRightWallNorm = (m_Cols - 1.f - head.col) / (m_Cols - 1.f);
	toReturn.DistToTopWallNorm = head.row / (m_Rows - 1.f);
	toReturn.DistToBottomWallNorm = (m_Rows - 1.f - head.row) / (m_Rows - 1.f);

	float headToFoodX{ static_cast<float>(m_FoodCol) - head.col }, headToFoodY{ static_cast<float>(m_FoodRow) - head.row };

	float headToFoodLength{ sqrtf(headToFoodX * headToFoodX + headToFoodY * headToFoodY) };

	toReturn.DistToFoodXNorm = headToFoodX / headToFoodLength;
	toReturn.DistToFoodYNorm = headToFoodY / headToFoodLength;

	toReturn.IsBodyUp = head.row + 1 < m_Rows && Snake::GetCellType(head.row + 1, head.col) == SNAKE;
	toReturn.IsBodyDown = head.row - 1 > -1 && Snake::GetCellType(head.row - 1, head.col) == SNAKE;
	toReturn.IsBodyLeft = head.col - 1 > -1 && Snake::GetCellType(head.row, head.col - 1) == SNAKE;
	toReturn.IsBodyRight = head.col + 1 < m_Cols && Snake::GetCellType(head.row, head.col + 1) == SNAKE;

	return toReturn;
}

Outputs Snake::GetBestDir()
{
	Outputs toReturn{};
	Cell& head = m_Snake.back();
	float headToFoodX{ static_cast<float>(m_FoodCol) - head.col }, headToFoodY{ static_cast<float>(m_FoodRow) - head.row };

	bool isBodyUp = head.row + 1 < m_Rows && Snake::GetCellType(head.row + 1, head.col) == SNAKE;
	bool isBodyDown = head.row - 1 > -1 && Snake::GetCellType(head.row - 1, head.col) == SNAKE;
	bool isBodyLeft = head.col - 1 > -1 && Snake::GetCellType(head.row, head.col - 1) == SNAKE;
	bool isBodyRight = head.col + 1 < m_Cols && Snake::GetCellType(head.row, head.col + 1) == SNAKE;


	if (isBodyUp)
	{
		if (headToFoodY < 0 && abs(headToFoodX) < abs(headToFoodY))
		{
			toReturn.GoDownPercent = 1.f;
		}
		else
		{
			if (headToFoodX > 0)
				toReturn.GoRightPercent = 1.f;
			else
				toReturn.GoLeftPercent = 1.f;
		}
	}
	else if (isBodyDown)
	{
		if (headToFoodY > 0 && abs(headToFoodX) < abs(headToFoodY))
		{
			toReturn.GoUpPercent = 1.f;
		}
		else
		{
			if (headToFoodX > 0)
				toReturn.GoRightPercent = 1.f;
			else
				toReturn.GoLeftPercent = 1.f;
		}
	}
	else if (isBodyLeft)
	{
		if (headToFoodX > 0 && abs(headToFoodX) > abs(headToFoodY))
		{
			toReturn.GoRightPercent = 1.f;
		}
		else
		{
			if (headToFoodY > 0)
				toReturn.GoUpPercent = 1.f;
			else
				toReturn.GoDownPercent = 1.f;
		}
	}
	else if (isBodyRight)
	{
		if (headToFoodX < 0 && abs(headToFoodX) > abs(headToFoodY))
		{
			toReturn.GoLeftPercent = 1.f;
		}
		else
		{
			if (headToFoodY > 0)
				toReturn.GoUpPercent = 1.f;
			else
				toReturn.GoDownPercent = 1.f;
		}
	}
	else
	{
		if (abs(headToFoodX) > abs(headToFoodY))
		{
			if (headToFoodX > 0)
				toReturn.GoRightPercent = 1.f;
			else
				toReturn.GoLeftPercent = 1.f;
		}
		if (abs(headToFoodX) < abs(headToFoodY))
		{
			if (headToFoodY > 0)
				toReturn.GoUpPercent = 1.f;
			else
				toReturn.GoDownPercent = 1.f;
		}
	}

	return toReturn;
}

void Snake::UpdateDir(Outputs nnResult)
{
	std::vector<float> results{};
	nnResult.ToVector(results);
	auto maxIt = std::max_element(begin(results), end(results));

	if (Utils::AreSame(*maxIt, nnResult.GoUpPercent))
	{
		m_Dir = UP;
	}
	if (Utils::AreSame(*maxIt, nnResult.GoDownPercent))
	{
		m_Dir = DOWN;
	}
	if (Utils::AreSame(*maxIt, nnResult.GoRightPercent))
	{
		m_Dir = RIGHT;
	}
	if (Utils::AreSame(*maxIt, nnResult.GoLeftPercent))
	{
		m_Dir = LEFT;
	}

}

void Snake::MakeFood()
{
	m_FoodRow = rand() % m_Rows;
	m_FoodCol = rand() % m_Cols;

	while (Snake::GetCellType(m_FoodRow, m_FoodCol) == SNAKE)
	{
		m_FoodRow = rand() % m_Rows;
		m_FoodCol = rand() % m_Cols;
	}


	Snake::SetCell(m_FoodRow, m_FoodCol, FOOD);
}

void Snake::SetCell(int row, int col, CellType type)
{
	int offset = row * m_Cols * 4 + col * 4;

	switch (type)
	{
	case EMPTY:
		m_Pixels[offset] = 0;
		m_Pixels[offset + 1] = 0;
		m_Pixels[offset + 2] = 0;
		m_Pixels[offset + 3] = 255;
		break;
	case SNAKE:
		m_Pixels[offset] = 0;
		m_Pixels[offset + 1] = 255;
		m_Pixels[offset + 2] = 0;
		m_Pixels[offset + 3] = 255;
		break;
	case FOOD:
		m_Pixels[offset] = 0;
		m_Pixels[offset + 1] = 0;
		m_Pixels[offset + 2] = 255;
		m_Pixels[offset + 3] = 255;
		break;
	default:
		break;
	}
}

Snake::CellType Snake::GetCellType(int row, int col)
{
	int offset = row * m_Rows * 4 + col * 4;

	int red = m_Pixels[offset + 2];
	int green = m_Pixels[offset + 1];
	int blue = m_Pixels[offset];

	if (red == 0 && green == 0 && blue == 0) return EMPTY;
	if (red == 0 && green == 255 && blue == 0) return SNAKE;
	if (red == 255 && green == 0 && blue == 0) return FOOD;

	return EMPTY; // should never reach here
}

Cell Snake::Move(const Cell& cell, HWND hWnd)
{
	Cell c{ cell.row, cell.col };

	switch (m_Dir)
	{
	case UP: c.row += 1; break;
	case DOWN: c.row -= 1; break;
	case LEFT: c.col -= 1; break;
	case RIGHT: c.col += 1; break;
	}

	// teleportation
	if (c.row == -1)     GameOver(hWnd);
	if (c.row == m_Rows)   GameOver(hWnd);
	if (c.col == -1)     GameOver(hWnd);
	if (c.col == m_Cols)   GameOver(hWnd);

	return c;
}

Cell Snake::Move(const Cell& cell)
{
	Cell c{ cell.row, cell.col };

	switch (m_Dir)
	{
	case DOWN: c.row -= 1; break;
	case UP: c.row += 1; break;
	case LEFT: c.col -= 1; break;
	case RIGHT: c.col += 1; break;
	}

	// teleportation
	if (c.row == -1)     m_IsDead = true; // bottom
	if (c.row == m_Rows)   m_IsDead = true; // top
	if (c.col == -1)     m_IsDead = true; // left
	if (c.col == m_Cols)   m_IsDead = true; // right

	return c;
}

void Snake::GameOver(HWND hWnd)
{
	wchar_t buff[200];
	_snwprintf_s(buff, sizeof(buff), L"Score: %d", Snake::GetScore());
	KillTimer(hWnd, ID_TIMER);
	MessageBox(hWnd, buff, L"Game Over!", MB_OK | MB_ICONINFORMATION);
	DestroyWindow(hWnd);
}
// !SNAKE_CLASS

