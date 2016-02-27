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
	float SPEED_MODIFIER = 1.0;
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
	float lstick_x() { return stick1.GetRawAxis(0); }
	float lstick_y() { return stick1.GetRawAxis(1); }
	float rstick_x() { return stick2.GetRawAxis(0); }
	float rstick_y() { return stick2.GetRawAxis(1); }

	AnalogGyro gyro;
	//gyro calibration constant, may need to be adjusted
	//gyro value of 360 is set to correspond to one full revolution
	const double voltsPerDegreePerSecond = 0.0128;


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
		solenoid1(2, 3),
		gyro(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void RobotInit() {
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		c->SetClosedLoopControl(true);
		gyro.InitGyro();
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
		c->SetClosedLoopControl(true);
		auto_time.Stop();                     //stop and reset timer
	    auto_time.Reset();
	    gyro.Reset();
		gyro.SetSensitivity(voltsPerDegreePerSecond);

		/*myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);*/
	}

	void AutonomousPeriodic()
	{

		std::stringstream ss5;
		std::string auto_timer_string;
		ss5<<auto_time.Get();
	    ss5>>auto_timer_string;
	    SmartDashboard::PutString("DB/String 0", auto_timer_string);
		auto_time.Start();  //start autonomous timer

	    if(auto_time.Get() <= 1.0){
			myRobot.Drive(-0.5, 0.0);              //drive forward

		}else if(auto_time.Get() <= 1.5){
			myRobot.TankDrive(-1.0, 1.0);         //turn right

		}else if(auto_time.Get() <= 2.5){
			myRobot.Drive(-0.5, 0.0);              //drive forward

		}else if(auto_time.Get() <= 3.5){
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

	    gyro.Reset();
		gyro.SetSensitivity(voltsPerDegreePerSecond);

		/*myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);*/
		/*myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);*/
	}

	void TeleopPeriodic()
	{
		// Print the value of the gyro to Driver Station
		std::stringstream stream;
		std::string gyro_value;
		stream << gyro.GetAngle();
		stream >> gyro_value;
		SmartDashboard::PutString("DB/String 0", gyro_value);

		//This is that part where we summon an alien mothership to control our robot for us
//		myRobot.ArcadeDrive(rstick_y(),rstick_x());
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


