#ifndef NEURON_CLASS
#define NEURON_CLASS
#include <vector>

struct Connection
{
	float Weight;
	float DeltaWeight;
};

class Neuron;

typedef std::vector<Neuron> Layer;
class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);

	void SetOutput(float output) { m_Output = output; };
	float GetOutput() const { return m_Output; };

	void FeedForward(const Layer& prevLayer);
	void CalcOutputGradients(float target);
	void CalcHiddenGradients(const Layer& nextLayer);
	void UpdateInputWeights(Layer& prevLayer);


private:

	//functions
	static float RandomWeight() { return rand() / float(RAND_MAX); };
	static float TransformFunction(float x);
	static float TransformFunctionDerivitave(float x);
	float SumDOW(const Layer& nextLayer);

	//members
	float m_Output;
	std::vector<Connection> m_OutputWeights;

	unsigned m_MyIndex;
	float m_Gradient;


	static float Eta; // range [0.f , 1.f]
	static float Alpha;// range [0.f , 1.f]

};
#endif

