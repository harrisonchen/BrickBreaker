#include <ball.h>
#include <paddle.h>
#include <block.h>
#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif

Ball ball;
Paddle paddle;
Block blocks;
int greenRowRegister[8];
int blueRowRegister[8];
int colRegister[8];
int ballTimer = 0;
int paddleTimer = 0;
int blockTimer = 0;
int outputTimer = 0;
int changeDir = 0;
int blockRow1 = 0x04;
int blockRow2 = 0x08;

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

void blockSetup()
{
  blocks.setBlock(blockRow1, 0x04);
  blocks.setBlock(blockRow1, 0x08);
  blocks.setBlock(blockRow1, 0x10);
  blocks.setBlock(blockRow1, 0x20);
  blocks.setBlock(blockRow2, 0x04);
  blocks.setBlock(blockRow2, 0x08);
  blocks.setBlock(blockRow2, 0x10);
  blocks.setBlock(blockRow2, 0x20);
}

//Clears Row Registers(clears array positions)
void rowRegClear()
{
  for(int i = 0; i < 8; ++i)
  {
    greenRowRegister[i] = 0;
    blueRowRegister[i] = 0;
  }
}

//Initialize Row Registers with all game positions
void rowRegInit()
{
  rowRegClear();
  for(int i = 0; i < 8; ++i)
  {
    if(ball.getCol() == colRegister[i])
    {
      blueRowRegister[i] += ball.getRow();
    }
    if(blocks.isBlockThere(blockRow1, colRegister[i]))
    {
      greenRowRegister[i] += blockRow1;
    }
    if(blocks.isBlockThere(blockRow2, colRegister[i]))
    {
      greenRowRegister[i] += blockRow2;
    }
    if(paddle.getLeftPaddle() == colRegister[i])
    {
      greenRowRegister[i] += paddle.getPaddleRow();
    }
    else if(paddle.getMidPaddle() == colRegister[i])
    {
      greenRowRegister[i] += paddle.getPaddleRow();
    }
    else if(paddle.getRightPaddle() == colRegister[i])
    {
      greenRowRegister[i] += paddle.getPaddleRow();
    }
  }
}

//Initialize Column Values
void colRegInit()
{
  colRegister[0] = 0x01;
  colRegister[1] = 0x02;
  colRegister[2] = 0x04;
  colRegister[3] = 0x08;
  colRegister[4] = 0x10;
  colRegister[5] = 0x20;
  colRegister[6] = 0x40;
  colRegister[7] = 0x80;
}

//Ball States
enum ballStates{ballInit, ballUpLeft, ballUpRight, ballDownLeft, ballDownRight} ballState;

//Ball Controllers
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
      if(changeDir == 1)
      {
        ballState = ballDownLeft;
        changeDir = 0;
      }
      else if(changeDir == 2)
      {
        ballState = ballDownRight;
        changeDir = 0;
      }
      else if( (ball.getRow() == 0x01) && (ball.getCol() == 0x01) )
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
      if(changeDir == 1)
      {
        ballState = ballDownRight;
        changeDir = 0;
      }
      else if(changeDir == 2)
      {
        ballState = ballDownLeft;
        changeDir = 0;
      }
      else if( (ball.getRow() == 0x01) && (ball.getCol() == 0x80) )
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
      //Block hit
      if(changeDir == 1)
      {
        ballState = ballUpLeft;
        changeDir = 0;
      }
      else if(changeDir == 2)
      {
        ballState = ballUpRight;
        changeDir = 0;
      }
      //Corner hit
      else if( (ball.hitPaddle((paddle.getPaddleRow() >> 1), paddle.getLeftPaddle(), 
                          paddle.getMidPaddle(), paddle.getRightPaddle())) &&
                          (ball.getCol() == 0x01) )
                          {
                            ballState = ballUpRight;
                          }
      //Corner and edge of paddle hit
      else if( (ball.getCol() == 0x01) && (ball.hitPaddle((paddle.getPaddleRow() >> 1), (paddle.getLeftPaddle() >> 1), 
                          (paddle.getMidPaddle() >> 1), (paddle.getRightPaddle()) >> 1)) )
                          {
                            ballState = ballUpRight;
                          }
      //Paddle hit
      else if( ball.hitPaddle((paddle.getPaddleRow() >> 1), paddle.getLeftPaddle(), 
                          paddle.getMidPaddle(), paddle.getRightPaddle()) )
                          {
                            ballState = ballUpLeft;
                          }
      //Edge of paddle hit
      else if( ball.hitPaddle((paddle.getPaddleRow() >> 1), (paddle.getLeftPaddle() << 1), 
                          (paddle.getMidPaddle() << 1), (paddle.getRightPaddle()) << 1) )
                          {
                            ballState = ballUpRight;
                          }
      //Wall hit
      else if(ball.getCol() == 0x01)
      {
        ballState = ballDownRight;
      }
      break;
    }
    case ballDownRight:
    {
      if(changeDir == 1)
      {
        ballState = ballUpRight;
        changeDir = 0;
      }
      else if(changeDir == 2)
      {
        ballState = ballUpLeft;
        changeDir = 0;
      }
      //Corner hit
      else if( (ball.hitPaddle((paddle.getPaddleRow() >> 1), paddle.getLeftPaddle(), 
                          paddle.getMidPaddle(), paddle.getRightPaddle())) &&
                          (ball.getCol() == 0x80) )
                          {
                            ballState = ballUpLeft;
                          }
      //Corner and edge of paddle hit
      if( (ball.getCol() == 0x80) && (ball.hitPaddle((paddle.getPaddleRow() >> 1), (paddle.getLeftPaddle() << 1), 
                          (paddle.getMidPaddle() << 1), (paddle.getRightPaddle()) << 1)) )
                          {
                            ballState = ballUpLeft;
                          }
      //Paddle hit
      else if(ball.hitPaddle((paddle.getPaddleRow() >> 1), paddle.getLeftPaddle(), 
                          paddle.getMidPaddle(), paddle.getRightPaddle()) )
                          {
                            ballState = ballUpRight;
                          }
      //Edge of paddle hit
      else if( ball.hitPaddle((paddle.getPaddleRow() >> 1), (paddle.getLeftPaddle() >> 1), 
                          (paddle.getMidPaddle() >> 1), (paddle.getRightPaddle()) >> 1) )
                          {
                            ballState = ballUpLeft;
                          }
      //Wall hit
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

//Paddle States
enum paddleStates{paddleInit, paddleStay, paddleLeft, paddleRight} paddleState;

//Paddle Controller
void paddleController()
{
  accelgyro.getAcceleration(&ax, &ay, &az);
  
  switch(paddleState)
  {
    case paddleInit:
    {
      paddleState = paddleStay;
      break;
    }
    case paddleStay:
    {
      if(ax < 4000 && ax > -4000)
      {
        paddleState = paddleStay;
      }
      else if(ax >= 4000)
      {
        paddleState = paddleLeft;
      }
      else if(ax <= -4000)
      {
        paddleState = paddleRight;
      }
      break;
    }
    case paddleLeft:
    {
      if(ax < 4000 && ax > -4000)
      {
        paddleState = paddleStay;
      }
      else if(ax >= 4000)
      {
        paddleState = paddleLeft;
      }
      else if(ax <= -4000)
      {
        paddleState = paddleRight;
      }
      break;
    }
    case paddleRight:
    {
      if(ax < 4000 && ax > -4000)
      {
        paddleState = paddleStay;
      }
      else if(ax >= 4000)
      {
        paddleState = paddleLeft;
      }
      else if(ax <= -4000)
      {
        paddleState = paddleRight;
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

//Block States
enum blockStates{blockInit, blockMissed, blockHit} blockState;

//Block Controller
void blockController()
{
  switch(blockState)
  {
    case blockInit:
    {
      blockSetup();
      blockState = blockMissed;
      break;
    }
    case blockMissed:
    {
      switch(ballState)
      {
        case ballUpLeft:
        {
          if( blocks.isBlockThere((ball.getRow() >> 1), ball.getCol()))
          {
            blocks.removeBlock((ball.getRow() >> 1), ball.getCol());
            changeDir = 1;
            blockState = blockHit;
          }
          else if( blocks.isBlockThere((ball.getRow() >> 1), (ball.getCol()) >> 1))
          {
            blocks.removeBlock((ball.getRow() >> 1), (ball.getCol() >> 1));
            changeDir = 2;
            blockState = blockHit;
          }
          break;
        }
        case ballUpRight:
        {
          if( blocks.isBlockThere((ball.getRow() >> 1), ball.getCol()))
          {
            blocks.removeBlock((ball.getRow() >> 1), ball.getCol());
            changeDir = 1;
            blockState = blockHit;
          }
          else if( blocks.isBlockThere((ball.getRow() >> 1), (ball.getCol()) << 1))
          {
            blocks.removeBlock((ball.getRow() >> 1), (ball.getCol() << 1));
            changeDir = 2;
            blockState = blockHit;
          }
          break;
        }
        case ballDownLeft:
        {
          if( blocks.isBlockThere((ball.getRow() << 1), ball.getCol()))
          {
            blocks.removeBlock((ball.getRow() << 1), ball.getCol());
            changeDir = 1;
            blockState = blockHit;
          }
          else if( blocks.isBlockThere((ball.getRow() << 1), (ball.getCol()) >> 1))
          {
            blocks.removeBlock((ball.getRow() << 1), (ball.getCol() >> 1));
            changeDir = 2;
            blockState = blockHit;
          }
          break;
        }
        case ballDownRight:
        {
          if( blocks.isBlockThere((ball.getRow() << 1), ball.getCol()))
          {
            blocks.removeBlock((ball.getRow() << 1), ball.getCol());
            changeDir = 1;
            blockState = blockHit;
          }
          else if( blocks.isBlockThere((ball.getRow() << 1), (ball.getCol()) << 1))
          {
            blocks.removeBlock((ball.getRow() << 1), (ball.getCol() << 1));
            changeDir = 2;
            blockState = blockHit;
          }
          break;
        }
      }
      break;
    }
    case blockHit:
    {
      blockState = blockMissed;
      break;
    }
    default:
    {
      blockState = blockInit;
      break;
    }
  }
  
  switch(blockState)
  {
    case blockInit:
    {
      break;
    }
    case blockMissed:
    {
      break;
    }
    case blockHit:
    {
      switch(ballState)
      {
        case ballUpLeft:
        {  
          break;
        }
        case ballUpRight:
        {
          break;
        }
        case ballDownLeft:
        {
          break;
        }
        case ballDownRight:
        {
          break;
        }
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

//Output States
enum outputStates{outputInit, col0, wait0, col1, wait1, col2, wait2, col3, wait3,
                    col4, wait4, col5, wait5, col6, wait6, col7, wait7} outputState;

//Output Controller
void outputController()
{
  rowRegInit();
  colRegInit();
  
  switch(outputState)
  {
    case outputInit:
    {
      outputState = col0;
      break;
    }
    case col0:
    {
      outputState = wait0;
      break;
    }
    case wait0:
    {
      outputState = col1;
      break;
    }
    case col1:
    {
      outputState = wait1;
      break;
    }
    case wait1:
    {
      outputState = col2;
      break;
    }
    case col2:
    {
      outputState = wait2;
      break;
    }
    case wait2:
    {
      outputState = col3;
      break;
    }
    case col3:
    {
      outputState = wait3;
      break;
    }
    case wait3:
    {
      outputState = col4;
      break;
    }
    case col4:
    {
      outputState = wait4;
      break;
    }
    case wait4:
    {
      outputState = col5;
      break;
    }
    case col5:
    {
      outputState = wait5;
      break;
    }
    case wait5:
    {
      outputState = col6;
      break;
    }
    case col6:
    {
      outputState = wait6;
      break;
    }
    case wait6:
    {
      outputState = col7;
      break;
    }
    case col7:
    {
      outputState = wait7;
      break;
    }
    case wait7:
    {
      outputState = col0;
      break;
    }
    default:
    {
      outputState = outputInit;
      break;
    }
  }
  
  switch(outputState)
  {
    case outputInit:
    {
      break;
    }
    case col0:
    {
      shiftVal(0, greenRowRegister[0], blueRowRegister[0], colRegister[0]);
      break;
    }
    case wait0:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col1:
    {
      shiftVal(0, greenRowRegister[1], blueRowRegister[1], colRegister[1]);
      break;
    }
    case wait1:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col2:
    {
      shiftVal(0, greenRowRegister[2], blueRowRegister[2], colRegister[2]);
      break;
    }
    case wait2:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col3:
    {
      shiftVal(0, greenRowRegister[3], blueRowRegister[3], colRegister[3]);
      break;
    }
    case wait3:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col4:
    {
      shiftVal(0, greenRowRegister[4], blueRowRegister[4], colRegister[4]);
      break;
    }
    case wait4:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col5:
    {
      shiftVal(0, greenRowRegister[5], blueRowRegister[5], colRegister[5]);
      break;
    }
    case wait5:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col6:
    {
      shiftVal(0, greenRowRegister[6], blueRowRegister[6], colRegister[6]);
      break;
    }
    case wait6:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    case col7:
    {
      shiftVal(0, greenRowRegister[7], blueRowRegister[7], colRegister[7]);
      break;
    }
    case wait7:
    {
      shiftVal(0xFF, 0xFF, 0xFF, 0);
      break;
    }
    default:
    {
      break;
    }
  }
}

void loop()
{
  if(ballTimer >= 500)
  {
    ballController();
    ballTimer = 0;
  }
  if(paddleTimer >= 250)
  {
    paddleController();
    paddleTimer = 0;
  }
  if(blockTimer >= 250)
  {
    blockController();
    blockTimer = 0;
  }
  outputController();
  outputTimer = 0;
  ++ballTimer;
  ++paddleTimer;
  ++blockTimer;
}
