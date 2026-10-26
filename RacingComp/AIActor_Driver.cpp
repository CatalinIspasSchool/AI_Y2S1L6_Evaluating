#include "AIActor_Driver.h"
#include "AIBrain_Driver.h"
#include "GameController.h"

AIActor_Driver::AIActor_Driver()
{

}



// **  AIActorBase overrides **

bool AIActor_Driver::Init(std::string _id, AIConstructorBase & _constructor)
{
	actorID = _id;

	// create new brain
	brain = std::make_unique<AIBrain_Driver>();

	brain->Init(_constructor, *this);



	return true;
}

/*
* SetGameContext will set the game controller context
*
 - AI decision making data should be on the blackboard, not accessed via the game controller

*/
void AIActor_Driver::SetGameContext(GameController& _gameController) {

	gameController = &_gameController;
}


void AIActor_Driver::SetDriverCar(Car& _car)
{
	driverCar = &_car;
}

/*
* Update will cause the AI Actor to use the brain to make a decision
*/
void AIActor_Driver::Update(float _dt)
{
	brain->Update();
}



/*
* unsed, but can be used for initialisation
*/
void AIActor_Driver::PreTransition()
{

}


/*
* Accel will increase the driver's car acceleation axis by the amount provided
* Must be a positive value
*/
void AIActor_Driver::Accel(float _amount)
{
	if (_amount > maxAccelChange)
		_amount = maxAccelChange;
	else if (_amount < 0)
		_amount = 0;

	driverCar->inputAccelAxis += _amount;

	if (driverCar->inputAccelAxis > maxAccelSpeed)
		driverCar->inputAccelAxis = maxAccelSpeed;

	if (driverCar->inputAccelAxis < -maxDecelSpeed)
		driverCar->inputAccelAxis = -maxDecelSpeed;

	EditBBValue("AccelAxis", driverCar->inputAccelAxis);

}

/*
* Accel will decrease the driver's car acceleation axis by the amount provided 
* * Must be a positive value
*/
void AIActor_Driver::Decel(float _amount)
{
	if (_amount > maxDecelChange)
		_amount = maxDecelChange;
	else if (_amount < 0)
		_amount = 0;


	driverCar->inputAccelAxis -= _amount;

	if (driverCar->inputAccelAxis > maxAccelSpeed)
		driverCar->inputAccelAxis = maxAccelSpeed;

	if (driverCar->inputAccelAxis < -maxDecelSpeed)
		driverCar->inputAccelAxis = -maxDecelSpeed;

	EditBBValue("AccelAxis", driverCar->inputAccelAxis);
}


/*
* Accel will change the driver's car acceleation axis
*/
void AIActor_Driver::Turn(float _amount)
{
	if (_amount > maxTurnSpeed)
		_amount = maxTurnSpeed;
	else if (_amount < -maxTurnSpeed)
		_amount = -maxTurnSpeed;

	if (gameController != nullptr)
		driverCar->inputSteerAxis += _amount;
	else
		std::cout << "WARNING [AIDriver]: Game Context Not Set" << std::endl;

	if (driverCar->inputSteerAxis > turnBound)
		driverCar->inputSteerAxis = turnBound;

	if (driverCar->inputSteerAxis < -turnBound)
		driverCar->inputSteerAxis = -turnBound;

	EditBBValue("SteerAxis", driverCar->inputSteerAxis);

	

}


Car& AIActor_Driver::GetDriverCar()
{
	return *driverCar;
}
