#ifndef Motors_h
#define Motors_h

#ifdef EBOT_8

#define M11 9
#define M12 10
#define PM1 13

#define M21 11
#define M22 12
#define PM2 14

#define ME 15
#endif


#ifdef EBOT_4

//left
#define M11 4
#define M12 12
#define PM1 6

//right
#define M21 9
#define M22 30
#define PM2 5
#define ME 255
#endif

#ifdef EBOT_X

#define MR11 14
#define MR12 15
#define PMR1 21

#define MR21 12
#define MR22 13
#define PMR2 20

#define ML11 16
#define ML12 17
#define PML1 22

#define ML21 18
#define ML22 19
#define PML2 23

#endif


class Motors
{
  public:

    // constructor (doesn't do anything)
	Motors();

	// sets the motor speed.  The sign of 'speed' determines the direction
	// and the magnitude determines the speed.  limits: -255 <= speed < 255
	// |speed| = 255 produces the maximum speed while speed = 0 is full brake.

	#if defined(EBOT_8) || defined(EBOT_4)

	static void setMLSpeed(int speed);
	static void setMRSpeed(int speed);
	//static void setMSpeed(int speed1,int speed2);

	#endif
	#ifdef EBOT_X
	static void setML1Speed(int speed);
	static void setML2Speed(int speed);
	static void setMLSpeed(int speed1,int speed2);

	static void setMR1Speed(int speed);
	static void setMR2Speed(int speed);
	static void setMRSpeed(int speed1,int speed2);
	#endif

	static void disable();
	//static void setMLSpeeds(int mL1Speed, int mL2Speed);
	//static void setMRSpeeds(int mR1Speed, int mR2Speed);

  private:

	static inline void init()
	{
		static unsigned char initialized = 0;

		if (!initialized)
		{
			initialized = 1;
			init2();
		}
	}

	// initializes timers 0 and 2 for proper PWM generation
	static void init2();
};


#endif

// Local Variables: **
// mode: C++ **
// c-basic-offset: 4 **
// tab-width: 4 **
// indent-tabs-mode: t **
// end: **
