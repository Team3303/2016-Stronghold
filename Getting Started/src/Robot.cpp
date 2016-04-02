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
	//PCM *pcm = new PCM(0);
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
	const float voltsDeadband = 0.008;
	const double voltsPerDegreePerSecond = 0.007;


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

	double alignSpeed = 0.9;

	void align(float angle){

		if(gyro.GetAngle() > (angle + 1.0) || gyro.GetAngle() < (angle - 1.0)){

			myRobot.TankDrive(alignSpeed, -1 * alignSpeed);   //turn left
			// myRobot.TankDrive((gyro.GetAngle() - 45.0)/180, -(gyro.GetAngle() - 45.0)/180);   //turn left

		}else if(gyro.GetAngle() < (angle - 1.0)){

			myRobot.TankDrive(-1 * alignSpeed, alignSpeed);   //turn right

		}else{

			myRobot.TankDrive(0.0, 0.0);

		}
	}


public:
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		shooter(2),
		shooter_stop(1),
		stick1(0),
		stick2(1),
		gamepad(2),// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		solenoid1(0, 1),
		gyro(1)
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
	    auto_time.Start();                    //start autonomous timer
	    gyro.Reset();
		gyro.SetSensitivity(voltsPerDegreePerSecond);
		raise();

		/*myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);*/
	}

	void AutonomousPeriodic()
	{
		std::stringstream stream1;
		std::stringstream stream2;
		std::string gyro_value;
		std::string gyro_rate;
		stream1 << gyro.GetAngle();
		stream1 >> gyro_value;
		stream2 << gyro.GetRate();
		stream2 >> gyro_rate;
		SmartDashboard::PutString("DB/String 0", gyro_value);
		SmartDashboard::PutString("DB/String 1", gyro_rate);

		std::stringstream ss5;
		std::string auto_timer_string;
		ss5<<auto_time.Get();
	    ss5>>auto_timer_string;
	    SmartDashboard::PutString("DB/String 2", auto_timer_string);

		double period1 = SmartDashboard::GetNumber("DB/Slider 0", 2.75);
		double period2 = SmartDashboard::GetNumber("DB/Slider 1", 2.3);
		double period3 = SmartDashboard::GetNumber("DB/Slider 2", 1.25);
		alignSpeed = SmartDashboard::GetNumber("DB/Slider 3", 0.65);

	    if(auto_time.Get() <= period1){
			myRobot.Drive(-0.75, 0.0);              //drive forward

		}else if(auto_time.Get() <= period1 + period2){


			align(60);

		}else if(auto_time.Get() <= period1 + period2 + period3){
			myRobot.Drive(-0.5, 0.0);              //drive forward

		}else if(SmartDashboard::GetBoolean("New Name", false) && (auto_time.Get() <= period1 + period2 + period3 + 1)){
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

		gyro.InitGyro();
		gyro.Reset();
		gyro.SetSensitivity(voltsPerDegreePerSecond);
		gyro.SetDeadband(voltsDeadband);

		/*myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);*/
		/*myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);*/
	}

	void TeleopPeriodic()
	{
		// Print the value of the gyro to Driver Station
		std::stringstream stream1;
		std::stringstream stream2;
		std::stringstream stream3;
		std::stringstream stream4;
		std::string gyro_value;
		std::string gyro_rate;
		std::string gyro_center;
		std::string gyro_offset;
		stream1 << gyro.GetAngle();
		stream1 >> gyro_value;
		stream2 << gyro.GetRate();
		stream2 >> gyro_rate;
		stream3 << gyro.GetCenter();
		stream3 >> gyro_center;
		stream4 << gyro.GetOffset();
		stream4 >> gyro_offset;
		SmartDashboard::PutString("DB/String 0", gyro_value);
		SmartDashboard::PutString("DB/String 1", gyro_rate);
		SmartDashboard::PutString("DB/String 2", gyro_center);
		SmartDashboard::PutString("DB/String 3", gyro_offset);

		if(rb()){
			gyro.Reset();
		}

		//This is that part where we summon an alien mother ship to control our robot for us
//		myRobot.ArcadeDrive(rstick_y(),rstick_x());
		myRobot.TankDrive(lstick_y(),rstick_y());

		//Shooter pulls in when x button pressed and limit switch not triggered
		if(shooter_stop.Get()){
			if(x_btn()){
				shooter.Set(-0.5);
			}
		}
		//Shooter shoots if limit switch pressed for as long as y pressed
		else{
			shooter.Set(0.0);
		}

		if(y_btn()){
			shooter.Set(1.0);
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


