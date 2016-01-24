#include "WPILib.h"
#include <sstream>

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Talon shooter;
	DigitalInput shooter_stop;
	Joystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;

	// FIX ME: Pointers for the following variables are not working
	bool x_btn;
	bool a_btn;
	bool b_btn;
	bool y_btn;
	bool lb;
	bool rb;
	bool lt;
	bool rt;
	float lstick_x;
	float lstick_y;
	float rstick_x;
	float rstick_y;



public:
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		shooter(3),
		shooter_stop(0),
		stick(0),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),

		x_btn(stick.GetRawButton(1)),
		a_btn(stick.GetRawButton(2)),
		b_btn(stick.GetRawButton(3)),
		y_btn(stick.GetRawButton(4)),
		lb(stick.GetRawButton(5)),
		rb(stick.GetRawButton(6)),
		lt(stick.GetRawButton(7)),
		rt(stick.GetRawButton(8)),

		lstick_x(stick.GetRawAxis(0)),
		lstick_y(stick.GetRawAxis(1)),
		rstick_x(stick.GetRawAxis(2)),
		rstick_y(stick.GetRawAxis(3))
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		if(autoLoopCounter < 50) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			shooter.SetInverted(true);
			shooter.Set(-0.5);
			std::stringstream ss;
			std::string a;
			ss<<shooter.Get();
			ss>>a;
			SmartDashboard::PutString("DB/String 0", a);
			autoLoopCounter++;
		}
		else if(autoLoopCounter < 100) {
			// wait
			shooter.Set(0.0);
			autoLoopCounter++;
		}
		else if(autoLoopCounter < 150) {
			shooter.Set(1.0);
			autoLoopCounter++;
		}
		else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
			shooter.Set(0.0);
		}
	}

	void TeleopInit()
	{
		shooter.SetInverted(true);
	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)



//		if(stick.GetRawButton(1) && !shooter_stop.Get()){
//			shooter.Set(-0.5);    //shooter pulls in if limit switch not pressed and x button pressed
//		}
//		else if(stick.GetRawButton(4)) {
//			shooter.Set(1);    //shooter shoots if y button pressed
//		}
//		else {
//			shooter.Set(0);
//		}

		if(!shooter_stop.Get()){
			if(x_btn){
				shooter.Set(-0.5);
			}
		}
		else {
			if(y_btn){
				shooter.Set(1.0);
			} else {
				shooter.Set(0.0);
			}
		}


	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
