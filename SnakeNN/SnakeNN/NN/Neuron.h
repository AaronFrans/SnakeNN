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
	//only for Deserializing
	Neuron() = default;


	void SetOutput(float output) { m_Output = output; };
	float GetOutput() const { return m_Output; };

	void SetGradient(float gradient) { m_Gradient = gradient; };
	float GetGradient() const { return m_Gradient; };

	void SetOutputWeights(std::vector<Connection>& outputWeights) {m_OutputWeights = outputWeights; };
	std::vector<Connection>& GetOutputWeights() { return m_OutputWeights; };

	void SetIndex(unsigned index) { m_MyIndex = index; };
	unsigned GetIndex() const { return m_MyIndex; };

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
	float m_Output{};
	std::vector<Connection> m_OutputWeights{};

	unsigned m_MyIndex{};
	float m_Gradient{};


	static float Eta; // range [0.f , 1.f]
	static float Alpha;// range [0.f , 1.f]

};
#endif

