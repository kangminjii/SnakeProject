#pragma once
#include <Windows.h>

class Snake
{
private:
	POINT center;
	POINT direction;
	Snake* next; // 연결부분

public:
	Snake(POINT C, POINT dir, Snake*);
	Snake();
	
	void SetNext(Snake*); // 연결된 다음 객체
	void SetPosition(int x, int y);
	void SetDirection(int x, int y);

	int GetX();
	int GetY();
	int getDirectionX();
	int getDirectionY();
	Snake* getNext(); // 멤버변수

	void Update(RECT& r);
	void Draw(HDC hdc);

	//POINT Item(HDC hdc);
	//BOOL collision(HDC hdc);
};