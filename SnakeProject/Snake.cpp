#include "Snake.h"

Snake::Snake(POINT C)
{
	center = { C.x, C.y };
	radius = 10;
	direction = { 1, 0 };
}

Snake::Snake()
{
	center = { 50, 100 };
	radius = 10;
	direction = { 1, 0 };
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
void Snake::Update(RECT& r)
{
	if (center.x + radius >= r.right || center.x - radius <= r.left);
		//death = TRUE;
	if (center.y + radius >= r.bottom || center.y - radius <= r.top);
		//death = TRUE;

	center.x += direction.x;
	center.y += direction.y;
}

void Snake::Draw(HDC hdc)
{
	Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
}
