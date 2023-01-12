#ifndef UTILSSNAKE
#define UTILSSNAKE
#include <string>

namespace Utils 
{

	bool AreSame(float a, float b)
	{
		return fabs(a - b) < FLT_EPSILON;
	}
}
#endif

