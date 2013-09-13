#include <ball.h>
#include <paddle.h>

Ball ball;
Paddle paddle;

#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int converted_ax = 0;
int converted_ay = 0;

#define OUTPUT_READABLE_ACCELGYRO

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int clearPin = 5;
int redByte = 0x01; int greenByte = 0x01; int blueByte = 0x01; int colByte = 0x01;

void setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
}

void shiftVal(int redByte, int greenByte, int blueByte, int colByte)
{
  digitalWrite(latchPin, LOW);
  digitalWrite(clearPin, LOW);
  digitalWrite(clearPin, HIGH);
  //shiftOut(dataPin, clockPin, bitOrder, value);
  shiftOut(dataPin, clockPin, MSBFIRST, colByte);
  shiftOut(dataPin, clockPin, MSBFIRST, (~blueByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~greenByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~redByte)&0xFF);
  digitalWrite(latchPin, HIGH);
}

enum ballStates{ballInit, ballUpLeft, ballUpRight, ballDownLeft, ballDownRight} ballState;

void ballController()
{
  switch(ballState)
  {
    case ballInit:
    {
      ballState = ballUpRight;
      break;
    }
    case ballUpLeft:
    {
      if( (ball.getRow() == 0x01) && (ball.getCol() == 0x01) )
      {
        ballState = ballDownRight;
      }
      else if(ball.getRow() == 0x01)
      {
        ballState = ballDownLeft;
      }
      else if(ball.getCol() == 0x01)
      {
        ballState = ballUpRight;
      }
      break;
    }
    case ballUpRight:
    {
      if( (ball.getRow() == 0x01) && (ball.getCol() == 0x80) )
      {
        ballState = ballDownLeft;
      }
      else if(ball.getRow() == 0x01)
      {
        ballState = ballDownRight;
      }
      else if(ball.getCol() == 0x80)
      {
        ballState = ballUpLeft;
      }
      break;
    }
    case ballDownLeft:
    {
      if( (ball.getRow() == 0x80) && (ball.getCol() == 0x01) )
      {
        ballState = ballUpRight;
      }
      else if(ball.getRow() == 0x80)
      {
        ballState = ballUpLeft;
      }
      else if(ball.getCol() == 0x01)
      {
        ballState = ballDownRight;
      }
      break;
    }
    case ballDownRight:
    {
      if( (ball.getRow() == 0x80) && (ball.getCol() == 0x80) )
      {
        ballState = ballUpLeft;
      }
      else if(ball.getRow() == 0x80)
      {
        ballState = ballUpRight;
      }
      else if(ball.getCol() == 0x80)
      {
        ballState = ballDownLeft;
      }
      break;
    }
    default:
    {
      ballState = ballInit;
      break;
    }
  }
  
  switch(ballState)
  {
    case ballInit:
    {
      break;
    }
    case ballUpLeft:
    {
      ball.shiftRowUp();
      ball.shiftColLeft();
      break;
    }
    case ballUpRight:
    {
      ball.shiftRowUp();
      ball.shiftColRight();
      break;
    }
    case ballDownLeft:
    {
      ball.shiftRowDown();
      ball.shiftColLeft();
      break;
    }
    case ballDownRight:
    {
      ball.shiftRowDown();
      ball.shiftColRight();
      break;
    }
    default:
    {
      break;
    }
  }
}

enum paddleStates{paddleInit, paddleStay, paddleLeft, paddleRight} paddleState;

void paddleController()
{
  switch(paddleState)
  {
    case paddleInit:
    {
      paddleState = paddleLeft;
      break;
    }
    case paddleStay:
    {
      break;
    }
    case paddleLeft:
    {
      if(paddle.getLeftPaddle() == 0x01)
      {
        paddleState = paddleRight;
      }
      break;
    }
    case paddleRight:
    {
      if(paddle.getRightPaddle() == 0x80)
      {
        paddleState = paddleLeft;
      }
      break;
    }
    default:
    {
      paddleState = paddleInit;
      break;
    }
  }
  
  switch(paddleState)
  {
    case paddleInit:
    {
      break;
    }
    case paddleStay:
    {
      break;
    }
    case paddleLeft:
    {
      paddle.shiftPaddleLeft();
      break;
    }
    case paddleRight:
    {
      paddle.shiftPaddleRight();
      break;
    }
    default:
    {
      break;
    }
  }
}

void gameController()
{
  
}

void loop()
{
  ballController();
  paddleController();
  
  shiftVal(0, paddle.getPaddleRow(), 0, paddle.getLeftPaddle());
  shiftVal(0, paddle.getPaddleRow(), 0, paddle.getMidPaddle());
  shiftVal(0, paddle.getPaddleRow(), 0, paddle.getRightPaddle());
  shiftVal(0, 0, ball.getRow(), ball.getCol());
  delay(200);
}
