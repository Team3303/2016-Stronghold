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
	DoubleSolenoid solenoid1;
	DoubleSolenoid solenoid2;
	DoubleSolenoid solenoid3;
	Compressor *c = new Compressor(0);
	Timer time_since, auto_time, shoot_time;
	// FIX ME: Pointers for the following variables are not working
	bool x_btn() { return stick.GetRawButton(1); }
	bool a_btn() { return stick.GetRawButton(2); }
	bool b_btn() { return stick.GetRawButton(3); }
	bool y_btn() { return stick.GetRawButton(4); }
	bool lb() { return stick.GetRawButton(5); }
	bool rb() { return stick.GetRawButton(6); }
	bool lt() { return stick.GetRawButton(7); }
	bool rt() { return stick.GetRawButton(8); }
	bool back_btn() { return stick.GetRawButton(9); }
	bool start_btn() { return stick.GetRawButton(10); }
	float lstick_x() { return stick.GetRawAxis(0); }
	float lstick_y() { return 0.9*stick.GetRawAxis(1); }
	float rstick_x() { return stick.GetRawAxis(2); }
	float rstick_y() { return 0.9*stick.GetRawAxis(3); }

	void raise(){
		// pistons out
		solenoid1.Set(DoubleSolenoid::Value::kReverse);
	}

	void lower(){
		// pistons out
		solenoid1.Set(DoubleSolenoid::Value::kForward);
	}

	/*void drive(float seconds,float speed,float turn){

		myRobot.Drive(speed, turn);
		drive_time.Start();
		if(drive_time.HasPeriodPassed(seconds)){
			myRobot.Drive(0.0,0.0);
			drive_time.Stop();
			drive_time.Reset();
		}
	}*/

public:
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		shooter(3),
		shooter_stop(0),
		stick(0),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		solenoid1(2, 3),
		solenoid2(7, 6),
		solenoid3(0, 1)  //forward turns on first port
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void RobotInit() {
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{

		std::stringstream ss5;
		std::string auto_timer_string;
		ss5<<auto_time.Get();
	    ss5>>auto_timer_string;
	    SmartDashboard::PutString("DB/String 0", auto_timer_string);
		auto_time.Start();  //start autonomous timer

	    if(!auto_time.HasPeriodPassed(0.1)){
			myRobot.Drive(-0.05, 1.0);              //drive forward

		}else if(!auto_time.HasPeriodPassed(0.2)){
			myRobot.TankDrive(1.0, -1.0);         //turn right

		}else if(!auto_time.HasPeriodPassed(0.3)){
			myRobot.Drive(-0.5, 0.0);              //drive forward

		}else if(!auto_time.HasPeriodPassed(0.4)){
			myRobot.Drive(0.0, 0.0);              //stop and shoot
			shooter.Set(1.0);

		}else{
			myRobot.Drive(0.0, 0.0);
			shooter.Set(0.0);                     //stop all motors
		}

		/*if(autoLoopCounter < 50) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
//			shooter.SetInverted(true);
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
		}*/
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		c->SetClosedLoopControl(true);

		//myRobot.ArcadeDrive(stick);  drive with arcade style (use right stick)
//		shooter.SetInverted(true);

//		if(stick.GetRawButton(1) && !shooter_stop.Get()){
//			shooter.Set(-0.5);    //shooter pulls in if limit switch not pressed and x button pressed
//		}
//		else if(stick.GetRawButton(4)) {
//			shooter.Set(1);    //shooter shoots if y button pressed
//		}
//		else {
//			shooter.Set(0);
//		}

		//This is that part where we summon an alien mothership to control our robot for us
		myRobot.TankDrive(lstick_y(),rstick_y());

		//Shooter pulls in when x button pressed and limit switch not triggered
		if(shooter_stop.Get()){
			if(x_btn()){
				shooter.Set(-0.5);
			}
		}
		//Shooter shoots if limit switch pressed for as long as y pressed
		else {
			if(y_btn()){
				shooter.Set(1.0);
			} else {
				shooter.Set(0.0);
			}
		}

		if(lt() && rt()) {
			if (start_btn()) {
				shooter.Set(0.0);
			}
			else if (y_btn()) {
				shooter.Set(1.0);
			}

		}

		if(b_btn()){

			raise();

			// pistons in
			//solenoid1.Set(DoubleSolenoid::Value::kForward);
		}else if(a_btn()){

			lower();

			// pistons out
			//solenoid1.Set(DoubleSolenoid::Value::kReverse);
		}

//		checkRaise(2);

		/*if(rb()){
			// switch to 60 psi
			solenoid2.Set(DoubleSolenoid::Value::kForward);
			solenoid3.Set(DoubleSolenoid::Value::kReverse);
		}else if(lb()){
			// switch to 30 psi (or other)
			solenoid2.Set(DoubleSolenoid::Value::kReverse);
			solenoid3.Set(DoubleSolenoid::Value::kForward);
		} */

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)


