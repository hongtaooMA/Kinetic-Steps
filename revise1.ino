
#include <Stepper.h>
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9);


//超声波测距模块 Trig接Arduino板的Digital 5口，触发测距；Echo接Digital 4口，接收距离信号。
int inputPin=4; // 定义超声波信号接收接口
int outputPin=5; // 定义超声波信号发出接口

char ss;//暂存字符，用来判断是否有效
char ch = 'a';//最后获得的字符
int num = 1;//字符转化数字，便于操作
int previous = 0;//上一步的角度
int now;//新一个循环时的角度
int angle[19] = {-1280, -1120, -960, -800, -640, -480, -320, -160, 0, 0, 0, 160, 320, 480, 640, 800, 960, 1120, 1280};//角度列表


void setup()
{
myStepper.setSpeed(100);// set the speed at 20 rpm:
Serial.begin(9600);
pinMode(inputPin, INPUT);
pinMode(outputPin, OUTPUT);
}


void loop()
{
// 超声波模块获取距离数据  
digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
int distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
distance= abs(distance/58); // 将脉冲时间转化为距离（单位：厘米）


// 接收上一个板传来的数据并读取最后一个有效值赋值给ch
while (Serial.available() > 0){
  ss=Serial.read();
  if ((ss > '0')&&(ss <= '3')){
  ch = ss;
  }
  delay(2);
}

  
//如果当前循环中此块板的传感器被触发，那么用此板的标记取代上一个板传来的数据，赋值给ch
if (distance < 20)
ch = 'a';


//将新ch向下传递
Serial.println(ch);


//字符ch与数字num的转化
num = ch - 'a' + 1;

//通过获得的数字，调用数组中的角度值，并通过（now-previous）获得stepper指令
if ((num > 0)&&(num < 11)){
now = angle[num + 8];
if (now != previous ){
  myStepper.step(previous - now);
  previous = now;
}
}

delay(100);
}
