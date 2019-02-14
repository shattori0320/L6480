//ver 1.01 2013/12/31 getparam_gatecfg1&2コマンド修正
//ver 1.02 2014/1/6コメント、bit値の修正

/*L6480 コントロール　コマンド
 引数-----------------------
 dia   1:正転 0:逆転,
 spd  (20bit)(0.01490*spd[step/s])
 pos  (22bit)
 n_step (22bit)
 act   1:現在座標をマーク  0:現在座標をリセット
 mssec ミリ秒
 val 各レジスタに書き込む値
 ---------------------------
 L6480_run(dia,spd);//指定方向に連続回転
 L6480_stepclock(dia);//指定方向にstepピンのクロックで回転
 L6480_move(dia,n_step);//指定方向に指定数ステップする 
 L6480_goto(pos);//指定座標に最短でいける回転方向で移動
 L6480_gotodia(dia,pos);//回転方向を指定して指定座標に移動
 L6480_gountil(act,dia,spd);//指定した回転方向に指定した速度で回転し、スイッチのONで急停止と座標処理
 L6480_relesesw(act,dia);//スイッチがOFFに戻るまで最低速度で回転し、停止と座標処理
 L6480_gohome();//座標原点に移動
 L6480_gomark();//マーク座標に移動
 L6480_resetpos();//絶対座標リセット
 L6480_resetdevice();//L6470リセット
 L6480_softstop();//回転停止、保持トルクあり
 L6480_hardstop();//回転急停止、保持トルクあり
 L6480_softhiz();//回転停止、保持トルクなし
 L6480_hardhiz();//回転急停止、保持トルクなし
 L6480_getstatus();//statusレジスタの値を返す （L6480_getparam_status();と同じ）ただしフラグがリセットされる
 
 L6480_busydelay(msec);　//busyフラグがHIGHになってから、指定ミリ秒待つ。
 
 レジスタ書き込みコマンド
 L6480_setparam_abspos(val); //[R, WS]現在座標default 0x000000 (22bit)
 L6480_setparam_elpos(val); //[R, WS]コイル励磁の電気的位置default 0x000 (2+7bit)
 L6480_setparam_mark(val); //[R, WR]マーク座標default 0x000000 (22bit)
 //使えません→L6480_setparam_spped //[R] 現在速度read onry  (20bit)
 L6480_setparam_acc(val); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
 L6480_setparam_dec(val); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
 L6480_setparam_maxspeed(val); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
 L6480_setparam_minspeed(val); //[R, WS]最小速度default 0x0000 (1+12bit) (0.238*val+[step/s])
 L6480_setparam_fsspd(val); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
 L6480_setparam_kvalhold(val); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6480_setparam_kvalrun(val); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6480_setparam_kvalacc(val); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6480_setparam_kvaldec(val); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6480_setparam_intspd(val); //[R, WH]逆起電力補償切替点速度default 0x0408 (14bit) (0.238*val[step/s])
 L6480_setparam_stslp(val); //[R, WH]逆起電力補償低速時勾配default 0x19 (8bit) (0.000015*val[% s/step])
 L6480_setparam_fnslpacc(val); //[R, WH]逆起電力補償高速時加速勾配default 0x29 (8bit) (0.000015*val[% s/step])
 L6480_setparam_fnslpdec(val); //[R, WH]逆起電力補償高速時減速勾配default 0x29 (8bit) (0.000015*val[% s/step])
 L6480_setparam_ktherm(val); //[R, WR]不明default 0x0 (4bit) (0.03125*val+1)
 //使えません→L6480_setparam_adcout //[R] read onry (5bit) ADCによる逆起電力補償の大きさかな？
 L6480_setparam_ocdth(val); //[R, WR]過電流しきい値default 0x8 (5bit) (31.25*val+31.25[mV])
 L6480_setparam_stallth(val); //[R, WR]失速電流しきい値？default 0x10 (5bit) (31.25*val+31.25[mV])
 L6480_setparam_stepmood(val); //[R, WH]ステップモードdefault 0x07 (8bit)
 L6480_setparam_alareen(val); //[R, WS]有効アラームdefault 0xff (1+1+1+1+1+1+1+1bit)
 L6480_setparam_gatecfg1(val);//[R, WH]geta driver configuration //default 0x000(1+3+3+5bit)
 L6480_setparam_gatecfg2(val);//[R, WH]geta driver configuration //default 0x00(3+5bit)
 L6480_setparam_config(val); //[R, WH]各種設定default 0x2e88 (3+3+1+1+1+1+1+1+1+3bit)
 //使えません→L6480_setparam_status //[R]状態read onry (16bit)

 [R]:読み取り専用
 [WR]:いつでも書き換え可
 [WH]:書き込みは出力がハイインピーダンスの時のみ可
 [WS]:書き換えはモータが停止している時のみ可
 
 
 レジスタ読み込みコマンド(返り値　long型)
 L6480_getparam_abspos();
 L6480_getparam_elpos();
 L6480_getparam_mark();
 L6480_getparam_speed();
 L6480_getparam_acc();
 L6480_getparam_dec();
 L6480_getparam_maxspeed();
 L6480_getparam_minspeed();
 L6480_getparam_fsspd();
 L6480_getparam_kvalhold();
 L6480_getparam_kvalrun();
 L6480_getparam_kvalacc();
 L6480_getparam_kvaldec();
 L6480_getparam_intspd();
 L6480_getparam_stslp();
 L6480_getparam_fnslpacc();
 L6480_getparam_fnslpdec();
 L6480_getparam_ktherm();
 L6480_getparam_adcout();
 L6480_getparam_ocdth();
 L6480_getparam_stallth();
 L6480_getparam_stepmood();
 L6480_getparam_alareen();
 L6480_getparam_gatecfg1();
 L6480_getparam_gatecfg2();
 L6480_getparam_config();
 L6480_getparam_status();
 */



void L6480_setparam_abspos(long val){L6480_transfer(0x01,3,val);}
void L6480_setparam_elpos(long val){L6480_transfer(0x02,2,val);}
void L6480_setparam_mark(long val){L6480_transfer(0x03,3,val);}
void L6480_setparam_acc(long val){L6480_transfer(0x05,2,val);}
void L6480_setparam_dec(long val){L6480_transfer(0x06,2,val);}
void L6480_setparam_maxspeed(long val){L6480_transfer(0x07,2,val);}
void L6480_setparam_minspeed(long val){L6480_transfer(0x08,2,val);}
void L6480_setparam_fsspd(long val){L6480_transfer(0x15,2,val);}
void L6480_setparam_kvalhold(long val){L6480_transfer(0x09,1,val);}
void L6480_setparam_kvalrun(long val){L6480_transfer(0x0a,1,val);}
void L6480_setparam_kvalacc(long val){L6480_transfer(0x0b,1,val);}
void L6480_setparam_kvaldec(long val){L6480_transfer(0x0c,1,val);}
void L6480_setparam_intspd(long val){L6480_transfer(0x0d,2,val);}
void L6480_setparam_stslp(long val){L6480_transfer(0x0e,1,val);}
void L6480_setparam_fnslpacc(long val){L6480_transfer(0x0f,1,val);}
void L6480_setparam_fnslpdec(long val){L6480_transfer(0x10,1,val);}
void L6480_setparam_ktherm(long val){L6480_transfer(0x11,1,val);}
void L6480_setparam_ocdth(long val){L6480_transfer(0x13,1,val);}
void L6480_setparam_stallth(long val){L6480_transfer(0x14,1,val);}
void L6480_setparam_stepmood(long val){L6480_transfer(0x16,1,val);}
void L6480_setparam_alareen(long val){L6480_transfer(0x17,1,val);}
void L6480_setparam_gatecfg1(long val){L6480_transfer(0x18,2,val);}
void L6480_setparam_gatecfg2(long val){L6480_transfer(0x19,1,val);}
void L6480_setparam_config(long val){L6480_transfer(0x1A,2,val);}

long L6480_getparam_abspos(){return L6480_getparam(0x01,3);}
long L6480_getparam_elpos(){return L6480_getparam(0x02,2);}
long L6480_getparam_mark(){return L6480_getparam(0x03,3);}
long L6480_getparam_speed(){return L6480_getparam(0x04,3);}
long L6480_getparam_acc(){return L6480_getparam(0x05,2);}
long L6480_getparam_dec(){return L6480_getparam(0x06,2);}
long L6480_getparam_maxspeed(){return L6480_getparam(0x07,2);}
long L6480_getparam_minspeed(){return L6480_getparam(0x08,2);}
long L6480_getparam_fsspd(){return L6480_getparam(0x15,2);}
long L6480_getparam_kvalhold(){return L6480_getparam(0x09,1);}
long L6480_getparam_kvalrun(){return L6480_getparam(0x0a,1);}
long L6480_getparam_kvalacc(){return L6480_getparam(0x0b,1);}
long L6480_getparam_kvaldec(){return L6480_getparam(0x0c,1);}
long L6480_getparam_intspd(){return L6480_getparam(0x0d,2);}
long L6480_getparam_stslp(){return L6480_getparam(0x0e,1);}
long L6480_getparam_fnslpacc(){return L6480_getparam(0x0f,1);}
long L6480_getparam_fnslpdec(){return L6480_getparam(0x10,1);}
long L6480_getparam_ktherm(){return L6480_getparam(0x11,1);}
long L6480_getparam_adcout(){return L6480_getparam(0x12,1);}
long L6480_getparam_ocdth(){return L6480_getparam(0x13,1);}
long L6480_getparam_stallth(){return L6480_getparam(0x14,1);}
long L6480_getparam_stepmood(){return L6480_getparam(0x16,1);}
long L6480_getparam_alareen(){return L6480_getparam(0x17,1);}
long L6480_getparam_gatecfg1(){return L6480_getparam(0x18,2);}
long L6480_getparam_gatecfg2(){return L6480_getparam(0x19,1);}
long L6480_getparam_config(){return L6480_getparam(0x1a,2);}
long L6480_getparam_status(){return L6480_getparam(0x1b,2);}


void L6480_run(int dia,long spd){
  if(dia==1)
    L6480_transfer(0x51,3,spd);
  else
    L6480_transfer(0x50,3,spd);
}
void L6480_stepclock(int dia){
  if(dia==1)
    L6480_transfer(0x59,0,0);    
  else
    L6480_transfer(0x58,0,0);
}
void L6480_move(int dia,long n_step){
  if(dia==1)
    L6480_transfer(0x41,3,n_step);
  else
    L6480_transfer(0x40,3,n_step);
}
void L6480_goto(long pos){
  L6480_transfer(0x60,3,pos);
}
void L6480_gotodia(int dia,int pos){
  if(dia==1)    
    L6480_transfer(0x69,3,pos);
  else    
    L6480_transfer(0x68,3,pos);
}
void L6480_gountil(int act,int dia,long spd){
  if(act==1)
    if(dia==1)
      L6480_transfer(0x8b,3,spd);
    else
      L6480_transfer(0x8a,3,spd);
  else
    if(dia==1)
      L6480_transfer(0x83,3,spd);
    else
      L6480_transfer(0x82,3,spd);
}  
void L6480_relesesw(int act,int dia){
  if(act==1)
    if(dia==1)
      L6480_transfer(0x9b,0,0);
    else
      L6480_transfer(0x9a,0,0);
  else
    if(dia==1)
      L6480_transfer(0x93,0,0);
    else
      L6480_transfer(0x92,0,0);
}
void L6480_gohome(){
  L6480_transfer(0x70,0,0);
}
void L6480_gomark(){
  L6480_transfer(0x78,0,0);
}
void L6480_resetpos(){
  L6480_transfer(0xd8,0,0);
}
void L6480_resetdevice(){
  L6480_send_u(0x00);//nop命令
  L6480_send_u(0x00);
  L6480_send_u(0x00);
  L6480_send_u(0x00);
  L6480_send_u(0x00);
  L6480_hardhiz();
  L6480_send_u(0xc0);
}
void L6480_softstop(){
  L6480_transfer(0xb0,0,0);
}
void L6480_hardstop(){
  L6480_transfer(0xb8,0,0);
}
void L6480_softhiz(){
  L6480_transfer(0xa0,0,0);
}
void L6480_hardhiz(){
  L6480_transfer(0xa8,0,0);
}
long L6480_getstatus(){
  long val=0;
  L6480_send_u(0xd0);
  for(int i=0;i<=1;i++){
    val = val << 8;
    digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
    val = val | SPI.transfer(0x00); // アドレスもしくはデータ送信。
    digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル 
  }
  return val;
}

void L6480_transfer(int add,int bytes,long val){
  int data[3];
  L6480_send(add);
  for(int i=0;i<=bytes-1;i++){
    data[i] = val & 0xff;  
    val = val >> 8;
  }
  if(bytes==3){
    L6480_send(data[2]);
  }
  if(bytes>=2){
    L6480_send(data[1]);
  }
  if(bytes>=1){
    L6480_send(data[0]);
  }  
}
void L6480_send(unsigned char add_or_val){
  while(!digitalRead(PIN_BUSY)){
    //digitalWrite(7,0);
  } //BESYが解除されるまで待機
  //digitalWrite(7,1);
  digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
  SPI.transfer(add_or_val); // アドレスもしくはデータ送信。
  digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル。
}
void L6480_send_u(unsigned char add_or_val){//busyを確認せず送信するため用
  digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
  SPI.transfer(add_or_val); // アドレスもしくはデータ送信。
  digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル。
}
void L6480_busydelay(long time){//BESYが解除されるまで待機
  while(!digitalRead(PIN_BUSY)){
  }
  delay(time);
}
long L6480_getparam(int add,int bytes){
  long val=0;
  int send_add = add | 0x20;
  L6480_send_u(send_add);
  for(int i=0;i<=bytes-1;i++){
    val = val << 8;
    digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
    val = val | SPI.transfer(0x00); // アドレスもしくはデータ送信。
    digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル 
  }
  return val;
}
