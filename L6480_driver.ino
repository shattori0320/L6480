//OC_SDの働くL6480向けのスケッチ

#include <SPI.h>
#include <MsTimer2.h>

// ピン定義。
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9

#define LED (7)

void setup()
{
  delay(1000);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,0);
  Serial.begin(9600);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  digitalWrite(PIN_SPI_SS, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(SPI_CLOCK_DIV128);

  while(!L6480_getstatus()){
    Serial.println("Communication Error");
    delay(1000);
  }
  L6480_resetdevice(); //L6480リセット
  L6480_setup();  //L6480を設定
  L6480_getstatus();//フラグリセット

  
  MsTimer2::set(100, fulash); // タイマー割り込み、現在座標と速度を表示
  MsTimer2::start();
  delay(1000);

  
  
 L6480_softstop();
 delay(500);

 /*
  for(int k=2;k<400;k++){  
    L6480_run(1,stepspeed(k)); //指定方向に連続回転
    //delay(5);
    //Serial.println(L6480_getstatus(),HEX);
    delay(100);
  }
  */

  int rot = 402;
  int mul = 10;
  L6480_move(1,rot*mul);
  delay(4000);
  L6480_move(0,rot*mul);
  delay(4000);
  L6480_softstop();
}


void loop(){
}

long stepspeed(long k){//速度換算
  long val=k/0.01490116119;
  return val;
}

void L6480_setup(){
  L6480_setparam_acc(0x0050);//0x8a); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6480_setparam_dec(0x0050);//0x8a); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6480_setparam_maxspeed(0x25);//0x1b); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
  L6480_setparam_minspeed(0x0);//0x1010); //[R, WS]最小速度default 0x0000 (1+12bit) (0.238*val+[step/s])
   L6480_setparam_fsspd(0x0035); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
  L6480_setparam_kvalhold(0x40);//0x50); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6480_setparam_kvalrun(0x64);//0x68); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6480_setparam_kvalacc(0x64);//0x68); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6480_setparam_kvaldec(0x64);//0x68); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)

  L6480_setparam_intspd(0x0000);//0x508); //245[R, WH]逆起電力補償切替点速度default 0x0408 (14bit) (0.238*val[step/s])
  L6480_setparam_stslp(0x35);//0x26); //[R, WH]逆起電力補償低速時勾配default 0x19 (8bit) (0.000015*val[% s/step])
  L6480_setparam_fnslpacc(0x40);//0x65); //[R, WH]逆起電力補償高速時加速勾配default 0x29 (8bit) (0.000015*val[% s/step])
  L6480_setparam_fnslpdec(0x70);//0x65); //[R, WH]逆起電力補償高速時減速勾配default 0x29 (8bit) (0.000015*val[% s/step])
  L6480_setparam_ktherm(0x1); //[R, WR]不明default 0x0 (4bit) (0.03125*val+1)
  L6480_setparam_ocdth(0x15);//0x1f); //[R, WR]過電流しきい値default 0x8 (5bit) (31.25*val+31.25[mV])
  L6480_setparam_stallth(0x0c);//0x1f); //[R, WR]失速電流しきい値？default 0x10 (5bit) (31.25*val+31.25[mV])
  L6480_setparam_stepmood(0x01);//0x07); //[R, WH]ステップモードdefault 0x07 (8bit)L6480_setparam_alareen(val); //[R, WS]有効アラームdefault 0xff (1+1+1+1+1+1+1+1bit)
  L6480_setparam_alareen(0xff);//0xde); //[R, WS]有効アラームdefault 0xff (1+1+1+1+1+1+1+1bit)
  L6480_setparam_gatecfg1(0x0057);//0x05D);//[R, WH]geta driver configuration //default 0x000(1+3+3+5bit)
  L6480_setparam_gatecfg2(0x89);//0x00);//[R, WH]geta driver configuration //default 0x00(3+5bit)
  L6480_setparam_config(0x1e81);//0x3728); //[R, WH]各種設定default 0x2e88 (3+3+1+1+1+1+1+1+1+3bit)  
}

void fulash(){
  Serial.print(L6480_getparam_abspos(),HEX);
  Serial.print("  ");
  Serial.println(L6480_getparam_speed(),HEX);
}
