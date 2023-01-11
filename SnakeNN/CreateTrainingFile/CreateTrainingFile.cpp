// CreateTrainingFile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

int main()
{
	std::ofstream sampleData;
	
	sampleData.open("trainingData.txt");
	
	sampleData << "topology: 2 4 4 1" << std::endl;
	for (int i = 2000; i >= 0; --i)
	{
		int n1 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int n2 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int t = n1 ^ n2;
	
		sampleData << "in: " << n1 << ".0 " << n2 << ".0 " << std::endl;
		sampleData << "out: " << t << ".0 " << std::endl;
	}
	sampleData.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
