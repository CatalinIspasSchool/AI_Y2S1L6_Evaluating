#include "AIConstructor_RBS.h"
#include "AIActor_Driver.h"


bool AIConstructor_RBS::Init() {

	method = AIConstructorBase::Method::RBS;
	return true;
}


/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_RBS::DefineActions()
{

	//Define the function for  turning away from collision
	auto avoidFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		// always turn left
		actor->Turn(-0.5f);
		//std::cout << " Turning " << -0.5 << std::endl;


		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAvoid", avoidFunction);




	//Define the function accelerating and centering
	auto accelFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		float curXpos = bb.GetValue<float>("TrackXPos");

		if (curXpos > 0.2)
		{
			actor->Turn(-0.05f);
			//std::cout << " Turning " << -0.05 << std::endl;
		}
		else if (curXpos < -0.2)
		{
			actor->Turn(0.05f);
			//std::cout << " Turning " << 0.05 << std::endl;
		}

		
		actor->Accel(20.f);


		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAccel", accelFunction);
	

}


/*
	DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_RBS::DefineConsiderations()
{


	// is there an obstacle of any type in reactDistance
	auto cObstacle = [](AIBrainBlackboardBase& bb) -> bool {

		int reactDistance = 5;

		// get all collision types from blackboard
		std::vector< AIMath_Global::Vector2f > _oil = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleOil");
		std::vector< AIMath_Global::Vector2f > _bollard = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleBollard");
		std::vector< AIMath_Global::Vector2f > _trash = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleTrash");

		// check if in car's path
		bool obstacleInPath = false;
		float curX = bb.GetValue<float>("TrackXPos");
		float _collideDist = 0.1f;

		for (auto c : _oil)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Oil detected at " << c.y << std::endl;
				return true;
			}
		}
	
		for (auto c : _bollard)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Bollard detected at " << c.y << std::endl;
				return true;
			}
		}

		for (auto c : _trash)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Trash detected at " << c.y << std::endl;
				return true;
			}
		}
		return(false);

		};



	AddConsiderationByName("ConsiderationObstacle", cObstacle);


	// is there NOT an obstacle of any type in reactDistance

	auto cNotObstacle = [](AIBrainBlackboardBase& bb) -> bool {

		int reactDistance = 5;

		// get all collision types from blackboard
		std::vector< AIMath_Global::Vector2f > _oil = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleOil");
		std::vector< AIMath_Global::Vector2f > _bollard = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleBollard");
		std::vector< AIMath_Global::Vector2f > _trash = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleTrash");

		// check if in car's path
		bool obstacleInPath = false;
		float curX = bb.GetValue<float>("TrackXPos");
		float _collideDist = 0.1f;
		
		for (auto c : _oil)
		{
			if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				return false;
			}
		}

		for (auto c : _bollard)
		{
			if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				return false;
			}
		}

		for (auto c : _trash)
		{
			if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				return false;
			}
		}
		return(true);

		};



	AddConsiderationByName("ConsiderationNOTObstacle", cNotObstacle);


}



/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_RBS::DefineOptions()
{

	AddOptionByName("OptionAvoid", "ActionAvoid");
	AddOptionConsideration("OptionAvoid", "ConsiderationObstacle");

	AddOptionByName("OptionAccel", "ActionAccel");
	AddOptionConsideration("OptionAccel", "ConsiderationNOTObstacle");
}



