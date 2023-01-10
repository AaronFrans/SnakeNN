#include "Snake.h"

Snake::Snake(int rows, int cols)
    : rows(rows), cols(cols)
{
    pixels.resize(rows * cols * 4);
    srand(static_cast<unsigned int>(time(NULL)));
    Snake::MakeFood();
    snake.push_back(Cell{ rows / 2, cols / 2 });
    dir = UP;
}

Snake::~Snake()
{}

void Snake::Update(HWND hWnd)
{
    snake.push_back(Snake::Move(snake.back()));

    Cell head = snake.back();
    bool eaten = false;

    if (Snake::GetCellType(head.row, head.col) == FOOD)
    {
        snake.push_back(Snake::Move(head));
        Cell newHead = snake.back();
        Snake::SetCell(newHead.row, newHead.col, SNAKE);
        eaten = true;
    }
    else if (Snake::GetCellType(head.row, head.col) == SNAKE)
    {
        wchar_t buff[200];
        _snwprintf_s(buff, sizeof(buff), L"Score: %d", Snake::GetScore());
        KillTimer(hWnd, ID_TIMER);
        MessageBox(hWnd, buff, L"Game Over!", MB_OK | MB_ICONINFORMATION);
        DestroyWindow(hWnd);
    }

    Snake::SetCell(head.row, head.col, SNAKE);

    Cell tail = snake.front();
    snake.pop_front();
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
        cols,
        rows,
        &pixels[0],
        &info,
        DIB_RGB_COLORS,
        SRCCOPY);
}

void Snake::HandleKey(WPARAM wParam)
{
    switch (wParam)
    {
    case VK_UP:
        dir = UP;
        break;
    case VK_DOWN:
        dir = DOWN;
        break;
    case VK_LEFT:
        dir = LEFT;
        break;
    case VK_RIGHT:
        dir = RIGHT;
        break;
    default:
        break;
    }
}

int Snake::GetScore()
{
    return static_cast<int>(snake.size());
}

void Snake::MakeFood()
{
    int row = rand() % rows;
    int col = rand() % cols;

    while (Snake::GetCellType(row, col) == SNAKE)
    {
        row = rand() % rows;
        col = rand() % cols;
    }

    Snake::SetCell(row, col, FOOD);
}

void Snake::SetCell(int row, int col, CellType type)
{
    int offset = row * cols * 4 + col * 4;

    switch (type)
    {
    case EMPTY:
        pixels[offset] = 0;
        pixels[offset + 1] = 0;
        pixels[offset + 2] = 0;
        pixels[offset + 3] = 255;
        break;
    case SNAKE:
        pixels[offset] = 0;
        pixels[offset + 1] = 255;
        pixels[offset + 2] = 0;
        pixels[offset + 3] = 255;
        break;
    case FOOD:
        pixels[offset] = 0;
        pixels[offset + 1] = 0;
        pixels[offset + 2] = 255;
        pixels[offset + 3] = 255;
        break;
    default:
        break;
    }
}

Snake::CellType Snake::GetCellType(int row, int col)
{
    int offset = row * rows * 4 + col * 4;

    int red = pixels[offset + 2];
    int green = pixels[offset + 1];
    int blue = pixels[offset];

    if (red == 0 && green == 0 && blue == 0) return EMPTY;
    if (red == 0 && green == 255 && blue == 0) return SNAKE;
    if (red == 255 && green == 0 && blue == 0) return FOOD;

    return EMPTY; // should never reach here
}

Cell Snake::Move(const Cell& cell)
{
    Cell c{ cell.row, cell.col };

    switch (dir)
    {
    case UP: c.row += 1; break;
    case DOWN: c.row -= 1; break;
    case LEFT: c.col -= 1; break;
    case RIGHT: c.col += 1; break;
    }

    // teleportation
    if (c.row == -1)     c.row = rows - 1;
    if (c.row == rows)   c.row = 0;
    if (c.col == -1)     c.col = cols - 1;
    if (c.col == cols)   c.col = 0;

    return c;
}
 // !SNAKE_CLASS

