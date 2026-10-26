#include "AIBrain_Driver.h"
#include "RBS_Reasoner.h"
#include "FSM_Reasoner.h"

void AIBrain_Driver::Update()
{
	// call the reasoner to get the selected action
	brainReasoner->Update();

}

bool AIBrain_Driver::Init(AIConstructorBase& _constructor, AIActorBase& _actorContext)
{
	brainBlackboard = std::make_unique<AIBrainBlackboard>();
	brainBlackboard->Init(_actorContext);

	// create the brain based on the constructor method
	if (_constructor.method == AIConstructorBase::Method::RBS)
	{
		brainReasoner = std::make_unique < RBS_Reasoner >();

		brainReasoner->SetOptions(_constructor);

		brainReasoner->Init("RBS Reasoner", *brainBlackboard);

	}
	else if(_constructor.method == AIConstructorBase::Method::FSM)
	{
		brainReasoner = std::make_unique < FSM_Reasoner >();

		brainReasoner->SetOptions(_constructor);

		brainReasoner->Init("FMS Reasoner", *brainBlackboard);

		FSM_Reasoner* _reasoner = static_cast<FSM_Reasoner*>(GetReasoner());
		_reasoner->SetStartingState(FSM_Reasoner::DriverState::Accelerating, "OptionAccelerate");
	}

	return true;
}
