#include <ModmataServo.h>

Servo *servos[14] = {nullptr};
int servo_count = 0;

/**
Attach a servo to a digital/PWM pin.
@param pin The pin number that the servo is attached to.
@return 1 on success, 0 on error
*/
int servoAttach(uint8_t pin)
{
  if (pin >= 0 && pin <= 13 && servo_count < 12)
  {
    Servo curr_servo;
    curr_servo.attach(pin);
    servos[pin] = &curr_servo;
    servo_count++;
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
Detach a servo from a pin
@param pin The pin number that the servo is being detached from
@return 1 on success, 0 on error
*/
int servoDetach(uint8_t pin)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    servos[pin]->detach();
    return 1;
  }
  else
  {
    return 0;
  }
}


/**
Write an angle value to a servo
@param pin The pin number that the servo is attached to
@param angle The angle value the user wishes to write
@return 1 on success, 0 on error
*/
int servoWrite(uint8_t pin, int angle)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    servos[pin].write(angle);
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
Read an angle value from a servo (the last value written to the servo)
@param pin The pin number that the servo is attached to
@return The previously written value on success, -1 on error
*/
int servoRead(uint8_t pin)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    return servos[pin].read();
  }
  else
  {
    return -1;
  }
}
