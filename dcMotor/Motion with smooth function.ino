const int ENA = 6;
const int IN1 = 5;
const int IN2 = 4;


const int ENB = 11;
const int IN3 = 10;
const int IN4 = 9;


void setup() 
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN4, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  
}

//This corresponds to 3 times the mechanical time constant of the motor-robot system in ms.
double winduptime = 300;

void loop() 
{
	
 // Move forward for one second at 100% speed
Smooth_Forward(100, 1000);


// Brake for one second
Motor1_Brake();
Motor2_Brake();
delay(1000);

 // Move backward for one second at 100% speed
Smooth_Backward(100, 1000);

Motor1_Brake();
Motor2_Brake();
delay(1000);

}
void Motor1_Backward(int Speed)
{
 digitalWrite(IN1,HIGH);
 digitalWrite(IN2,LOW);
 analogWrite(ENA,Speed);
}

void Motor1_Forward(int Speed)
{
 digitalWrite(IN1,LOW);
 digitalWrite(IN2,HIGH);
 analogWrite(ENA,Speed);
}
void Motor1_Brake()
{
 digitalWrite(IN1,LOW);
 digitalWrite(IN2,LOW);
}
void Motor2_Backward(int Speed)
{
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,HIGH);
 analogWrite(ENB,Speed);
}

void Motor2_Forward(int Speed)
{
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);
 analogWrite(ENB,Speed);
}
void Motor2_Brake()
{
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,LOW);
}


// This function is meant to slowly ramp up the motor, to negate sharp input currents. It is not designed (yet) to mitigate wheel slippage
void Smooth_Forward(int speed, int duration)
{
  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;
  
  while (i < duration)
  {
    // Smooth ramping up during wind-up transient
    if(i <= winduptime)
    {
      y = SmoothFunction(i/winduptime);
    }
    
    // Smooth ramping down during wind-down transient
    else if (i >= duration - winduptime)
    {
      y = SmoothFunction(-(i - duration)/winduptime);
    }
    // Constant value during steady state
    else
    {
      y = 1;
    }
    
    // Normalize y to between 0 and 255
    y = round(y*255*speed/100);
    
    z = (int) y;
    Motor1_Forward(z);
    Motor2_Forward(z);
    i += ts;
    delay(ts);
  }

}

// Perhaps integrate this and Smooth_Forward into one function to save on space
void Smooth_Backward(int speed, int duration)
{
  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;
  
  while (i < duration)
  {
    // Smooth ramping up during wind-up transient
    if(i <= winduptime)
    {
      y = SmoothFunction(i/winduptime);
    }
    
    // Smooth ramping down during wind-down transient
    else if (i >= duration - winduptime)
    {
      y = SmoothFunction(-(i - duration)/winduptime);
    }
    // Constant value during steady state
    else
    {
      y = 1;
    }
    
    // Normalize y to between 0 and 255
    y = round(y*255*speed/100);
    
    z = (int) y;
    Motor1_Backward(z);
    Motor2_Backward(z);
    i += ts;
    delay(ts);
  }

}

// Is this declaration necessary? Who knows
double SmoothFunction(double);

// SmoothFunction is normalized to go from 0 to 1 between x=0 to x=1
double SmoothFunction(double x)
{
    
  double output;
  
  // Definition of sigmoid curve (logistic type)	
  output = 1/(1+exp(-(10*x - 5)));
  
  return output;
}
