#pragma once
#include "Neuron.h"

struct Inputs
{
	float DistToLeftWallNorm{ 0.f }; //[0 to 1]
	float DistToRightWallNorm{ 0.f }; //[0 to 1]
	float DistToTopWallNorm{ 0.f }; //[0 to 1]
	float DistToBottomWallNorm{ 0.f }; //[0 to 1]
	float DistToFoodXNorm{ 0.f }; //[-1 to 1]
	float DistToFoodYNorm{ 0.f }; //[-1 to 1]
	float IsBodyUp{ 0.f }; //[0 or 1]
	float IsBodyDown{ 0.f }; //[0 or 1]
	float IsBodyLeft{ 0.f }; //[0 or 1]
	float IsBodyRight{ 0.f }; //[0 or 1]

	void ToVector(std::vector<float>& inputs) const
	{
		inputs.push_back(DistToLeftWallNorm);
		inputs.push_back(DistToRightWallNorm);
		inputs.push_back(DistToTopWallNorm);
		inputs.push_back(DistToBottomWallNorm);
		inputs.push_back(DistToFoodXNorm);
		inputs.push_back(DistToFoodYNorm);
		inputs.push_back(IsBodyUp);
		inputs.push_back(IsBodyDown);
		inputs.push_back(IsBodyLeft);
		inputs.push_back(IsBodyRight);
	};

	const static int NrOfInputs = 10;
};



struct Outputs
{
	float GoUpPercent{ 0.f };
	float GoLeftPercent{ 0.f };
	float GoRightPercent{ 0.f };
	float GoDownPercent{ 0.f };

	void ToVector(std::vector<float>& outputs) const
	{
		outputs.push_back(GoUpPercent);
		outputs.push_back(GoDownPercent);
		outputs.push_back(GoLeftPercent);
		outputs.push_back(GoRightPercent);
	};

	static Outputs FromLayer(const Layer& outputs)
	{
		Outputs toReturn{};

		toReturn.GoUpPercent = outputs[0].GetOutput();
		toReturn.GoDownPercent = outputs[1].GetOutput();
		toReturn.GoLeftPercent = outputs[2].GetOutput();
		toReturn.GoRightPercent = outputs[3].GetOutput();

		return toReturn;
	}

	const static int NrOfOutputs = 4;
};