#define left2_light 2
#define left1_light 3
#define right1_light 4
#define right2_light 5
#define MotorR_I1      2 //定義 A1 接腳（右）
#define MotorR_I2      3 //定義 A2 接腳（右）
#define MotorR_PWMR    44//定義 ENA (PWM調速) 接腳
#define MotorL_I3      5 //定義 B1 接腳（左）
#define MotorL_I4      6 //定義 B2 接腳（左）
#define MotorL_PWML    45//定義 ENB (PWM調速) 接腳

void setup() {
  // put your setup code here, to run once:
  pinMode(left2_light, INPUT);
  pinMode(left1_light, INPUT);
  pinMode(right1_light, INPUT);
  pinMode(right2_light, INPUT);
  Serial.begin(9600);
}

int _l2, _l1, _r1, _r2;
float error = 0;
float _w2 = 0.8;
float _w1 = 0.4;
int amp = 50;
int numofLight = 0;
int vL = 0, vR = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //有光是0，沒光是1
  _l2 = 1 - digitalRead(left2_light);
  _l1 = 1 - digitalRead(left1_light);
  _r1 = 1 - digitalRead(right1_light);
  _r2 = 1 - digitalRead(right2_light);
  //transfer to 1 when there is light and 0 if there is no light
  numofLight = _l2 + _l1 + _r1 + _r2;
  error = _w2*_l2 + _w1*_l1 - _w1*_r1 - _w2*_r2;
  if(numofLight == 0){
    vL = 0; vR = 0;
  }else{
    error = error * 50 / numofLight;
    vL = 210 - error;   vR = 210 + error;
    if(vL > 255){
      vL = 255;
    }
    if(vR > 255){
      vR = 255;
    }
  }
  Serial.println(numofLight);
  Serial.print("vL=");
  Serial.print(vL);
  Serial.print("\tvR=");
  Serial.println(vR);
  delay(200);
}

void MotorWriting(double vL, double vR) {
  // TODO: use TB6612 to control motor voltage & direction
  int out_vL, out_vR;
  if(vL<0){
    out_vL = -vL;
    digitalWrite(MotorL_I3, LOW);
    digitalWrite(MotorL_I4, HIGH);
  }else{
    out_vL = vL;
    digitalWrite(MotorL_I3, HIGH);
    digitalWrite(MotorL_I4, LOW);
  }

  if(vR<0){
    out_vR = -vR;
    digitalWrite(MotorR_I1, LOW);
    digitalWrite(MotorR_I2, HIGH);
  }else{
    out_vR = vR;
    digitalWrite(MotorR_I1, HIGH);
    digitalWrite(MotorR_I2, LOW);
  }
  analogWrite(MotorR_PWMR, out_vR);
  analogWrite(MotorL_PWML, out_vL);
  delay(5);
}// MotorWriting