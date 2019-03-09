#include "CompareNodes.h"



CompareNodes::CompareNodes()
{
}


CompareNodes::~CompareNodes()
{
}


bool CompareNodes::operator ( )(const Node& n1, const Node& n2)
{
	return n1.GetF()>n2.GetF();
}
