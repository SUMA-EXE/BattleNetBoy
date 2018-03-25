#include <Arduboy2.h>
#include "bitmap.h"
#include <EEPROM.h>
#include <ArduboyTones.h>


Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);


#define ARDBITMAP_SBUF arduboy.getBuffer()//突っ込むバッファ
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;//ビットマップの奴

int xx = 0;
int BB = 0;
int BGM = 0;
int SED = 0;

int Matk[3][3] = {{5, 0, 5}, {6, 0, 10}, {9, 0, 14}};

int TIME = 0;

int R = 0;

int mode = 0;


int finish = 0;

int LEV[2] = {0}; //レベル
int CPUtype = 0;

int MAXHP[2] = {1000};

int PX[2] = {0, 3};
int PY[2] = {0};
int PHP[2] = {0};
int PG[2] = {0}; //プレイヤー画像
int PHIT[2] = {0};

int MO[2] = {0}; //モーション
int MT[2] = {0}; //モーション時間
int IT[2] = {0};

int PRM[2] = {0};//ﾗﾝﾀﾞﾑ行動用
int CPU[2] = {0, 1};

int DM[6][3] = {0};

int INM[6][3] = {0};//移動不可能
int VM[6][3] = {0};//爆発マス

int TX[10] = {0};
int TY[10] = {0};
int TP[10] = {0};
int TT[10] = {0};
int TM[10] = {0};//弾モーション
int TC = 0; //弾カウント

int OX[6] = {0}; //オブジェクト
int OY[6] = {0};
int OM[6] = {0};
int OHP[6] = {0};
int OT[6] = {0};
int OHIT[6] = {0}; //攻撃受けた
int OB[6] = {0}; //ノックバック
int ONo = 0;

int Fr[15] = {0}; //フォルダー
int FrB[15] = {0}; //フォルダー
int FrNo = 0;
int Hnd[5] = {0}; //手札
int Cchage = 0;
int CPcel = 0;
int CPco = 0;

/*const unsigned char PROGMEM rever[] =
  {
  // width, height,
  0x00, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00,
  };*/

//キャラクター描写
void GWrite (int G_x, int G_y, int G_m, int G_P, int G_t) { //x,y,画像種類,パターン,色抜き

  if (G_m == 0) { //立ち絵
    ardbitmap.drawCompressed(G_x , G_y, F1, 1 - G_P * (G_t == 0), ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, F2, G_P - 0, ALIGN_CENTER, G_P);
    }
  } else if (G_m == 1) { //バスター
    ardbitmap.drawCompressed(G_x , G_y, BA1, 1 - G_P * (G_t == 0), ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, BA2, G_P - 0, ALIGN_CENTER, G_P);
    }

  } else if (G_m == 2) { //キューブ
    ardbitmap.drawCompressed(G_x , G_y, C1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, C2, G_P - 0, ALIGN_CENTER, G_P);
    }

  } else if (G_m == 3) { //移動
    ardbitmap.drawCompressed(G_x , G_y, I1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, I2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 4) { //発射エフェクト
    ardbitmap.drawCompressed(G_x , G_y, BT1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, BT2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 5) { //爆発エフェクト
    //ardbitmap.drawCompressed(G_x , G_y, V1, 1, ALIGN_CENTER, G_P); //画像表示

    //ardbitmap.drawCompressed(G_x , G_y, V2, 0, ALIGN_CENTER, G_P);
    ardbitmap.drawCompressedResized(G_x , G_y, V1, 1, ALIGN_CENTER , G_P, 1 - (0.2 * G_P));

    ardbitmap.drawCompressedResized(G_x , G_y, V2, 0, ALIGN_CENTER , G_P, 1 - (0.2 * G_P));

  } else if (G_m == 6) { //ダメージ
    ardbitmap.drawCompressed(G_x , G_y, D1, 1 - G_P * (G_t == 0), ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, D2, G_P - 0, ALIGN_CENTER, G_P);
    }
  } else if (G_m == 7) { //弾丸
    ardbitmap.drawCompressed(G_x , G_y, DN1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, DN2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 8) { //ミニボム
    ardbitmap.drawCompressed(G_x , G_y, BM1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, BM2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 9) { //砲撃マシーン
    ardbitmap.drawCompressed(G_x , G_y, HM1, 1 , ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, HM2, 0, ALIGN_CENTER, G_P);
    }

  } else if (G_m == 10) { //エアーショット
    ardbitmap.drawCompressed(G_x , G_y, EAR1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, EAR2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 11) { //投げキューブ
    ardbitmap.drawCompressed(G_x , G_y, NC1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, NC2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 12) { //ウェーブ
    ardbitmap.drawCompressed(G_x , G_y, WA1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, WA2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 13) { //ブレイク弾
    ardbitmap.drawCompressed(G_x , G_y, BD1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, BD2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 14) { //うおおお
    ardbitmap.drawCompressed(G_x , G_y, EI1, 1 - G_P * (G_t == 0), ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, EI2, G_P - 0, ALIGN_CENTER, G_P);
    }

  } else if (G_m == 15) { //ファイヤー
    ardbitmap.drawCompressed(G_x , G_y, FF1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, FF2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 16) { //麻痺
    ardbitmap.drawCompressed(G_x , G_y, MR1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, MR2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 17) { //ブーメラン
    ardbitmap.drawCompressed(G_x , G_y, VM1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    ardbitmap.drawCompressed(G_x , G_y, VM2, G_P - 0, ALIGN_CENTER, G_P);

  } else if (G_m == 18) { //プリズム
    ardbitmap.drawCompressed(G_x , G_y, PR1, 1 - G_P, ALIGN_CENTER, G_P); //画像表示

    if (G_t == 0) {
      ardbitmap.drawCompressed(G_x , G_y, PR2, G_P - 0, ALIGN_CENTER, G_P);
    }
  }
}


void CPname(int A_n) {
  if (A_n == 5) {
    arduboy.print(F("Buster"));
  } else if (A_n == 6) {
    arduboy.print(F("Cube"));
  } else if (A_n == 9) {
    arduboy.print(F("Tank"));
  } else if (A_n == 10) {
    arduboy.print(F("Gunman"));
  } else if (A_n == 11) {
    arduboy.print(F("Air"));
  } else if (A_n == 12) {
    arduboy.print(F("Bomb"));
  } else if (A_n == 13) {
    arduboy.print(F("Throw"));
  } else if (A_n == 14) {
    arduboy.print(F("Wave"));
  } else if (A_n == 15) {
    arduboy.print(F("Break"));
  } else if (A_n == 16) {
    arduboy.print(F("Gust"));
  } else if (A_n == 17) {
    arduboy.print(F("Twin"));
  } else if (A_n == 18) {
    arduboy.print(F("Paraly"));
  } else if (A_n == 19) {
    arduboy.print(F("Boomer"));
  } else if (A_n == 20) {
    arduboy.print(F("Explos"));
  } else if (A_n == 21) {
    arduboy.print(F("Prism"));
  } else if (A_n == 22) {
    arduboy.print(F("Fire"));
  }

}

//威力
int CPpower(int A_n) {
  int No = 0;
  if (A_n == 5) {
    No = 30;
  } else if (A_n == 9) {
    No = 80;
  } else if (A_n == 11) {
    No = 40;
  } else if (A_n == 12) {
    No = 120;
  } else if (A_n == 14) {
    No = 60;
  } else if (A_n == 15) {
    No = 50;
  } else if (A_n == 17) {
    No = 60;
  } else if (A_n == 18) {
    No = 20;
  } else if (A_n == 19) {
    No = 70;
  } else if (A_n == 20) {
    No = 300;
  } else if (A_n == 22) {
    No = 120;
  }
  return No;
}

//コード
int CPcode(int A_n) {
  int No = 0;
  if (A_n == 5) {
    No = 1;
  } else if (A_n == 6) {
    No = 2;
  } else if (A_n == 9) {
    No = 1;
  } else if (A_n == 10) {
    No = 3;
  } else if (A_n == 11) {
    No = 2;
  } else if (A_n == 12) {
    No = 3;
  } else if (A_n == 13) {
    No = 1;
  } else if (A_n == 14) {
    No = 2;
  } else if (A_n == 15) {
    No = 3;
  } else if (A_n == 16) {
    No = 1;
  } else if (A_n == 17) {
    No = 2;
  } else if (A_n == 18) {
    No = 3;
  } else if (A_n == 19) {
    No = 1;
  } else if (A_n == 20) {
    No = 2;
  } else if (A_n == 21) {
    No = 3;
  } else if (A_n == 22) {
    No = 3;
  }
  return No;
}

//時間
int ATKtime (int A_n) {
  int No = 0;
  if (A_n == 5) {
    No = 4;
  } else if (A_n == 6) {
    No = 1;
  } else if (A_n == 9) {
    No = 8;
  } else if (A_n == 10) {
    No = 1;
  } else if (A_n == 11) {
    No = 6;
  } else if (A_n == 12) {
    No = 6;
  } else if (A_n == 13) {
    No = 6;
  } else if (A_n == 14) {
    No = 8;
  } else if (A_n == 15) {
    No = 8;
  } else if (A_n == 16) {
    No = 12;
  } else if (A_n == 17) {
    No = 12;
  } else if (A_n == 18) {
    No = 4;
  } else if (A_n == 19) {
    No = 6;
  } else if (A_n == 20) {
    No = 8;
  } else if (A_n == 21) {
    No = 1;
  } else if (A_n == 22) {
    No = 6;
  }


  return No;
}


void CPWrite (int C_x, int C_y, int C_m, int C_P) { //x,y,画像種類,色抜き


  if (C_m == 1) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP1, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 5) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP5, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 6) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP6, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 9) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP9, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 10) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP10, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 11) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP11, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 12) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP12, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 13) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP13, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 14) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP14, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 15) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP15, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 16) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP16, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 17) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP17, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 18) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP18, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 19) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP19, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 20) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP20, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 21) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP21, C_P, ALIGN_CENTER, 0); //画像表示
  } else  if (C_m == 22) { //立ち絵
    ardbitmap.drawCompressed(C_x , C_y, CP22, C_P, ALIGN_CENTER, 0); //画像表示
  }

}



void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();

  SED = EEPROM.read(495);
  for (int L = 0; L <= 14; L++) {
    FrB[L] = EEPROM.read(L + 496);

    if (FrB[L] == 0) {
      FrB[L] = 6 * (1 + L / 5); // 5* (L / 5 == 0) + 6 * (L / 5 == 1) + 11 * (L / 5 == 2);
    }

  }


}

void loop() {
  // put your main code here, to run repeatedly:
  if (!arduboy.nextFrame()) return;
  arduboy.clear();

  arduboy.pollButtons();
  BGM = 0;

  if (mode == 0) {
    //モード選択


    TIME = 0;



    PX[0] = 1;
    PX[1] = 4;


    for (int L = 0; L <= 1; L++) {

      PY[L] = 1;
      PHP[L] = 0;
      PHIT[L] = 0;

      MO[L] = 0; //モーション
      IT[L] = 0;

      PRM[L] = {0};//ﾗﾝﾀﾞﾑ行動用
    }
    CPU[0] = 0;



    for (int L = 0; L <= 9; L++) {
      TP[L] = 0;
      TM[L] = 0;//弾モーション
    }
    TC = 0; //弾カウント

    for (int L = 0; L <= 5; L++) {
      OM[L] = {0};
      OHP[L] = {0};
      OB[L] = {0}; //ノックバック

      for (int LL = 0; LL <= 2; LL++) {
        VM[L][LL] = 0;//爆発マス
      }

    }
    ONo = 0;

    //Fr[15]={0};//フォルダー
    FrNo = 0;
    for (int L = 0; L <= 4; L++) {
      Fr[L] = 0;
      Fr[L + 5] = 0;
      Fr[L + 10] = 0;

      Hnd[L] = 0; //手札
    }

    Cchage = 240;
    CPcel = 0;
    CPco = 0;

    finish = 0;

    if (LEV[1] == 0) {
      MAXHP[1] = 500;
    } else if (LEV[1] == 1) {
      MAXHP[1] = 1000;
    } else if (LEV[1] == 2) {
      MAXHP[1] = 2000;
    } else if (LEV[1] == 3) {
      MAXHP[1] = 3000;
    }



    arduboy.setCursor(28, 8);
    if (CPUtype == 0) {
      //バスターマン
      Matk[0][1] = 5;
      Matk[1][1] = 10;
      Matk[2][1] = 14;
      arduboy.print(F("Buster Man"));
    } else if (CPUtype == 1) {
      //キューブマン
      Matk[0][1] = 12;
      Matk[1][1] = 6;
      Matk[2][1] = 16;
      arduboy.print(F("Cube Man"));
    } else if (CPUtype == 2) {
      //タンクマン
      Matk[0][1] = 9;
      Matk[1][1] = 13;
      Matk[2][1] = 14;
      arduboy.print(F("Tank Man"));
    } else if (CPUtype == 3) {
      //ブレイクマン
      Matk[0][1] = 19;
      Matk[1][1] = 15;
      Matk[2][1] = 17;
      arduboy.print(F("Break Man"));
    } else {
      Matk[0][1] = Matk[0][0];
      Matk[1][1] = Matk[1][0];
      Matk[2][1] = Matk[2][0];
      arduboy.print(F("CPU1"));

    }

    arduboy.setCursor(28, 16);
    arduboy.print(F("LV"));
    if (LEV[1]<3){
    arduboy.print(LEV[1] + 1);
    }else{
    arduboy.print(F("SP"));
    }
    arduboy.setCursor(28, 24);
    arduboy.print(F("Card Folder"));
    arduboy.setCursor(28, 32);
    arduboy.print(F("CPU1 VS CPU2"));

    arduboy.setCursor(92, 0);
    if (SED == 1) {
      arduboy.print(F("SE:ON"));
    } else {
      arduboy.print(F("SE:OFF"));
    }

    for (int LL = 0; LL <= 2; LL = LL + 2) {
      for (int L = 0; L <= 2; L++) {
        CPWrite(24 * LL + 4 + 28, L * 8 + 44, 1, 1);
        CPWrite(24 * LL + 4 + 28, L * 8 + 44, Matk[L][LL], 0);
      }
    }

    arduboy.setCursor(20, xx * 8);
    if (xx == 11) {
      arduboy.setCursor(84, 0);
    } else if (xx > 7) {
      arduboy.setCursor(68, (xx - 3) * 8);
    }
    arduboy.print(F(">"));
    //} else{
    //arduboy.drawBitmap(48*((xx-5)/3)+28,((xx-5)%3)*8+40, rever, 8, 8, 2);
    //}
    arduboy.setCursor(28, 0);
    arduboy.print(F("1P VS CPU"));

    if (xx == 1) {
      Matk[0][2] = Matk[0][1];
      Matk[1][2] = Matk[1][1];
      Matk[2][2] = Matk[2][1];
    }


    if (arduboy.justPressed(UP_BUTTON)) {
      //BGM=3;
      xx = (xx + 11) % 12;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
      //BGM=3;
      xx = (xx + 1) % 12;
    } /*else if (arduboy.justPressed(RIGHT_BUTTON)) {
      //BGM=3;
    } else if (arduboy.justPressed(LEFT_BUTTON)) {
      //BGM=3;
      CPUtype = CPUtype - ((CPUtype != 0) and (xx == 1));
      LEV[1] = LEV[1] - ((LEV[1] != 0) and (xx == 2));
    }*/


    if (  (arduboy.justPressed(A_BUTTON)) or (arduboy.justPressed(B_BUTTON)) ) {

      CPUtype = (CPUtype + (xx == 1)) % 5;
      LEV[1] = (LEV[1] + (xx == 2)) % 4;

      mode = 0 + (xx == 0) + (xx == 3) * 2 + (xx == 4) * 3;

      if (xx == 11) {
        SED = (SED + 1) % 2;
        EEPROM.write(495, SED);
      }
      if (xx >= 5) {
        Matk[(xx - 5) % 3][((xx - 5) / 3) * 2] = (Matk[(xx - 5) % 3][((xx - 5) / 3) * 2] + 1) % 23;
      }
    }


    for (int L = 0; L <= 10; L++) {
      if ((xx >= 5) and (xx != 11)) {
        if (CPcode(Matk[(xx - 5) % 3][((xx - 5) / 3) * 2]) == 0) {
          (Matk[(xx - 5) % 3][((xx - 5) / 3) * 2] = Matk[(xx - 5) % 3][((xx - 5) / 3) * 2] + 1) % 23;
        }
      }
    }

  } else if (mode == 2) {



    TIME++;

    if (arduboy.justPressed(UP_BUTTON)) {
      xx = (xx + 15) % 16;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
      xx = (xx + 1) % 16;
    } else if ( (arduboy.justPressed(LEFT_BUTTON)) or (arduboy.justPressed(RIGHT_BUTTON)) ) {
      xx = (xx + 8) % 16;
    }

    if ( (arduboy.justPressed(A_BUTTON)) or (arduboy.justPressed(B_BUTTON)) ) {
      if (xx != 15) {
        FrB[xx] = (FrB[xx] + 1) % 23;
      } else {
        mode = 0;
        xx = 0;
        for (int L = 0; L <= 14; L++) {
          EEPROM.write(L + 496, FrB[L]);
        }


      }
    }


    for (int L = 0; L <= 10; L++) {
      if (xx != 15) {
        if (CPcode(FrB[xx]) == 0) {
          (FrB[xx] = FrB[xx] + 1) % 23;
        }
      }
    }
    //    arduboy.setCursor(64 * (xx / 8 == 1)-1, (xx % 8) * 8);
    //    arduboy.print(F(">"));
    for (int L = 0; L <= 14; L++) {
      CPWrite(64 * (L / 8 == 1) + 4, (L % 8) * 8 + 4, 1, 1);
      CPWrite(64 * (L / 8 == 1) + 4, (L % 8) * 8 + 4, FrB[L], 0);
      arduboy.setCursor(9 + 64 * (L / 8 == 1), (L % 8) * 8);
      CPname(FrB[L]);
      arduboy.setCursor(9 + 64 * (L / 8 == 1) + 37, (L % 8) * 8);
      if (CPpower(FrB[L]) != 0) {
        arduboy.print(CPpower(FrB[L]));
      } else {
        arduboy.print(F("-"));
      }
    }
    arduboy.setCursor(9 + 64, 7 * 8);
    arduboy.print(F("<OK>"));
    arduboy.drawRect(64 * (xx / 8 == 1) + 9, (xx % 8) * 8, 37, 8, 1);

    /*
      if ((TIME / 5) % 2 == 0) {
        arduboy.drawBitmap(64 * (xx / 8 == 1), (xx % 8) * 8, rever, 8, 8, 2);
      }
    */
  } else if (mode == 3) {

    Matk[0][1] = Matk[0][2];
    Matk[1][1] = Matk[1][2];
    Matk[2][1] = Matk[2][2];
    CPU[0] = 2;
    mode = 1;
    LEV[0] = 2;
    LEV[1] = 2;
    MAXHP[1] = 1000;

  } else if (mode == 1) {
    //バトルモード
    TIME++;//時間

    if ((TIME > 5) and (finish == 0)) {
      BB = 0;
      if (CPU[0] == 0) {
        if (MT[0] == 0) {
          if (arduboy.pressed(LEFT_BUTTON) and (PX[0] != 0)) {
            if (INM[PX[0] - 1][PY[0]] == 0) {
              MT[0] = 2;
              MO[0] = 4;
            }
          }
          if (arduboy.pressed(RIGHT_BUTTON) and (PX[0] != 2)) {
            if (INM[PX[0] + 1][PY[0]] == 0) {
              MT[0] = 2;
              MO[0] = 3;
            }
          }
          if (arduboy.pressed(UP_BUTTON) and (PY[0] != 0)) {
            if (INM[PX[0]][PY[0] - 1] == 0) {
              MT[0] = 2;
              MO[0] = 1;
            }
          }
          if (arduboy.pressed(DOWN_BUTTON) and (PY[0] < 2)) {
            if (INM[PX[0]][PY[0] + 1] == 0) {
              MT[0] = 2;
              MO[0] = 2;
            }
          }
        }
      }

      if ( arduboy.justPressed(A_BUTTON) ) {
        //Bボタン…チップセレクト
        CPcel = (CPcel + 1) % 5;
        if (Hnd[CPcel] == 0) {
          CPcel = 0;
        }

        BGM = 16;
      }

      if ( arduboy.justPressed(B_BUTTON) ) {
        //Aボタン
        if (  MT[0] == 0  ) {
          //攻撃
          if (Hnd[CPcel] != 0) {
            MO[0] = Hnd[CPcel];
            MT[0] = ATKtime(Hnd[CPcel]);
            if (CPco == CPcode(Hnd[CPcel])) {
              Cchage = Cchage + 26;
            }
            CPco = CPcode(Hnd[CPcel]);
            Hnd[CPcel] = 0;
            CPcel = 0;
          }

        }
      }



      //ゲージ
      Cchage++;

      FrNo = 0;
      for (int L = 0; L <= 14; L++) {
        if (Fr[L] != 0) {
          FrNo++;
        }
      }

      if (FrNo == 0) {
        for (int L = 0; L <= 14; L++) {
          Fr[L] = FrB[L];
        }
        FrNo = 15;
      }

      //フォルダー関係
      for (int L = 0; L <= 13; L++) {
        if (Fr[L] == 0) {
          Fr[L] = Fr[L + 1];
          Fr[L + 1] = 0;
        }
      }
      if ( (Hnd[4] != 0) and (Cchage > 80) ) {
        Cchage = 81;
      }

      //チップ補充
      if ( (Hnd[4] == 0) and (Cchage > 80) ) {
        R = random(FrNo);
        Hnd[4] = Fr[R];
        Fr[R] = 0;
        Cchage = Cchage - 80;
      }

      //手札調整
      for (int L = 0; L <= 3; L++) {
        if (Hnd[L] == 0) {
          Hnd[L] = Hnd[L + 1];
          Hnd[L + 1] = 0;
        }
      }


      //CPU行動用

      for (int L = 0; L <= 1; L++) {
        if (CPU[L] != 0) {
          if (MT[L] == 0 ) {
            (PRM[L] = PRM[L] + 1) % 100;
            if (PRM[L] % 10 == 2) {
              if (LEV[L] == 0) {
                MT[L] = 20;
              } else if (LEV[L] == 1) {
                MT[L] = 10;
              } else if (LEV[L] == 2){
                MT[L] = 5;
              }

              MO[L] = 23;
            } else if (PRM[L] % 10 == 0) {
              MT[L] = 4 - LEV[L];
              MO[L] = 8;
            } else if (PRM[L] % 25 == 11) {
              MT[L] = ATKtime(Matk[0][L]);
              MO[L] = Matk[0][L];
            } else if (PRM[L] % 50 == 46) {
              MT[L] = ATKtime(Matk[1][L]);
              MO[L] = Matk[1][L];
            } else if (PRM[L] % 100 == 51) {
              MT[L] = ATKtime(Matk[2][L]);
              MO[L] = Matk[2][L];
            }

          }
        }
      }





      //ダメージエリア表示初期化
      for (int x = 0; x <= 5; x++) {
        for (int y = 0; y <= 2; y++) {
          DM[x][y] = 0;
          INM[x][y] = 0;
          if (VM[x][y] > 0) {
            if (VM[x][y] == 3) {
              BGM = 1;
            }
            VM[x][y]--;
          }
        }
      }

      if (OHP[5] > 0) {
        OHP[0] = 0;
        OM[0] = 0;
        VM[OX[0]][OY[0]] = 5;
      }
      if (OHP[4] > 0) {
        OHP[0] = 0;
        OM[0] = 0;
        VM[OX[0]][OY[0]] = 5;
      }

      //オブジェ詰める
      for (int LL = 0; LL <= 5; LL++) {
        for (int L = 0; L <= 4; L++) {
          if (OHP[L] <= 0) {
            OHP[L] = OHP[L + 1];
            OM[L] = OM[L + 1];
            OX[L] = OX[L + 1];
            OY[L] = OY[L + 1];
            OB[L] = OB[L + 1];
            OT[L] = OT[L + 1];
            OHIT[L] = OHIT[L + 1];
            OHP[L + 1] = 0;
            OM[L + 1] = 0;
            /*            OT[L + 1] = 0;
                        OX[L + 1] = 0;
                        OY[L + 1] = 0;
                        OB[L + 1] = 0;
                        OHIT[L + 1] = 0;
            */
          }
        }
      }




      //オブジェクト
      for (int L = 0; L <= 5; L++) {
        if (OHP[L] != 0) {
          OT[L] = OT[L] - (OT[L] > 0);
          ONo = L;
          OHIT[L] = OHIT[L] - (OHIT[L] != 0);
          if (OB[L] == 2) {
            if (OX[L] != 0) {
              OX[L]--;
            }
          } else if (OB[L] == 1) {
            if (OX[L] != 5) {
              OX[L]++;
            }
          }

          if ((OT[L] != 0) and (OM[L] != 0)) {
            DM[OX[L]][OY[L]] = 1;
          } else {
            INM[OX[L]][OY[L]] = 1;

            if ( (OM[L] == 2) and (TIME % 20 == 0) ) {
              TM[TC] = 1;
              TX[TC] = OX[L] * 16 + 24;
              TY[TC] = OY[L];
              TP[TC] = OX[L] > 2;
              TC = (TC + 1) % 10;
              BGM = 4;
            }
            if ( (OM[L] == 3) and (OHP[L] != 100) and (OHP[L] > 0) ) {
              if (OX[L] != 5) {
                DM[OX[L] + 1][OY[L]] = 100 - OHP[L];
                VM[OX[L] + 1][OY[L]] = 4;
                if (OY[L] != 0) {
                  DM[OX[L] + 1][OY[L] - 1] = 100 - OHP[L];
                  VM[OX[L] + 1][OY[L] - 1] = 4;
                }
                if (OY[L] != 2) {
                  DM[OX[L] + 1][OY[L] + 1] = 100 - OHP[L];
                  VM[OX[L] + 1][OY[L] + 1] = 4;
                }
              }
              if (OX[L] != 0) {
                DM[OX[L] - 1][OY[L]] = 100 - OHP[L];
                VM[OX[L] - 1][OY[L]] = 4;
                if  (OY[L] != 0) {
                  DM[OX[L] - 1][OY[L] - 1] = 100 - OHP[L];
                  VM[OX[L] - 1][OY[L] - 1] = 4;
                }
                if (OY[L] != 2) {
                  DM[OX[L] - 1][OY[L] + 1] = 100 - OHP[L];
                  VM[OX[L] - 1][OY[L] + 1] = 4;
                }
              }
              if (OY[L] != 0) {
                DM[OX[L]][OY[L] - 1] = 100 - OHP[L];
                VM[OX[L]][OY[L] - 1] = 4;
              }
              if (OY[L] != 2) {
                DM[OX[L]][OY[L] + 1] = 100 - OHP[L];
                VM[OX[L]][OY[L] + 1] = 4;
              }
              /*
                if ((OX[L] != 5) and (OY[L] != 0)) {
                DM[OX[L] + 1][OY[L] - 1] = 100 - OHP[L];
                VM[OX[L] + 1][OY[L] - 1] = 4;
                }
                if ((OX[L] != 0) and (OY[L] != 0)) {
                DM[OX[L] - 1][OY[L] - 1] = 100 - OHP[L];
                VM[OX[L] - 1][OY[L] - 1] = 4;
                }
                if ((OX[L] != 5) and (OY[L] != 2)) {
                DM[OX[L] + 1][OY[L] + 1] = 100 - OHP[L];
                VM[OX[L] + 1][OY[L] + 1] = 4;
                }
                if ((OX[L] != 0) and (OY[L] != 2)) {
                DM[OX[L] - 1][OY[L] + 1] = 100 - OHP[L];
                VM[OX[L] - 1][OY[L] + 1] = 4;
                }*/
              OHP[L] = 100;
              BGM = 80;
            }


            for (int LL = 0; LL <= 1; LL++) {
              if ( (PX[LL] == OX[L]) and (PY[LL] == OY[L])) {
                PHP[LL] = PHP[LL] + 200;
                MO[LL] = 7;
                MT[LL] = 7;
                PHIT[LL] = 10;
                OHP[L] = 0;
                OM[L] = 0;

              }
            }
            for (int LL = 0; LL <= 5; LL++) {
              if ((L != LL) and (OHP[LL] > 0) and (OM[LL] != 0) and (OT[LL] == 0) and (OX[LL] == OX[L]) and (OY[LL] == OY[L])) {
                VM[OX[L]][OY[L]] = 5;

                OHP[L] = 0;
                OM[L] = 0;

                OHP[LL] = 0;
                OM[LL] = 0;

              }
            }
          }

        }  else {
          OM[L] = 0;
          OX[L] = 0;
          OY[L] = 0;
        }
      }




      //弾処理
      for (int L = 0; L <= 9; L++) {
        if (TM[L] == 0) {
          TX[L] = 0;
          TY[L] = 0;
        } else if (TM[L] != 0) {
          TX[L] = TX[L] + 16 * (1 - 2 * TP[L]) * (TM[L] == 1) + 8 * (1 - 2 * TP[L]) * (TM[L] == 2) + 12 * (1 - 2 * TP[L]) * (TM[L] == 3) + 4 * (1 - 2 * TP[L]) * (TM[L] == 4) + 4 * (1 - 2 * TP[L]) * (TM[L] == 5) + 8 * (1 - 2 * TP[L]) * (TM[L] == 6) + 4 * (1 - 2 * TP[L]) * (TM[L] == 7) + 8 * (1 - 2 * TP[L]) * (TM[L] == 8) + 4 * (1 - 2 * TP[L]) * (TM[L] == 9) + 16 * (1 - 2 * TP[L]) * (TM[L] == 10) + 8 * (1 - 2 * TP[L]) * ((TM[L] == 11) and (TT[L] <= 5)) - 8 * (1 - 2 * TP[L]) * ((TM[L] == 11) and (TT[L] > 6)) + 8 * (1 - 2 * TP[L]) * (TM[L] == 12);
          if ((TX[L] > 16) and (TX[L] < 112) ) {
            DM[((TX[L] - 16) / 16) % 6][TY[L]] = 1;
          }
          //VM[((TX[L]-16)/16)%6][TY[L]]=4;
          TT[L] = TT[L] + (TM[L] == 4) + (TM[L] == 5) + (TM[L] == 11);

          if ( (TM[L] == 4) and (TT[L] == 11) ) {
            DM[((TX[L] - 16) / 16) % 6][TY[L]] = 120;
            VM[((TX[L] - 16) / 16) % 6][TY[L]] = 4;
            if (TY[L] != 0) {
              DM[((TX[L] - 16) / 16) % 6][TY[L] - 1] = 120;
              VM[((TX[L] - 16) / 16) % 6][TY[L] - 1] = 4;
            }
            if (TY[L] != 2) {
              DM[((TX[L] - 16) / 16) % 6][TY[L] + 1] = 120;
              VM[((TX[L] - 16) / 16) % 6][TY[L] + 1] = 4;
            }

            TM[L] = 0;
          } else if ( (TM[L] == 5) and (TT[L] == 11) ) {
            OHP[ONo + 1] = 300;
            OM[ONo + 1] = 1;
            OX[ONo + 1] = ((TX[L] - 16) / 16) % 6;
            OY[ONo + 1] = TY[L];
            OT[ONo + 1] = 0;
            ONo = ONo + 1;
            TM[L] = 0;
          }


          //敵ヒット
          if ( ( ((TX[L] - 16) / 16) == PX[1 - TP[L]] ) and (TY[L] == PY[1 - TP[L]] ) and (PHIT[1 - TP[L]] <= 3 ) and (TM[L] != 4) and (TM[L] != 5) and (TM[L] != 8) ) {
            PHIT[1 - TP[L]] = 3;
            PHP[1 - TP[L]] = PHP[1 - TP[L]] + 30 * (TM[L] == 1) + 80 * (TM[L] == 2) + 40 * (TM[L] == 3) + 80 * (TM[L] == 6) + 50 * (TM[L] == 7) + 60 * (TM[L] == 9) + 20 * (TM[L] == 10) + 70 * (TM[L] == 11) + 120 * (TM[L] == 12); //玉ヒット
            if (TM[L] == 2) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 7;
              PHIT[1 - TP[L]] = 13;
              DM[PX[1 - TP[L]]][(PY[1 - TP[L]] + 1) % 3] = 80;
              DM[PX[1 - TP[L]]][(PY[1 - TP[L]] + 2) % 3] = 80;
              VM[PX[1 - TP[L]]][(PY[1 - TP[L]] + 1) % 3] = 4;
              VM[PX[1 - TP[L]]][(PY[1 - TP[L]] + 2) % 3] = 4;
            } else if (TM[L] == 3) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 18;
            } else if ((TM[L] == 6) or (TM[L] == 9) or (TM[L] == 12)) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 7;
              PHIT[1 - TP[L]] = 13;
            }/* else if (TM[L] == 9) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 7;
              PHIT[1 - TP[L]] = 13;
            }*/ else if (TM[L] == 10) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 30;
            } else if (TM[L] == 11) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 8;
              PHIT[1 - TP[L]] = 5;
            }/* else if (TM[L] == 12) {
              MO[1 - TP[L]] = 7;
              MT[1 - TP[L]] = 7;
              PHIT[1 - TP[L]] = 13;
            }*/
            if (TM[L] != 11) {
              TM[L] = 0;
            }

          }
          //オブジェヒット
          for (int LL = 0; LL <= 5; LL++) {
            if ( ( ((TX[L] - 16) / 16) % 6 == OX[LL] ) and (TY[L] == OY[LL]) and ( OHP[LL] > 0 ) and (TM[L] != 4) and (TM[L] != 5) and (TM[L] != 6) and ((TM[L] != 1) or ((OX[LL] <= 2) == TP[L]) or (OM[LL] != 2) ) ) {
              OHIT[LL] = 3;
              OHP[LL] = OHP[LL] - 30 * (TM[L] == 1) - 80 * (TM[L] == 2) - 40 * (TM[L] == 3) - 1000 * (TM[L] == 7) - 60 * (TM[L] == 9) - 20 * (TM[L] == 10) - 70 * (TM[L] == 11) - 120 * (TM[L] == 12);
              if (TM[L] == 2) {
                DM[OX[LL]][(OY[LL] + 1) % 3] = 80;
                DM[OX[LL]][(OY[LL] + 2) % 3] = 80;
                VM[OX[LL]][(OY[LL] + 1) % 3] = 4;
                VM[OX[LL]][(OY[LL] + 2) % 3] = 4;
              } else if (((TM[L] == 3) or (TM[L] == 8)) and (OT[LL] == 0)) {
                OB[LL] = TP[L] + 1;
              }
              if (TM[L] != 7) {
                TM[L] = 0;
              }
            }
          }
        }
        if ( (TX[L] < 16) or (TX[L] > 112) ) {
          TM[L] = 0;
        }


      }


      //燃えるボディー攻撃
      for (int L = 0; L <= 1; L++) {
        if ( (MT[L] == 2) and (MO[L] == 20) ) {
          if (PX[L] != 5) {
            DM[PX[L] + 1][PY[L]] = 300;
            VM[PX[L] + 1][PY[L]] = 4;
          }
          if (PX[L] != 0) {
            DM[PX[L] - 1][PY[L]] = 300;
            VM[PX[L] - 1][PY[L]] = 4;
          }
          if (PY[L] != 0) {
            DM[PX[L]][PY[L] - 1] = 300;
            VM[PX[L]][PY[L] - 1] = 4;
          }
          if (PY[L] != 2) {
            DM[PX[L]][PY[L] + 1] = 300;
            VM[PX[L]][PY[L] + 1] = 4;
          }
        }
      }


      //ダメージ爆風用
      for (int x = 0; x <= 5; x++) {
        for (int y = 0; y <= 2; y++) {
          if (DM[x][y] >= 10) {
            for (int L = 0; L <= 1; L++) {
              if ((PX[L] == x) and (PY[L] == y) and (PHIT[L] <= 3 ) ) {
                PHP[L] = PHP[L] + DM[x][y];
                MO[L] = 7;
                MT[L] = 7;
                PHIT[L] = 14;
              }
            }
            for (int L = 0; L <= 5; L++) {
              if ((OX[L] == x) and (OY[L] == y) ) {
                OHP[L] = OHP[L] - DM[x][y];
                OHIT[L] = 3;
              }
            }
          }
        }
      }









      //処理//モーション/1-4:↑↓→←移動/
      for (int L = 0; L <= 1; L++) {
        PG[L] = 0;
        PHIT[L] = PHIT[L] - (PHIT[L] != 0); //攻撃ヒット
        if (MT[L] > 0) {
          MT[L]--;
          if ( (MO[L] >= 1) and (MO[L] <= 4) ) { //1-4移動
            PG[L] = 3;
            if (MT[L] == 0) {
              PX[L] = (PX[L] + (MO[L] == 3) + (MO[L] == 4) * 5) % 6;
              PY[L] = (PY[L] + (MO[L] == 2) + (MO[L] == 1) * 2) % 3;
            }
          } else if (MO[L] == 5) { //バスター
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 1;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              if (L == 0) {
                Cchage = Cchage + 26;
              }
              BGM = 4;
            }
          } else if (MO[L] == 6) { //キューブ落下
            PG[L] = 1;
            MT[L] = 0;

            OHP[ONo + 1] = 300;
            OM[ONo + 1] = 1;
            OX[ONo + 1] = PX[L] + 1 - L * 2;
            OY[ONo + 1] = PY[L];
            OT[ONo + 1] = 10;
            ONo = ONo + 1;
            BGM = 5;
          } else if (MO[L] == 7) { //ダメージ
            PG[L] = 6;
          } else if ( MO[L] == 8 ) { //ワープ移動
            PG[L] = 3;
            R = random(9);
            if ( (MT[L] == 0) and (INM[L * 3 + R % 3][R / 3]) == 0 and (DM[L * 3 + R % 3][R / 3]) == 0 ) {
              PX[L] = L * 3 + R % 3;
              PY[L] = R / 3;
            }
          } else if (MO[L] == 9) { //キャノン
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 2;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 3;
            }
          } else if (MO[L] == 10) { //砲台落下
            PG[L] = 1;
            MT[L] = 0;

            OHP[ONo + 1] = 150;
            OM[ONo + 1] = 2;
            OX[ONo + 1] = PX[L] + 1 - L * 2;
            OY[ONo + 1] = PY[L];
            OT[ONo + 1] = 10;
            ONo = ONo + 1;
            BGM = 5;
          } else if (MO[L] == 11) { //エアーショット
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 3;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 12;
            }
          } else if (MO[L] == 12) { //ボム
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 4;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TT[TC] = 0;
              TC = (TC + 1) % 10;
              BGM = 12;
            }
          } else if (MO[L] == 13) { //投げキューブ
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 5;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TT[TC] = 0;
              TC = (TC + 1) % 10;
              BGM = 12;
            }
          } else if (MO[L] == 14) { //ウェーブ
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 6;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 7;
            }
          } else if (MO[L] == 15) { //ブレイク弾
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 7;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 5;
            }
          } else if (MO[L] == 16) { //ポルターガイスト
            PG[L] = 14;
            if (MT[L] == 2) {
              TM[TC] = 8;
              TX[TC] = (L * 5) * 17 + 20;
              TY[TC] = 0;
              TP[TC] = L;
              TC = (TC + 1) % 10;

              TM[TC] = 8;
              TX[TC] = (L * 5) * 17 + 20;
              TY[TC] = 1;
              TP[TC] = L;
              TC = (TC + 1) % 10;

              TM[TC] = 8;
              TX[TC] = (L * 5) * 17 + 20;
              TY[TC] = 2;
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 18;
            }
          } else if (MO[L] == 17) { //ファイヤー
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 9;
              TX[TC] = (L * 5) * 17 + 20;
              TY[TC] = (PY[L] + 1) % 3;
              TP[TC] = L;
              TC = (TC + 1) % 10;
              TM[TC] = 9;
              TX[TC] = (L * 5) * 17 + 20;
              TY[TC] = (PY[L] + 2) % 3;
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 6;
            }
          } else if (MO[L] == 18) { //麻痺攻撃
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 10;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 14;
            }
          } else if (MO[L] == 19) { //ブーメラン
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 11;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TT[TC] = 0;
              TC = (TC + 1) % 10;
              BGM = 10;
            }
          } else if (MO[L] == 20) { //ボディー
            PG[L] = 14;

          } else if (MO[L] == 21) { //プリズム落下
            PG[L] = 1;
            MT[L] = 0;

            OHP[ONo + 1] = 100;
            OM[ONo + 1] = 3;
            OX[ONo + 1] = PX[L] + 1 - L * 2;
            OY[ONo + 1] = PY[L];
            OT[ONo + 1] = 10;
            ONo = ONo + 1;
            BGM = 5;
          }  else if (MO[L] == 22) { //ファイヤーショット
            PG[L] = 1;
            if (MT[L] == 2) {
              TM[TC] = 12;
              TX[TC] = PX[L] * 16 + 24;
              TY[TC] = PY[L];
              TP[TC] = L;
              TC = (TC + 1) % 10;
              BGM = 8;
            }
          }/*  else if (MO[L] == 23) {//何もしない
            PG[L] = 0;
          }*/



        }

      }


    }
    //エリア表示
    for (int x = 0; x <= 5; x++) {
      for (int y = 0; y <= 2; y++) {
        ardbitmap.drawCompressed(16 + 8 + x * 16 , 32 + 4 + 8 * y - 4, fild2, (x <= 2 )xor( DM[x][y] != 0), ALIGN_CENTER, 0);
        ardbitmap.drawCompressed(16 + 8 + x * 16 , 32 + 4 + 8 * y - 4, fild1, (x > 2 )xor( DM[x][y] != 0), ALIGN_CENTER, 0);
      }
    }



    for (int LLL = 0; LLL <= 2; LLL++) {//画像ループ

      //オブジェクト
      for (int L = 0; L <= 5; L++) {
        if ( (OHP[L] != 0) and (OY[L] == LLL) ) {
          if (OM[L] == 1) {
            GWrite (16 + 8 + OX[L] * 16, 22 + 8 + OY[L] * 8 - OT[L] * 8 - 4, 2, 0 , (OHIT[L] != 0) );
          } else if (OM[L] == 2) {
            GWrite (16 + 8 + OX[L] * 16, 22 + 8 + OY[L] * 8 - OT[L] * 8 - 4, 9, OX[L] > 2  , (OHIT[L] != 0) );
          } else if (OM[L] == 3) {
            GWrite (16 + 8 + OX[L] * 16, 22 + 8 + OY[L] * 8 - OT[L] * 8 - 4, 18, 0  , (OHIT[L] != 0) );
          }
        }
      }


      //画像表示
      for (int L = 0; L <= 1; L++) {
        if (PY[L] == LLL) {
          GWrite (16 + 8 + PX[L] * 16, 22 + 8 + PY[L] * 8 - 4, PG[L], L , (PHIT[L] != 0) );
          if ( ( MT[L] == 1 ) and ((MO[L] == 5) or (MO[L] == 9) ) ) {
            GWrite (PX[L] * 16 + 12 + 24 * (1 - L), 22 + 8 + PY[L] * 8 - 3 - 4, 4, L , 0);
          }
        }
      }

      //弾丸
      for (int L = 0; L <= 9; L++) {
        if (TY[L] == LLL) {
          if ( TM[L] == 2 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 3 - 4, 7, TP[L], 0);
          } else if ( TM[L] == 3 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 3 - 4, 10, TP[L], 0);
          } else if ( TM[L] == 4 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 3 + (5 - TT[L]) * (5 - TT[L]) - 25 - 4, 8, TP[L], 0);
          } else if ( TM[L] == 5 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 3 + (5 - TT[L]) * (5 - TT[L]) - 25 - 4, 11, TP[L], 0);
          } else if ( TM[L] == 6 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 12, TP[L], 0);
          } else if ( TM[L] == 7 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 13, TP[L], 0);
          } else if ( TM[L] == 9 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 15, TP[L], 0);
          } else if ( TM[L] == 10 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 16, TP[L], 0);
          } else if ( TM[L] == 11 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 17, TP[L], 0);
          } else if ( TM[L] == 12 ) {
            GWrite (TX[L] , 22 + 8 + LLL * 8 - 4, 15, TP[L], 0);
          }
        }
      }

      //爆発エフェクト
      for (int L = 0; L <= 5; L++) {
        if (VM[L][LLL] != 0) {
          GWrite (16 + 8 + L * 16, 22 + 8 + LLL * 8 - 4, 5, VM[L][LLL], 0);
          /*if (VM[L][LLL] == 3){
            BGM=1;
            }*/
        }
      }



    }//画像ループエンド


    if (CPU[0] == 0) {
      //手札
      for (int L = 0; L <= 4; L++) {
        if (Hnd[L] != 0) {
          CPWrite(10 + 12 * L - 2, 56 + 2, 1, 1);
          CPWrite(10 + 12 * L - 2, 56 + 2, Hnd[L], 0);
        }
      }
      if ((TIME / 5) % 2 == 0) {
        //arduboy.drawBitmap(10 + 12 * CPcel - 4 - 2, 56 - 4 + 2, rever, 8, 8, 2);
      }

      arduboy.drawRect(4 + 12 * CPcel - 2, 52, 12, 12, 1);
      //if (CPco == 0) {
      /*arduboy.drawFastVLine(4 + 12 * CPcel - 2, 52, 12, 1);
        arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 52, 12, 1);
        arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52, 12, 1);
        arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52 + 11, 12, 1);
      */
      //} else
      if (CPco == 1) {
        /*
          arduboy.drawFastVLine(4 + 12 * CPcel - 2, 57, 7, 1);
          arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 57, 7, 1);

          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52, 12, 1);
          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52 + 11, 12, 1);
        */
        arduboy.drawFastVLine(4 + 12 * CPcel - 2, 53, 4, 0);
        arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 53, 4, 0);
      } else if (CPco == 2) {
        /*
          arduboy.drawFastVLine(4 + 12 * CPcel - 2, 52, 4, 1);
          arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 52, 4, 1);
          arduboy.drawFastVLine(4 + 12 * CPcel - 2, 58 + 2, 4, 1);
          arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 58 + 2, 4, 1);

          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52, 12, 1);
          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52 + 11, 12, 1);

        */

        arduboy.drawFastVLine(4 + 12 * CPcel - 2, 56, 4, 0);
        arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 56, 4, 0);
      } else if (CPco == 3) {
        /*
          arduboy.drawFastVLine(4 + 12 * CPcel - 2, 52, 7, 1);
          arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 52, 7, 1);

          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52, 12, 1);
          arduboy.drawFastHLine(4 + 12 * CPcel - 2, 52 + 11, 12, 1);
        */
        arduboy.drawFastVLine(4 + 12 * CPcel - 2, 59, 4, 0);
        arduboy.drawFastVLine(4 + 12 * CPcel + 11 - 2, 59, 4, 0);
      }
      arduboy.setCursor(64, 56);
      CPname(Hnd[CPcel]);
      arduboy.drawRect(0, 8, 8, 44, 1);
      arduboy.fillRect(2, 10, 4, (Cchage / 2) * (Cchage <= 80) + 40 * (Cchage > 80), 1);

      arduboy.setCursor(104, 56);
      if (CPpower(Hnd[CPcel]) != 0) {
        arduboy.print(CPpower(Hnd[CPcel]));
      } else {
        arduboy.print(F("-"));
      }

    }
    arduboy.setCursor(0, 0);
    arduboy.print(F("P1:HP"));
    arduboy.print(MAXHP[0] - PHP[0]);

    arduboy.setCursor(64, 0);
    arduboy.print(F("P2:HP"));
    arduboy.print(MAXHP[1] - PHP[1]);

    arduboy.setCursor(44, 20);

    if (MAXHP[1] - PHP[1] <= 0) {
      finish = 1;
    } else if (MAXHP[0] - PHP[0] <= 0) {
      finish = 2;
    }


    if ((TIME < 5) or (finish != 0)) {


      if (finish == 0) {
        if (TIME >= 3) {
          arduboy.print(F("Fight!!"));
        } else {
          arduboy.print(F("Ready?"));
        }
      }

      if (finish == 1) {
        arduboy.print(F("Play1 win"));
      } else if (finish == 2) {
        arduboy.print(F("Play2 win"));
      }
      if (finish != 0) {
        arduboy.setCursor(48, 28);
        arduboy.print(F("TIME:"));
        arduboy.print(TIME);
        arduboy.setCursor(44, 36);
        TIME--;
        arduboy.print(F("pushA&B"));
        if ( (arduboy.pressed(A_BUTTON)) and (arduboy.pressed(B_BUTTON)) ) {
          mode = 0;
        }


      }

      delay(750)    ;
    }

    /*
      arduboy.print(FrNo);
      arduboy.print(CPco);
      arduboy.print(F("\n"));
      arduboy.print(OM[0]);
      arduboy.print(OM[1]);
      arduboy.print(OM[2]);
      arduboy.print(OM[3]);
      arduboy.print(OM[4]);
      arduboy.print(random(10));
    */
  }

  if ((SED == 1) and (BGM != 0)) {
    sound.tone(50 * BGM, 100); //爆発
  }

  arduboy.display();
}
