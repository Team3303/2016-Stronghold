#include "WPILib.h"
#include <sstream>

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Talon shooter;
	DigitalInput shooter_stop;
	Joystick stick1, stick2, gamepad; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;
	int SPEED_MODIFIER = 0.75;
	DoubleSolenoid solenoid1;
	Compressor *c = new Compressor(0);
	Timer time_since, auto_time, shoot_time;
	bool x_btn() { return gamepad.GetRawButton(1); }
	bool a_btn() { return gamepad.GetRawButton(2); }
	bool b_btn() { return gamepad.GetRawButton(3); }
	bool y_btn() { return gamepad.GetRawButton(4); }
	bool lb() { return gamepad.GetRawButton(5); }
	bool rb() { return gamepad.GetRawButton(6); }
	bool lt() { return gamepad.GetRawButton(7); }
	bool rt() { return gamepad.GetRawButton(8); }
	bool back_btn() { return gamepad.GetRawButton(9); }
	bool start_btn() { return gamepad.GetRawButton(10); }
	float lstick_x() { return SPEED_MODIFIER*stick1.GetRawAxis(0); }
	float lstick_y() { return SPEED_MODIFIER*stick1.GetRawAxis(1); }
	float rstick_x() { return SPEED_MODIFIER*stick2.GetRawAxis(0); }
	float rstick_y() { return SPEED_MODIFIER*stick2.GetRawAxis(1); }

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
		stick1(0),
		stick2(1),
		gamepad(2),// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		solenoid1(2, 3)
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
		c->SetClosedLoopControl(true);
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
	}

	void TeleopInit()
	{
		c->SetClosedLoopControl(true);
	}

	void TeleopPeriodic()
	{
		//This is that part where we summon an alien mothership to control our robot for us
		myRobot.ArcadeDrive(rstick_y(),rstick_x());

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

		} else if(a_btn()){

			lower();

		}

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)


