task main()
{
  //Create "deadzone" variables. Adjust threshold value to increase/decrease deadzone
  int X2 = 0, Y1 = 0, X1 = 0, threshold = 15;

  //Loop Forever
  while(1 == 1)
  {
    //Create "deadzone" for Y1/Ch3
    if(abs(vexRT[Ch3]) > threshold)
      Y1 = vexRT[Ch3];
    else
      Y1 = 0;
    //Create "deadzone" for X1/Ch4
    if(abs(vexRT[Ch4]) > threshold)
      X1 = vexRT[Ch4];
    else
      X1 = 0;
    //Create "deadzone" for X2/Ch1
    if(abs(vexRT[Ch1]) > threshold)
      X2 = vexRT[Ch1];
    else
      X2 = 0;

    //Remote Control Commands
    motor[frontRight] = Y1 - X2 - X1;
    motor[backRight] =  Y1 - X2 + X1;
    motor[frontLeft] = Y1 + X2 + X1;
    motor[backLeft] =  Y1 + X2 - X1;
  }
}