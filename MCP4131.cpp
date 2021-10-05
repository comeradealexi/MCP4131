#include <wiringPi.h>

class MCP4131
{
public:
	MCP4131(int inGPIO_CS, int inGPIO_SCK, int inGPIO_SDI) : GPIO_CS(inGPIO_CS), GPIO_SCK(inGPIO_SCK), GPIO_SDI(inGPIO_SDI)
	{
		wiringPiSetupSys();

		pinMode(GPIO_CS, OUTPUT);
		pinMode(GPIO_SCK, OUTPUT);
		pinMode(GPIO_SDI, OUTPUT);


		digitalWrite(GPIO_CS, LOW);
		digitalWrite(GPIO_SCK, LOW);
		digitalWrite(GPIO_SDI, LOW);

		digitalWrite(GPIO_CS, HIGH);
		digitalWrite(GPIO_SCK, LOW);
		digitalWrite(GPIO_SDI, LOW);
	}

	~MCP4131()
	{

	}

	//7 bit values 0-127
	void WriteData(unsigned char Value)
	{
		const int DelayTime = 10;

		if (Value > 127) Value = 127; //Clamp to 7 bit max value

		digitalWrite(GPIO_CS, LOW);
		delayMicroseconds(DelayTime);

		for (int i = 0; i < 16; i++)
		{
			bool bHigh = false; //First 9 bits are all 0

			//Write out the last 7 data bits (MSB fist)
			if (i > 8)
			{
				unsigned char idx = (i - 9); //6 to 0
				unsigned char bit = 1 << (6 - idx);
				bHigh = Value & bit;
			}

			digitalWrite(GPIO_SDI, bHigh ? HIGH : LOW);
			digitalWrite(GPIO_SCK, HIGH);
			delayMicroseconds(DelayTime);
			digitalWrite(GPIO_SCK, LOW);
			delayMicroseconds(DelayTime);
		}

		digitalWrite(GPIO_CS, HIGH);
	}

private:
	const int GPIO_CS;
	const int GPIO_SCK;
	const int GPIO_SDI;
};


int main()
{
	MCP4131 x(17, 27, 22);
	while (1)
	{
		for (int i = 0; i < 128; i++)
		{
			x.WriteData((unsigned char)i);
			delay(10);
		}

		for (int i = 127; i >= 0; i--)
		{
			x.WriteData((unsigned char)i);
			delay(10);
		}
	}
}