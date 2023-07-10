#include "Snake.h"
#include <time.h>

const int radius = 10;

Snake::Snake(POINT C, POINT dir)
{
	center = { C.x, C.y };
	direction = { dir.x, dir.y };
}

Snake::Snake()
{
	center = { 50, 100 };
	direction = { 0, 0 };
}


void Snake::SetPosition(int x, int y)
{
	center.x = x;
	center.y = y;
}

void Snake::SetDirection(int x, int y)
{
	direction.x = x;
	direction.y = y;
}

int Snake::GetX() const
{
	return center.x;
}

int Snake::GetY() const
{
	return center.y;
}

int Snake::getDirectionX()
{
	return direction.x;
}

int Snake::getDirectionY()
{
	return direction.y;
}

void Snake::Update(RECT& r)
{
	/// 죽음 처리하기
	if (center.x + radius >= r.right || center.x - radius <= r.left)
	{
		direction.x = 0;
		direction.y = 0;
	}
	if (center.y + radius >= r.bottom || center.y - radius <= r.top)
	{
		direction.x = 0;
		direction.y = 0;
	}

	center.x += direction.x;
	center.y += direction.y;
}

void Snake::Draw(HDC hdc)
{
	Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
}