#include "stdafx.h"
#include "Profile.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


string Profile::getname()
{
	return name;
}

void Profile::setname(string Name)
{
	name = Name;
}

string Profile::getPassword()
{
	return password;
}

void Profile::setPassword(string Pass)
{
	password = Pass;
}

unsigned int Profile::getR()
{
	return R;
}

unsigned int Profile::getG()
{
	return G;
}

unsigned int Profile::getB()
{
	return B;
}

void Profile::setR(unsigned int Value)
{
	R = Value;
}

void Profile::setG(unsigned int Value)
{
	G = Value;
}

void Profile::setB(unsigned int Value)
{
	B = Value;
}



Profile::Profile()
{

}


Profile::~Profile()
{

}

bool Profile::operator==(Profile & right)
{
	if (this->getname() == right.getname() && this->getPassword() == right.getPassword()) return true;
	return false;
}

ifstream & operator>>(ifstream & os, Profile & dt)
{
	char tmpName[256],tmpPass[256],R[256], G[256], B[256];
	os.getline(tmpName,256);
	dt.setname(tmpName);
	os.getline(tmpPass, 256);
	dt.setPassword(tmpPass);
	os.getline(R, 256);
	dt.setR(atoi(R));
	os.getline(G, 256);
	dt.setG(atoi(G));
	os.getline(B, '\n');
	dt.setB(atoi(B));
	return os;
}

ofstream & operator<<(ofstream & os, Profile & dt)
{
	os << dt.getname().data() << endl << dt.getPassword().data()<<endl<<dt.R << endl <<dt.G<<endl<<dt.B<<endl;
	return os;
}


