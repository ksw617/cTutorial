#include <stdio.h>
#include <Windows.h>
#pragma region DoubleBuffer
//���� �ʱ�ȭ
#define BufferWidth 400	// ���� ���� ũ��
#define BufferHeight 100 // ���� ���� ũ��


HANDLE hBuffer[2];	//â �ΰԸ� �����ϴ� �ڵ�
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

	player->shape[0][0] = "��������ᡡ����";
	player->shape[0][1] = "����ᡡ����ᡡ";
	player->shape[0][2] = "�ᡡ������������";
	player->shape[0][3] = "�ᡡ������������";
	player->shape[0][4] = "��ᡡ����������";
	player->shape[0][5] = "�ᡡ���ᡡ����";
	player->shape[0][6] = "�ᡡ�������ᡡ��";
	player->shape[0][7] = "���ᡡ�ᡡ���ᡡ";
	player->shape[0][8] = "�ᡡ��������ᡡ";
	player->shape[0][9] = "�������ᡡ��";
	player->shape[0][10] = "�ᡡ�������ᡡ��";
	player->shape[0][11] = "�ᡡ��ᡡ�ᡡ��";
	player->shape[0][12] = "���ᡡ���ᡡ����";

	player->shape[1][0] = "��������ᡡ����";
	player->shape[1][1] = "����ᡡ����ᡡ";
	player->shape[1][2] = "�ᡡ������������";
	player->shape[1][3] = "�ᡡ������������";
	player->shape[1][4] = "�ᡡ�����������";
	player->shape[1][5] = "�ᡡ�����ᡡ��";
	player->shape[1][6] = "�ᡡ�ᡡ��������";
	player->shape[1][7] = "���ᡡ���ᡡ�ᡡ";
	player->shape[1][8] = "�����ᡡ��������";
	player->shape[1][9] = "���������ᡡ";
	player->shape[1][10] = "�����ᡡ��������";
	player->shape[1][11] = "�����ᡡ��ᡡ��";
	player->shape[1][12] = "�������ᡡ���ᡡ";

	player->shape[2][0] = "����������ᡡ��";
	player->shape[2][1] = "�����ᡡ���ᡡ";
	player->shape[2][2] = "�ᡡ������������";
	player->shape[2][3] = "�ᡡ������������";
	player->shape[2][4] = "�ᡡ������������";
	player->shape[2][5] = "�ᡡ������������";
	player->shape[2][6] = "�ᡡ������������";
	player->shape[2][7] = "�ᡡ������������";
	player->shape[2][8] = "���ᡡ�������ᡡ";
	player->shape[2][9] = "�ᡡ����ᡡ��";
	player->shape[2][10] = "�ᡡ������������";
	player->shape[2][11] = "���ᡡ��ᡡ�ᡡ";
	player->shape[2][12] = "����ᡡ����ᡡ";

	player->shape[3][0] = "������ᡡ������";
	player->shape[3][1] = "���ᡡ�����ᡡ";
	player->shape[3][2] = "�ᡡ������������";
	player->shape[3][3] = "�ᡡ������������";
	player->shape[3][4] = "�ᡡ�������ᡡ��";
	player->shape[3][5] = "�ᡡ���ᡡ���";
	player->shape[3][6] = "��ᡡ���������";
	player->shape[3][7] = "�ᡡ�ᡡ���ᡡ��";
	player->shape[3][8] = "���ᡡ�������ᡡ";
	player->shape[3][9] = "���������";
	player->shape[3][10] = "�ᡡ������������";
	player->shape[3][11] = "���ᡡ��ᡡ�ᡡ";
	player->shape[3][12] = "����ᡡ����ᡡ";
}

void BgInit()
{
	for (int i = 0; i < FloorCount; i++)
	{
		floors[i] = (Floor*)malloc(sizeof(Floor));
		floors[i]->x = 30 * i + 10;
		floors[i]->y = 93;
		floors[i]->color = WHITE;
		floors[i]->shape[0] = "�����������������������";
		floors[i]->shape[1] = "�����������������������";
		floors[i]->shape[2] = "�����������������������";
		floors[i]->shape[3] = "�����������������������";
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
//���� �ʱ�ȭ
void InitBuffer()
{
	//���� ��ũ�� index���� 0
	screenIndex = 0;

	//size.x = 80, size.y = 40 : ȭ�� ũ��
	COORD size = { BufferWidth, BufferHeight };

	//â ũ�� ���� : 0, ���� : 0 ������ : 80 - 1, �Ʒ��� : 40 - 1
	SMALL_RECT rect = { 0,0,BufferWidth - 1, BufferHeight - 1 };

	//0�� ���� �����
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//0�� ȭ�� ���� ������ ����(���� ���� �ּ�, ũ��)
	SetConsoleScreenBufferSize(hBuffer[0], size);
	//0�� â ������ ����
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	//1�� ���� �����
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//1�� ȭ�� ���� ������ ����(���� ���� �ּ�, ũ��)
	SetConsoleScreenBufferSize(hBuffer[1], size);
	//1�� â ������ ����
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	CONSOLE_CURSOR_INFO info;	//�ܼ� Ŀ�� ���� ����ü
	info.dwSize = 1;			//Ŀ�� ũ��
	info.bVisible = FALSE;		//Ŀ�� �Ⱥ��̰�

	//0 & 1 ������ ���ۿ� Ŀ�� ���� ���
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

//���� ����
void CloseBuffer()
{
	//���� �ݱ�
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}

#pragma endregion
