#ifndef NULL_POINTER_EXCEPTION_HPP_
#define NULL_POINTER_EXCEPTION_HPP_
using namespace std;
#include <exception>
class NullPointerException:public exception
{
public:
	NullPointerException(const char* msg):exception(msg)
	{
	}

};
#endif