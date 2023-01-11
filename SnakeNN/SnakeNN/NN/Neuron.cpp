#include "Neuron.h"

float Neuron::Eta = 0.15f;
float Neuron::Alpha = 0.1f;



Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned connection{}; connection < numOutputs; ++connection)
	{
		m_OutputWeights.push_back(Connection());
		m_OutputWeights.back().Weight = RandomWeight();
	}


	m_MyIndex = myIndex;
}

void Neuron::FeedForward(const Layer& prevLayer)
{
	float sum = 0.f;


	//Add up all pervious outputs +bias neuron
	for (unsigned neuron = 0; neuron < prevLayer.size(); ++neuron)
		sum += prevLayer[neuron].m_Output *
		prevLayer[neuron].m_OutputWeights[m_MyIndex].Weight;

	m_Output = Neuron::TransformFunction(sum);
}

void Neuron::CalcOutputGradients(float target)
{
	float delta = target - m_Output;
	m_Gradient = delta * Neuron::TransformFunctionDerivitave(m_Output);
}

void Neuron::CalcHiddenGradients(const Layer& nextLayer)
{
	float derivativeOfWeights = SumDOW(nextLayer);
	m_Gradient = derivativeOfWeights * Neuron::TransformFunctionDerivitave(m_Output);
}

void Neuron::UpdateInputWeights(Layer& prevLayer)
{

	for (unsigned neuronIndex = 0; neuronIndex < prevLayer.size() - 1; ++neuronIndex)
	{
		Neuron& neuron = prevLayer[neuronIndex];
		float oldDeltaWeight = neuron.m_OutputWeights[m_MyIndex].DeltaWeight;

		//eta = overall net learning rate

		//alpha = the momentum of the weight

		float newDeltaWeight = Eta * neuron.GetOutput() * m_Gradient
			+ Alpha * oldDeltaWeight;

		neuron.m_OutputWeights[m_MyIndex].DeltaWeight = newDeltaWeight;
		neuron.m_OutputWeights[m_MyIndex].Weight += newDeltaWeight;
	}
}

//output will be tanh with range[-1.f,1.f]
float Neuron::TransformFunction(float x)
{

	return tanh(x);
}

float Neuron::TransformFunctionDerivitave(float x)
{
	//tanh derivative
	return 1.f - x * x;
}

float Neuron::SumDOW(const Layer& nextLayer)
{
	float sum = 0.f;


	for (unsigned neuron = 0; neuron < nextLayer.size() - 1; ++neuron)
		sum += m_OutputWeights[neuron].Weight * nextLayer[neuron].m_Gradient;

	return sum;
}
