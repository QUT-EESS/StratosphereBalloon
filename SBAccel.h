/*
 * Stratosphere Balloon
 * SBAccel.h
 * Purpose: Abstracts all Accelerometer functions
 *		Note: this library is heavily extracted and deArduino'd from the Seeedstudio GitHub repo
 *			https://github.com/Seeed-Studio/Accelerometer_ADXL335
 * Created: 30/12/2014
 * Author(s): Blake Fuller
 * Status: UNTESTED
 */ 

 #include "RDAnalog.h"
 
/*macro definitions of Analog read pins*/
#define X_AXIS_PIN 1
#define Y_AXIS_PIN 2
#define Z_AXIS_PIN 3

#define ADC_AMPLITUDE 1024	//amplitude of the 10bit-ADC of Bolt is 1024LSB
#define ADC_REF 3.3   		//ADC reference is 5v
#define ZERO_X  1.22 		//acceleration of X-AXIS is 0g, the voltage of X-AXIS is 1.22v
#define ZERO_Y  1.22 		//
#define ZERO_Z  1.25 		//
#define SENSITIVITY 0.25	//sensitivity of X/Y/Z axis is 0.25v/g
#define XCAL 3.649
#define YCAL 3.649
#define ZCAL 3.769

static float scale;

void SBAccelPinsInit(void)
{
    DDRF &= ~(1<<X_AXIS_PIN)&~(1<<Y_AXIS_PIN)&~(1<<Z_AXIS_PIN);
	scale = (float)SENSITIVITY*ADC_AMPLITUDE/ADC_REF;
}

void SBAccelCal(void) // Complete me!!!
{

}

void SBAccelGetXYZ(int16_t *x,int16_t *y,int16_t *z)
{
    *x = RDAnalogReadAvg(X_AXIS_PIN, MODE_10_BIT, 5);
    *y = RDAnalogReadAvg(Y_AXIS_PIN, MODE_10_BIT, 5);
    *z = RDAnalogReadAvg(Z_AXIS_PIN, MODE_10_BIT, 5);
}

void SBAccelGetAccelerationFl(float *ax,float *ay,float *az)
{
    int16_t x,y,z;
    float xvoltage,yvoltage,zvoltage;
    SBAccelGetXYZ(&x,&y,&z);
    xvoltage = (float)x*ADC_REF/ADC_AMPLITUDE;
    yvoltage = (float)y*ADC_REF/ADC_AMPLITUDE;
    zvoltage = (float)z*ADC_REF/ADC_AMPLITUDE;
    *ax = (xvoltage - ZERO_X)/SENSITIVITY;
    *ay = (yvoltage - ZERO_Y)/SENSITIVITY;
    *az = (zvoltage - ZERO_Z)/SENSITIVITY;
	
	*ax += XCAL;
	*ay += YCAL;
	*az += ZCAL;
}

void SBAccelGetAccelerationInt(int *ax, int *ay, int *az){
	float x, y, z;
	SBAccelGetAccelerationFl(&x, &y, &z);
	*ax = (int) (x*1000);
	*ay = (int) (y*1000);
	*az = (int) (z*1000);
}

void ABAccelToLCD(void){
	int x, y, z;
	char xStr[10], yStr[10], zStr[10];
	SBAccelGetAccelerationInt(&x, &y, &z);
	itoa(x, xStr, 10);
	itoa(y, yStr, 10);
	itoa(z, zStr, 10);
	
	RDLCDPosition(0,0);
	RDLCDString("X:         ");
	RDLCDPosition(15,0);
	RDLCDString(xStr);
	RDLCDPosition(0,1);
	RDLCDString("Y:         ");
	RDLCDPosition(15,1);
	RDLCDString(yStr);
	RDLCDPosition(0,2);
	RDLCDString("Z:         ");
	RDLCDPosition(15,2);
	RDLCDString(zStr);
}
