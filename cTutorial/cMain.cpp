#include <stdio.h>
#include <Windows.h>
#pragma region DoubleBuffer
//버퍼 초기화
#define BufferWidth 400	// 가로 버퍼 크기
#define BufferHeight 100 // 세로 버퍼 크기


HANDLE hBuffer[2];	//창 두게를 제어하는 핸들
int screenIndex;	//hBuffer[screenIndex], screenIndex == 0 or 1


void InitBuffer();
void FlipBuffer();
void ClearBuffer();
void WriteBuffer(int x, int y, const char* shape, int color);
void CloseBuffer();

#pragma endregion


		  #pragma region Define
#define G 9.80665
#define Vo 20
#define FloorCount 5

#pragma endregion


#pragma region Enum
enum Color
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE,
};

enum Dir
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

#pragma endregion
#pragma region Struct

struct Collision
{
	float time = 0.0;
	float y = 0;
	int ground = 0;
	float h = 0;
	bool jump = false;
};

struct Obj
{
	Dir dir;
	int x;
	int y;
	Color color;
	Collision collision;

	const char* shape[4][13];
};

#pragma endregion

struct Floor
{
	int x;
	int y;
	Color color;
	const char* shape[4];

};

Obj* player = nullptr;

Floor* floors[FloorCount] = {};

void PlayerInit();
void BgInit();
void Init();
void Update();



int main()
{
	InitBuffer();
	Init();

	while (true)
	{
		Update();

		FlipBuffer();
		ClearBuffer();

		Sleep(20);

	}

	CloseBuffer();
	return 0;
}

void PlayerInit()
{
	player = (Obj*)malloc(sizeof(Obj));
	player->x = 10;
	player->y = 80;
	player->color = WHITE;
	player->dir = RIGHT;
	player->collision.h = 0;
	player->collision.jump = false;
	player->collision.time = 0.0f;
	player->collision.y = 0;
	player->collision.ground = 0;

	player->shape[0][0] = "　　　■■　　　";
	player->shape[0][1] = "　■■　　■■　";
	player->shape[0][2] = "■　　　　　　■";
	player->shape[0][3] = "■　　　　　　■";
	player->shape[0][4] = "■■　　　　　■";
	player->shape[0][5] = "■　■■■　　■";
	player->shape[0][6] = "■　　　　■　■";
	player->shape[0][7] = "　■　■　　■　";
	player->shape[0][8] = "■　　　　■■　";
	player->shape[0][9] = "　■■■■■　　";
	player->shape[0][10] = "■　　　　■　　";
	player->shape[0][11] = "■　■■　■　　";
	player->shape[0][12] = "　■　　■　　　";

	player->shape[1][0] = "　　　■■　　　";
	player->shape[1][1] = "　■■　　■■　";
	player->shape[1][2] = "■　　　　　　■";
	player->shape[1][3] = "■　　　　　　■";
	player->shape[1][4] = "■　　　　　■■";
	player->shape[1][5] = "■　　■■■　■";
	player->shape[1][6] = "■　■　　　　■";
	player->shape[1][7] = "　■　　■　■　";
	player->shape[1][8] = "　　■　　　　■";
	player->shape[1][9] = "　　■■■■■　";
	player->shape[1][10] = "　　■　　　　■";
	player->shape[1][11] = "　　■　■■　■";
	player->shape[1][12] = "　　　■　　■　";

	player->shape[2][0] = "　　　　■■　　";
	player->shape[2][1] = "　■■■　　■　";
	player->shape[2][2] = "■　　　　　　■";
	player->shape[2][3] = "■　　　　　　■";
	player->shape[2][4] = "■　　　　　　■";
	player->shape[2][5] = "■　　　　　　■";
	player->shape[2][6] = "■　　　　　　■";
	player->shape[2][7] = "■　　　　　　■";
	player->shape[2][8] = "　■　　　　■　";
	player->shape[2][9] = "■　■■■■　■";
	player->shape[2][10] = "■　　　　　　■";
	player->shape[2][11] = "　■　■■　■　";
	player->shape[2][12] = "　■■　　■■　";

	player->shape[3][0] = "　　■■　　　　";
	player->shape[3][1] = "　■　　■■■　";
	player->shape[3][2] = "■　　　　　　■";
	player->shape[3][3] = "■　　　　　　■";
	player->shape[3][4] = "■　　　　■　■";
	player->shape[3][5] = "■　■■■　■■";
	player->shape[3][6] = "■■　　　　■■";
	player->shape[3][7] = "■　■　　■　■";
	player->shape[3][8] = "　■　　　　■　";
	player->shape[3][9] = "■■■■■■■■";
	player->shape[3][10] = "■　　　　　　■";
	player->shape[3][11] = "　■　■■　■　";
	player->shape[3][12] = "　■■　　■■　";
}

void BgInit()
{
	for (int i = 0; i < FloorCount; i++)
	{
		floors[i] = (Floor*)malloc(sizeof(Floor));
		floors[i]->x = 30 * i + 10;
		floors[i]->y = 93;
		floors[i]->color = WHITE;
		floors[i]->shape[0] = "■■■■■■■■■■■■■■■■■■■■■■";
		floors[i]->shape[1] = "■■■■■■■■■■■■■■■■■■■■■■";
		floors[i]->shape[2] = "■■■■■■■■■■■■■■■■■■■■■■";
		floors[i]->shape[3] = "■■■■■■■■■■■■■■■■■■■■■■";
	}
	
}

void Init()
{
	PlayerInit();
	BgInit();
}

void Update()
{



	//h = vo * time - 0.5 * g * t * t
	
	if (GetAsyncKeyState(VK_SPACE) && !player->collision.jump)
	{
		player->collision.jump = true;
		player->collision.time = 0;
		player->collision.y = player->y;
		player->collision.ground = player->y;
		player->collision.h = 0;
	}


	if (player->collision.jump)
	{
		player->collision.time += 0.1;
		//h = vo * time - 0.5 * g * t * t

		player->collision.h = -(Vo * player->collision.time) + (0.5 * G * player->collision.time * player->collision.time);

		player->y = (int)(player->collision.y + player->collision.h);

		for (int i = 0; i < FloorCount; i++)
		{
			if (floors[i]->x < player->x + 8 &&
				player->x < floors[i]->x + 20 && 
				floors[i]->y < player->y + 13 && 
				player->y < floors[i]->y + 4)
			{

				player->y = floors[i]->y - 13;
				player->collision.jump = false;
			}

		}


	}


	if (GetAsyncKeyState(VK_LEFT))
	{
		player->x--;
		player->dir = LEFT;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		player->x++;
		player->dir = RIGHT;
	}

	for (int i = 0; i < 13; i++)
	{
		WriteBuffer(player->x, player->y + i, player->shape[player->dir][i], player->color);
	}


	for (int i = 0; i < FloorCount; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			WriteBuffer(floors[i]->x, floors[i]->y + j, floors[i]->shape[j], floors[i]->color);
		}

	}


	

}

#pragma region DoubleBuffer
//버퍼 초기화
void InitBuffer()
{
	//현재 스크린 index값은 0
	screenIndex = 0;

	//size.x = 80, size.y = 40 : 화면 크기
	COORD size = { BufferWidth, BufferHeight };

	//창 크기 왼쪽 : 0, 위쪽 : 0 오른쪽 : 80 - 1, 아래쪽 : 40 - 1
	SMALL_RECT rect = { 0,0,BufferWidth - 1, BufferHeight - 1 };

	//0번 버퍼 만들기
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//0번 화면 버퍼 사이즈 설정(만든 버퍼 주소, 크기)
	SetConsoleScreenBufferSize(hBuffer[0], size);
	//0번 창 사이즈 설정
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	//1번 버퍼 만들기
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//1번 화면 버퍼 사이즈 설정(만든 버퍼 주소, 크기)
	SetConsoleScreenBufferSize(hBuffer[1], size);
	//1번 창 사이즈 설정
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	CONSOLE_CURSOR_INFO info;	//콘솔 커서 정보 구조체
	info.dwSize = 1;			//커서 크기
	info.bVisible = FALSE;		//커서 안보이게

	//0 & 1 각각의 버퍼에 커서 정보 등록
	SetConsoleCursorInfo(hBuffer[0], &info);
	SetConsoleCursorInfo(hBuffer[1], &info);
}

void FlipBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[screenIndex]);
	screenIndex = !screenIndex;
}


void ClearBuffer()
{
	COORD pos = { 0,0 };
	DWORD dw;

	FillConsoleOutputCharacter(hBuffer[screenIndex], ' ', BufferWidth * BufferHeight, pos, &dw);
}


void WriteBuffer(int x, int y, const char* shape, int color)
{
	COORD pos = { x * 2, y };

	SetConsoleCursorPosition(hBuffer[screenIndex], pos);
	SetConsoleTextAttribute(hBuffer[screenIndex], color);

	DWORD dw;

	WriteFile(hBuffer[screenIndex], shape, strlen(shape), &dw, NULL);
}

//버퍼 해제
void CloseBuffer()
{
	//버퍼 닫기
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}

#pragma endregion
