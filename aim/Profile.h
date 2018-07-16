#pragma once
#include <fstream>
#include <string>
using namespace std;



class Profile
{
private:
	string name;
	string password;
	unsigned int R=0, G=0, B=0;
public:
	string getname();
	void setname(string Name);

	string getPassword();
	void setPassword(string Pass);

	unsigned int getR();
	unsigned int getG();
	unsigned int getB();
	void setR(unsigned int Value);
	void setG(unsigned int Value);
	void setB(unsigned int Value);

	Profile();
	~Profile();
	bool operator==(Profile& right);

	friend ofstream& operator<<(ofstream& os, Profile& dt);
	friend ifstream& operator>>(ifstream& os, Profile& dt);
};


