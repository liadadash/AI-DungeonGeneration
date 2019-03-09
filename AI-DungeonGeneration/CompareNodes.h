#pragma once
#include "Node.h"
class CompareNodes
{
public:
	CompareNodes();
	~CompareNodes();
	bool operator ( ) (const Node& n1, const Node& n2);
};

