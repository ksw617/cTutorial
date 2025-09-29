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
#define FloorCount 10

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
	int left;
	int top;
	int right;
	int bottom;
	float time = 0.f;
	float y = 0.f;
	float h = 0.f;
	float vo = 0.f;
	bool jump = false;
	Color color;
	const char* boxCollider[4];
	
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
	player->x = 80;
	player->y = 47;
	player->color = WHITE;
	player->dir = RIGHT;
	player->collision.h = 0.0f;
	player->collision.y = player->y;
	player->collision.jump = false;
	player->collision.time = 0.0f;
	player->collision.vo = 0.f;
	player->collision.boxCollider[0] = "�������";
	player->collision.boxCollider[1] = "�������";
	player->collision.boxCollider[2] = "�������";
	player->collision.boxCollider[3] = "�������";
	player->collision.color = RED;
	player->collision.left = player->x;
	player->collision.right = player->collision.left + 8; //player->collision.boxCollider[0]�� ����
	player->collision.top = player->y + 10;
	player->collision.bottom = player->collision.top + 4; // player->collision.boxCollider �迭 ũ��


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


	floors[6]->x = 20;
	floors[6]->y = 75;

	floors[7]->x = 50;
	floors[7]->y = 75;

	floors[8]->x = 80;
	floors[8]->y = 75;


	floors[9]->x = 80;
	floors[9]->y = 60;
}

void Init()
{
	PlayerInit();
	BgInit();
}

void Update()
{


	if (GetAsyncKeyState(VK_SPACE) && !player->collision.jump)
	{
		player->collision.jump = true;
		player->collision.vo = Vo;
		player->y--;
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

	player->collision.left = player->x + 1;
	player->collision.right = player->collision.left + 6; //player->collision.boxCollider[0]�� ����
	player->collision.top = player->y + 10;
	player->collision.bottom = player->collision.top + 4; // player->collision.boxCollider �迭 ũ��

	bool isCollision = false;

	for (int i = 0; i < FloorCount; i++)
	{
		if (floors[i]->x < player->collision.right &&
			player->collision.left < floors[i]->x + 20 &&
			floors[i]->y < player->collision.bottom &&
			player->collision.top < floors[i]->y + 4)
		{

			player->collision.vo = 0.0f;
			isCollision = true;
			player->collision.jump = false;
			player->y = floors[i]->y - 13;
		}
	}

	if (isCollision)
	{
		player->collision.time = 0.0f;
		player->collision.y = player->y;
		player->collision.h = 0.f;
		WriteBuffer(10,10,"���������������������������������������������������������������", GREEN);
	}
	else
	{


		player->collision.time += 0.1f;
		float up = (player->collision.vo * player->collision.time);
		float down = (0.5f * G * player->collision.time * player->collision.time);

		player->collision.h = -up + down;

		player->y = (int)(player->collision.y + player->collision.h);
		WriteBuffer(10, 10, "���������������������������������������������������������������", RED);
	}


	


	

	for (int i = 0; i < 13; i++)
	{
		WriteBuffer(player->x, player->y + i, player->shape[player->dir][i], player->color);
	}


	for (int i = 0; i < 4; i++)
	{
		WriteBuffer(player->collision.left, player->collision.top + i, player->collision.boxCollider[i], player->collision.color);
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
