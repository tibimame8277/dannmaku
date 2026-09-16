#include "DxLib.h"
#define PI    3.1415926535897932384626433832795
char Key[256];
int cnt = 0;
void Check_Key() {
	GetHitKeyStateAll(Key);
};
void ATTACK(int x[40], int y[40], int flag[40],int pl_x,int pl_y,int *bosshp) {

	for (int i = 0;i != 40;i++) {
		if (flag[i] == 1) {
			y[i] -= 10;
			if(y[i] < -50) {
				flag[i] = 0;
			}
		}
		if (flag[i] == 0) {
			y[i] = pl_y;
			x[i] = pl_x;
		}
		if (flag[i] == 2) {
			y[i] = pl_y;
			x[i] = pl_x;
			*bosshp--;
			flag[i] = 0;
		}
	}
};
void CKhit(int x[40], int y[40], int flag[40], int boss_x, int boss_y) {
	//ここの判定がすべて動作していないから判定式の実験を行うように
	for (int i = 0;i != 40;i++) {//									下の制限をのぞいたand回路は動く
		if (x[i]-3> boss_x-35 &&y[i]-8>boss_y-60 && x[i]-3 < boss_x+35&&x[i] - 8 < boss_y + 70){
			flag[i] = 2;
		}
		if (x[i] - 3 > boss_x - 35 && y[i]+8 > boss_y - 60 && x[i] - 3 < boss_x + 35 && x[i] + 8 < boss_y + 70) {
			flag[i] = 2;
		}
		if (x[i] + 3 > boss_x - 35 && y[i]-8 > boss_y - 60 && x[i] + 3 < boss_x + 35 && x[i] - 8 < boss_y + 70) {
			flag[i] = 2;
		}
		if (x[i] + 3 > boss_x - 35 && y[i]+8 > boss_y - 60 && x[i] + 3 < boss_x + 35 && x[i] + 8 < boss_y + 70) {
			flag[i] = 2;
		}
	}
};
struct xy { int x;int y; };
struct bonexy { int x[40];int y[40];int Flag[40]; };//攻撃用で使ってる

int PlayerX, PlayerY;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	xy boss = { 320,75 };
	xy pl = { 320,640 };
	xy bone[4] = {
	{320,640},
	{320,640 }, 
	{320,640 }, 
	{320,640 }, 
	};
	bonexy bones[4] = { 
		{{0},{0},0 },
		{{0},{0},0 }, 
		{{0},{0},0 },
		{{0},{0},0 }, };	
	int boss_x1 = 0, boss_y1 = 0, boss_hp=1000,speed = 7;
	int attackcnt = 0;
	int MouseX=0, MouseY=0;
	ChangeWindowMode(TRUE);
	// 画面モードのセット    　　 　　透過色の設定　(真緑)	　　　　描画先画面を裏画面にセット
	SetGraphMode(640, 720, 16);		SetTransColor(0, 255, 0);		SetDrawScreen(DX_SCREEN_BACK);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}
	int boss_png = LoadGraph("boss.png");
	int oura = LoadGraph("o-ra.png");
	int dog = LoadGraph("dog.png");
	int bonepng = LoadGraph("bone.png");

	int shiro = GetColor(255, 255, 255);
	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		DrawBox(0, 0, 2000, 2000, GetColor(50, 50, 50), TRUE);//背景を黒に固定している
		Check_Key();//上のチェックキーを呼び出してる
		for (int i = 0;i != 4;i++) {
		ATTACK(bones[i].x, bones[i].y, bones[i].Flag, bone[i].x, bone[i].y,&boss_hp);
		CKhit(bones[i].x, bones[i].y, bones[i].Flag,boss.x,boss.y);
	}
		//0から左端　3が右端
		if (Key[KEY_INPUT_SPACE] == 1) {
			bone[0].x = pl.x - 15;
			bone[1].x = pl.x - 5;
			bone[2].x = pl.x + 5;
			bone[3].x = pl.x + 15;

			bone[0].y = pl.y - 30;
			bone[1].y = pl.y - 30;
			bone[2].y = pl.y - 30;
			bone[3].y = pl.y - 30;
			speed = 3;
		}
		else {
			bone[0].x = pl.x - 60;
			bone[1].x = pl.x - 35;
			bone[2].x = pl.x + 35;
			bone[3].x = pl.x + 60;

			bone[0].y = pl.y;
			bone[1].y = pl.y - 25;
			bone[2].y = pl.y - 25;
			bone[3].y = pl.y;
			speed = 5;
		}

		if (Key[KEY_INPUT_Z] == 1) {
			if (cnt % 3 == 0) {
			attackcnt = attackcnt % 39;
				attackcnt++;
				for (int i = 0;i != 4;i++) {
					bones[i].Flag[attackcnt] = 1;
				}
			}
		}

		if (Key[KEY_INPUT_UP] == 1) {
			pl.y -= speed;
		}
		if (Key[KEY_INPUT_DOWN] == 1) {
			pl.y += speed;
		}
		if (Key[KEY_INPUT_RIGHT] == 1) {
			pl.x += speed;
		}
		if (Key[KEY_INPUT_LEFT] == 1) {
			pl.x -= speed;
		}
		//DrawGraph(x, y, dog, TRUE);
		boss_x1 = boss.x;boss_y1 = boss.y;
		SetDrawBright(50, 50, 50);
		DrawRotaGraph(boss_x1, boss_y1, 0.5, 0, oura, TRUE);//     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		SetDrawBright(250, 250, 250);//									次は当たり判定作れよ
		DrawRotaGraph(boss.x, boss.y, 0.40, 0 , oura, TRUE);//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		DrawBox(boss.x-35, boss.y-60, boss.x+35, boss.y+70, GetColor(0, 0, 255), TRUE); //	敵の当たり判定の概要　boss.x-35, boss.y-60, boss.x+35, boss.y+70
		DrawRotaGraph(boss.x, boss.y, 0.2, 0, boss_png, TRUE);//                                                  bones[j].x[i]-3, bones[j].y[i]-8, bones[j].x[i]+3, bones[j].y[i]+8
		//DrawGraph(X, Y, dog, TRUE);
		DrawRotaGraph(pl.x, pl.y, 0.25, 0, dog, TRUE);
		for (int i = 0;i != 4;i++) {
			DrawRotaGraph(bone[i].x, bone[i].y, 0.25, PI / 2, bonepng, TRUE);
		}
	//　↓攻撃骨の表示　↑自機周辺の骨
		for (int j = 0;j != 4;j++) {
		for (int i = 0;i != 40;i++) {
			DrawRotaGraph(bones[j].x[i], bones[j].y[i], 0.25, PI / 2, bonepng, TRUE);
			DrawBox(bones[j].x[i]-3, bones[j].y[i]-8, bones[j].x[i]+3, bones[j].y[i]+8, GetColor(0,255,255), TRUE); //	攻撃の当たり判定の概要　
		}
	}
		//でバック用文字列たち
		DrawFormatString(0, 0, shiro,"boss hp %d",boss_hp);


		//常に加算を続ける系のやつ
		cnt++;
		ScreenFlip();//作画のメインループ　
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}