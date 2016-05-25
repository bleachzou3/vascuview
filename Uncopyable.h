#ifndef UNCOPYABLE_HPP_
#define UNCOPYABLE_HPP_
class Uncopyable
{
protected:
	Uncopyable(){}
	~Uncopyable(){}
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};


#endif