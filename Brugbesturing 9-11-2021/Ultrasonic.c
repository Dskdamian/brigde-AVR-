
#define trigger RC2
#define echo    RC3

void main()
{
  //  Configure The IO Pins
  TRISB1 = 0;
  TRISB2 = 1;
  ...
  while(1)
  {
    ...
  }
}
// -- [[ Configure Timer1 To Operate In Timer Mode  ]] --

// Clear The Timer1 Register. To start counting from 0
TMR1 = 0;
// Choose the local clock source (timer mode)
TMR1CS = 0;
// Choose the desired prescaler ratio (1:1)
T1CKPS0 = 0;
T1CKPS1 = 0;

int calc_distance(void);

// Definition Of The calc_dist() Function
int calc_dist(void)
{
  int distance=0;
  TMR1=0;
  // Send Trigger Pulse To The Sensor
  trigger=1;
  __delay_us(10);
  trigger=0;
  // Wait For The Echo Pulse From The Sensor
  while(!echo);
  // Turn ON Timer Module
  TMR1ON=1;
  // Wait Until The Pulse Ends
  while(echo);
  // Turn OFF The Timer
  TMR1ON=0;
  // Calculate The Distance Using The Equation
  distance=TMR1/58.82;
  return distance;
}
