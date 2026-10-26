#include "AIConstructor_FSM.h"
#include "AIActor_Driver.h"


bool AIConstructor_FSM::Init() {

	method = AIConstructorBase::Method::FSM;
	return true;
}


/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_FSM::DefineActions()
{

	//Define the function for Accelerating
	auto accelFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		actor->Accel(20.f);
		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAccelerate", accelFunction);


	//Define the function for Steering to centre of track
	auto centreFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		float curSteer = bb.GetValue<float>("TrackXPos");

		if (curSteer > 0.2)
			actor->Turn(-0.05f);
		else if (curSteer < -0.2)
			actor->Turn(0.05f);
		

		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionCentre", centreFunction);

}


/*
	DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_FSM::DefineConsiderations()
{



}



/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_FSM::DefineOptions()
{

	// create the two option
	AddOptionByName("OptionAccelerate", "ActionAccelerate");
	AddOptionByName("OptionCentre", "ActionCentre");
}



