const int IN1 = 5;
const int IN2 = 4;
const int ENA = 6;

const int IN3 = 10;
const int IN4 = 9;
const int ENB = 11;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN4, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
 // Move forward for one second at 50% speed
Smooth_Forward(50, 1000);
Motor1_Brake();
Motor2_Brake();
delay(100);

//Motor1_Forward(150);
//Motor2_Forward(150);
//delay(1000);
//Motor1_Brake();
//Motor2_Brake();
//delay(100);
//Motor1_Backward(200);
//Motor2_Backward(200);
//delay(1000);
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
	// Leave this at 100 (ms) for now
	double winduptime = 100;
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
			y = Smoothfunction(-(i - duration)/winduptime);
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

// SmoothFunction is normalized to go from 0 to 1 between x=0 to x=1
double SmoothFunction(double x)
{
    
    double output;
    
	output = 1/(1+exp(-(10*x - 5)));
	
	return output;
}
