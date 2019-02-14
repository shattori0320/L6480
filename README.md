# L6480

## 参考
- http://spinelify.blog.fc2.com/blog-entry-66.html
- http://tyokota.hateblo.jp/entry/2017/06/16/191852

## 使ったもの
- 電源：　GSV3000 12V
- モータ：ST-57BYG076-3004D
- マイコン:Arduino

以下レジスタの説明など（工事中）

## H01 ABS_POS
モーターの絶対位置を保持している
## H02 EL_POS
マイクロステップの位置を保持している
## H03 MARK
マークしている絶対位置
## H04 SPEED
現在の回転速度
## H05 ACC
加速度 12bit
- 0x50 tick = 250 ns
- 1164 step/s^2
==tickの値はクロックに依らないのか==
## H06 DEC
減速度 12bit
- 0x50 tick = 250 ns
- 1164 step/s^2

## H07 MAX_SPEED
最大速度 
- 無負荷なら0x25で回った

## H08 MIN_SPEED
最小速度
## H09 KVAL_HOLD
モータ停止中の出力電圧　K~val~
- 0x20
- V~s~ * 32/256 = 1.5 when V~s~ is 12V
- ==このライブラリを書いた人は24Vで使ってそう==
    - モーターの定格は3Vなので
- 0x40にした
## H0A KVAL_RUN
モータ定速回転中の電圧
- 0x20
- V~s~ * 32/256 = 1.5 when V~s~ is 12V
- 0x64にした
## H0B KVAL_ACC
加速中の電圧
- 0x20
- V~s~ * 32/256 = 1.5 when V~s~ is 12V
## H0C KVAL_DEC
減速中の電圧
- 0x20 
- V~s~ * 32/256 = 1.5 when V~s~ is 12V
## H0D INT_SPEED
補間を開始する回転数
## H0E ST_SLP
補間の傾き
## H0F FN_SLP_ACC
加速最終時の傾き
## H10 FN_SLP_DEC
減速最終時の傾き
## H11 K_THERM
高温時の補正
## H12 ADC_OUT
ADCの出力
## H13 OCD_TH
オーバーカレントの閾値
- 0x15 687.5mV
- 9.8A if 2R~DSon~is 0.07 
## H14 STALL_TH
ストール電流の閾値
- 0x02 93.25mV
- 1.33A if 2R~DSon~is 0.07
- 0x0cに変えた
## H15 FS_SPD
フルステップ駆動に切り替える回転数
## H16 STEP_MODE
ステップモード
## H17 ALARM_EN
アラームon/off

## H18 GATECFG1
ゲートドライバ設定１

### IGATE
010B 8mA sink/source current
FETの充放電電流

### TCC
10111B 300ns the duration of constant current phase during gate turn-on and turn-off sequences
ゲートの充電時間の設定

### TBOOST
0B 0ns duration of the overboost phase
放電をちょっと早くするためのブースト時間の設定
### WD_EN
0B disable clock source monitoring

## H19 GATECFG2
ゲートドライバ設定２

### TDT 
100B 500ns deadtime duration
デッドタイムを設定する

### TBLANK 
00111B 875ns the duration of the blanking of the current sensing comparators
電流検出の無効時間を設定する

## H1A CONFIG
システム設定レジスタ

### OSC_SEL
0001B 16MHz from internal oscilator

### SW_MODE
0B HardStop Interrupt

### OC_SD
1B overcurrent event bridge shutdown

### VCCVAL
1B 15V regulator output voltage
==電源電圧以上が設定されている==

### UVLO
0B under voltage lock out
- V~CCthOn~:6.9V
- V~CCthOff~:6.3V
- V~BOOTthOn~:6V
- V~BOOTthOff~:5.5V

### EN_VSCOMP
0B Motor supply voltage compensation disabled

### PWM frequency
- F_PWM_INT: 0B division factor 1
- F_PWM_DEC: 111B Multiplication factor 2
- frequency is 62.5kHz

### H1B STATUS
現在のステータス
