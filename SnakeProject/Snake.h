#pragma once
#include <Windows.h>

class Snake
{
private:
	POINT center;
	int radius;
	POINT direction;
public:
	Snake(POINT C);
	Snake();
	void SetPosition(int x, int y);
	void SetDirection(int x, int y);
	void Update(RECT& r);
	void Draw(HDC hdc);

	//BOOL Collision(HDC hdc);
};