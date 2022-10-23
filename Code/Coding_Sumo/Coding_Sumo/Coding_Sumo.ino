#include <PS4Controller.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>

#define BRAKE   0
#define CW      1
#define CCW     2
#define DEC_1   50
#define DEC_2   50
#define DEF_1   100 
#define DEF_2   100
#define INC_1   150
#define INC_2   150
#define MAX_1   200
#define MAX_2   200
#define MAX     255
#define MOTOR_1 0
#define MOTOR_2 1

#define RPWM_1  27
#define LPWM_1  14
#define R_EN_1  26
#define L_EN_1  12
#define RPWM_2  16
#define LPWM_2  17
#define R_EN_2  4
#define L_EN_2  5
short usSpeed_1;
short usSpeed_2;
unsigned short usMotor_Status_1;
unsigned short usMotor_Status_2;

const int pwmchan_1 = 0;
const int pwmchan_2 = 1;
const int pwmchan_3 = 2;
const int pwmchan_4 = 3;
const int freq = 25000;
const int resolution = 8;

void getESPBtMacAddress()
{
  const uint8_t* address = esp_bt_dev_get_address();
  char str[100];
  sprintf(str, "ESP32's Bluetooth MAC address is - %02x:%02x:%02x:%02x:%02x:%02x", address[0],address[1],address[2],address[3],address[4],address[5]);
  Serial.println(str);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) {
  if(motor == MOTOR_1){
    if(direct == CW){
      digitalWrite(L_EN_1, HIGH);
      digitalWrite(R_EN_1, HIGH);
      ledcWrite (pwmchan_2, pwm);
      ledcWrite (pwmchan_1, 0);
    }
    else if(direct == CCW){
      digitalWrite(L_EN_1, HIGH);
      digitalWrite(R_EN_1, HIGH);
      ledcWrite (pwmchan_2, 0);
      ledcWrite (pwmchan_1, pwm);    
      }
    else{
      digitalWrite(L_EN_1, LOW);
      digitalWrite(R_EN_1, LOW);
      ledcWrite (pwmchan_2, 0);
      ledcWrite (pwmchan_1, 0);           
    } 
  }
  else if(motor == MOTOR_2){
    if(direct == CW){
      digitalWrite(L_EN_2, HIGH);
      digitalWrite(R_EN_2, HIGH);
      ledcWrite (pwmchan_4, pwm);
      ledcWrite (pwmchan_3, 0);
    }
    else if(direct == CCW){
      digitalWrite(L_EN_2, HIGH);
      digitalWrite(R_EN_2, HIGH); 
      ledcWrite (pwmchan_4, 0);
      ledcWrite (pwmchan_3, pwm);
    }
    else{
      digitalWrite(L_EN_2, LOW);
      digitalWrite(R_EN_2, LOW);
      ledcWrite (pwmchan_4, 0);
      ledcWrite (pwmchan_3, 0);         
    }
  }
}

void setPWM(){
  usSpeed_1 = DEF_1;
  usSpeed_2 = DEF_2;
  if(PS4.Down()){
    Serial.println ("Pengurangan Kecepatan");
    usSpeed_1 = DEC_1;
    usSpeed_2 = DEC_2;
  }
  if(PS4.Up()){
    Serial.println("Penambahan Kecepatan");
    usSpeed_1 = INC_1;
    usSpeed_2 = INC_2;
  }
  if(PS4.R1()){
    Serial.println("Top Speed");
    usSpeed_1 = MAX_1;
    usSpeed_2 = MAX_2;
    if(PS4.R2()){
      Serial.println("Maximum Speed");
      usSpeed_1 = MAX;
      usSpeed_2 = MAX;
    }
  }
}

void Stop() {
  Serial.println("Stop");
  usMotor_Status_1 = BRAKE;
  usMotor_Status_2 = BRAKE;
  motorGo(MOTOR_1, usMotor_Status_1, 0);
  motorGo(MOTOR_2, usMotor_Status_2, 0);
}

void Forward() {
  Serial.println("Forward");
  usMotor_Status_1 = CCW;
  usMotor_Status_2 = CW;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Reverse() {
  Serial.println("Reverse");
  usMotor_Status_1 = CW;
  usMotor_Status_2 = CCW;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Left(){
  Serial.println("Left");
  usMotor_Status_1 = CCW;
  usMotor_Status_2 = CW;
  motorGo(MOTOR_1, usMotor_Status_1, BRAKE);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Right(){
  Serial.println("Right");
  usMotor_Status_1 = CCW; //berlawan arah
  usMotor_Status_2 = CW; //searah jarum jam;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, BRAKE);
}

void reverseLeft(){
  Serial.println("Reverse Left");
  usMotor_Status_1 = CW;
  usMotor_Status_2 = CCW;
  motorGo(MOTOR_1, usMotor_Status_1, BRAKE);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void reverseRight(){
  Serial.println("Reverse Right");
  usMotor_Status_1 = CW; //berlawan arah
  usMotor_Status_2 = CCW; //searah jarum jam;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, BRAKE);
}

void rotateLeft(){
  Serial.println("Rotate Left");
  usMotor_Status_1 = CW; //berlawan arah
  usMotor_Status_2 = CW; //searah jarum jam
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void rotateRight(){
  Serial.println("Rotate Right");
  usMotor_Status_1 = CCW; //berlawan arah
  usMotor_Status_2 = CCW; //searah jarum jam
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

//void Standing(){
//  if(ps2x.ButtonReleased(PSB_L3)) Stop();
//  usSpeed_1 = MAX;
//  usSpeed_2 = MAX;
//  Reverse();
//  delay(1000);
//  if(ps2x.ButtonPressed(PSB_L3))  Forward();
//}

//void Racing(){
//  if(ps2x.ButtonReleased(PSB_R3))   Stop();
//  usSpeed_1 = MAX;
//  usSpeed_2 = MAX;
//  Forward();
//  delay(1500);
//  Reverse();
//  delay(4000);
//}

void increaseSpeed(){
  usSpeed_1 = usSpeed_1 + 10;
  usSpeed_2 = usSpeed_1 + 10;
  if(usSpeed_1 > 255 || usSpeed_2 > 255){
    usSpeed_1 = 255; 
    usSpeed_2 = 255; 
  }
  Serial.print("Speed -: ");
  Serial.print(usSpeed_1);
  Serial.print(" ");
  Serial.println(usSpeed_2);
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

void decreaseSpeed(){
  usSpeed_1 = usSpeed_1 - 10;
  usSpeed_2 = usSpeed_2 - 10;
  if(usSpeed_1 < 0 || usSpeed_2 < 0){
    usSpeed_1 = 0; 
    usSpeed_2 = 0; 
  }
  Serial.print("Speed -: ");
  Serial.print(usSpeed_1);
  Serial.print(" ");
  Serial.println(usSpeed_2);
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

void onConnect()
{
  Serial.println("Connected!.");
}
  
void onDisConnect()
{
  Serial.println("Disconnected!.");    
}
  
void setup() {
  Serial.begin(115200);
  // PS4.begin("1a:2b:3c:01:01:01");
  /* Cek ESP Bluetooth Mac Address*/
  // getESPBtMacAddress();
  
  /* Init PS4 Connection */
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Initialization ready!");

  pinMode(RPWM_1, OUTPUT);
  pinMode(LPWM_1, OUTPUT);
  pinMode(R_EN_1, OUTPUT);
  pinMode(L_EN_1, OUTPUT);
  pinMode(RPWM_2, OUTPUT);
  pinMode(LPWM_2, OUTPUT);
  pinMode(R_EN_2, OUTPUT);
  pinMode(L_EN_2, OUTPUT);
  digitalWrite(RPWM_1, LOW);
  digitalWrite(LPWM_1, LOW);
  digitalWrite(RPWM_2, LOW);
  digitalWrite(LPWM_2, LOW);
  digitalWrite(R_EN_1, HIGH);
  digitalWrite(L_EN_1, HIGH);
  digitalWrite(R_EN_2, HIGH);
  digitalWrite(L_EN_2, HIGH);

  ledcSetup(pwmchan_1,freq,resolution);
  ledcSetup(pwmchan_2,freq,resolution);
  ledcSetup(pwmchan_3,freq,resolution);
  ledcSetup(pwmchan_4,freq,resolution);

  ledcAttachPin(RPWM_1,pwmchan_1);
  ledcAttachPin(LPWM_1,pwmchan_2);  
  ledcAttachPin(RPWM_2,pwmchan_3);
  ledcAttachPin(LPWM_2,pwmchan_4);
}

void loop() {
//  bool setCpuFrequencyMhz(160);
  if(PS4.isConnected()){
      Serial.println("Connected");
      if ( PS4.Cross() ){
          setPWM();
          if (PS4.Left()) { Left(); }
          else if (PS4.Right()) { Right(); }
          else { Forward(); }
          delay (100);
      }
          
      else if ( PS4.L1() ){
          setPWM();
          if (PS4.Left()) { reverseLeft(); }
          else if (PS4.Right()) { reverseRight(); }
          else { Reverse(); }
          delay (100);
      }
       
      else if ( PS4.Left() ){ 
          setPWM();
          rotateLeft();
          delay (100);
      }
          
      else if ( PS4.Right() ){
          setPWM();
          rotateRight();
          delay (100); 
      }

      else if (PS4.Circle()){
          increaseSpeed();
          if (PS4.Left()) { Left(); }
          else if (PS4.Right()) { Right(); }
          else { Forward(); }
          delay (100);
      }

      else if (PS4.Triangle()){
          decreaseSpeed();
          if (PS4.Left()) { reverseLeft(); }
          else if (PS4.Right()) { reverseRight(); }
          else { Reverse(); }
          delay (100);
      }

      else if ( PS4.Square() ){
          if (PS4.Left()) { setPWM(); reverseLeft(); }
          else if (PS4.Right()) { setPWM(); reverseRight(); }
          else { setPWM(); Reverse(); }
          delay (100);
      }

     else {
      Stop();
     }
   } else {
     Serial.println("Not Connected");
   }
}
