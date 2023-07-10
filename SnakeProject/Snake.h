#pragma once
#include <Windows.h>

class Snake
{
private:
	POINT center;
	POINT direction;

public:
	Snake(POINT C, POINT dir);
	Snake();

	void SetPosition(int x, int y);
	void SetDirection(int x, int y);

	int GetX() const;
	int GetY() const;
	int getDirectionX();
	int getDirectionY();

	void Update(RECT& r);
	void Draw(HDC hdc);
};