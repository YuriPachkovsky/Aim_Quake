#pragma once
#include "Deque"
#include "Profile.h"
using namespace std;

class AccFileManager
{
public:
	deque<Profile> profile;
	void AddNewProf(Profile &Prof);
	bool loginCheck(Profile &Prof);
	unsigned int IndexOfElem(Profile &Prof);
	AccFileManager();
	~AccFileManager();
};

