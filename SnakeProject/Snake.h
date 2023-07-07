#pragma once
#include <Windows.h>

class Snake
{
private:
	POINT center;
	POINT direction;
	Snake* next; // ����κ�

public:
	Snake(POINT C, POINT dir, Snake*);
	Snake();
	
	void SetNext(Snake*); // ����� ���� ��ü
	void SetPosition(int x, int y);
	void SetDirection(int x, int y);

	int GetX();
	int GetY();
	int getDirectionX();
	int getDirectionY();
	Snake* getNext(); // �������

	void Update(RECT& r);
	void Draw(HDC hdc);

	//POINT Item(HDC hdc);
	//BOOL collision(HDC hdc);
};