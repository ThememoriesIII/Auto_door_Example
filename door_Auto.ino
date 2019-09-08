#include <HCSR04.h>

#define tpin 9 //ขาทริก
#define epin 10 //ขาแอคโค้
#define Sdoor_close 5 //door pin ขาสวืทฝั่งปิด
#define Sdoor_open 6 //door pin ขาสวิทฝั่งเปิด
#define motorA 4 //ขามอเตอร์ฝั่งบวก
#define motorB 3 //ขามอเตอร์ฝั่งลบ

#define Max_dis 100 //ค่าระยะสูงสุดที่คนเดินเข้ามา
#define Low_dis 10 //ค่าระยะต่ำสุดที่คนเดืนเข้า
#define dooropen 0 //door state สถานะการเปิดประตู
#define doorclose 1 //door state
#define time_check_door 200 //เวลาเช็คสถานะประตู เช็คทุก 200 มิลลิวิ
#define time_check_SW 50 //เวลาเช็คสถานะประตู เช็คทุก 50 มิลลิวิ
#define time_delay_door 3000 //เวลาเช็คสถานะประตู เช็คทุก 3 วิ

HCSR04 Sensor_door(tpin,epin);

float messur_dist = 0; //เก็บระยะทาง
unsigned long time1 = 0; //เก็บระยะเวลาที่ฟังชั่นทำงาน
unsigned long time2 = 0; //เก็บระยะเวลาที่ฟังชั่นทำงาน
unsigned long doorTime = 0; //เก็บระยะเวลาประตูเปิด
bool door = 0;

void setup() {
  pinMode(Sdoor_close,INPUT);
  pinMode(Sdoor_open,INPUT);
  pinMode(motorA,OUTPUT);
  pinMode(motorB,OUTPUT);
}

void loop() {
  if(millis()-time2 >= time_check_SW)
  {
     sw_door();
     time2 = millis();
  }
  if(millis()-time1 >= time_check_door)
  {
     door_control();
     time1 = millis();
  }
}

void door_control()
{
     
    messur_dist = Sensor_door.dist();
    //ถ้าคนอยู่ในระยะ 10 ถัง 100เมตร กับประตูปิด ให้เปิดประตู
    if((messur_dist>Low_dis&&messur_dist<Max_dis)&&door==doorclose)
    {
      openAdoor();
      doorTime = millis();
    }
    else
    {
      //ถ้าประตูเปิดอยู่แล้วเปิดเกิน 3 วินาทีสั่งปิด
      if(door==dooropen&&millis()-doorTime >= time_delay_door)
        closeAdoor();
      else
        //ถ้าประตูปิดแล้ว และ คนไม่อยู่ในระยะให้หยุดมอเตอร์
        stopAdoor();
    }
}

void sw_door()
{
  /*ถ้าสวิทฝั่งปิดเป็น 1 กับฝั่งเปิด เป็น 0 ถือว่า ประตูปิด
    ถ้าสวิทฝั่งเปิดเป็น 1 กับฝั่งปิด เป็น 0 ถือว่า เปิด
    ถ้าสวิทฝั่งเปิดเป็น 0 กับฝั่งปิด เป็น 0 ถือว่า เปิด
  */
  door = (digitalRead(Sdoor_close)==HIGH&&digitalRead(Sdoor_open)==LOW)?doorclose:dooropen;
}

void openAdoor()
{
  digitalWrite(motorA,HIGH);
  digitalWrite(motorB,LOW);
}
void closeAdoor()
{
  digitalWrite(motorA,LOW);
  digitalWrite(motorB,HIGH);
}
void stopAdoor()
{
  digitalWrite(motorA,LOW);
  digitalWrite(motorB,LOW);
}
