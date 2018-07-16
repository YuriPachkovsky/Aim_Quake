#include "stdafx.h"
#include "AccFileManager.h"

void AccFileManager::AddNewProf(Profile & Prof)
{
	profile.push_back(Prof);
}

bool AccFileManager::loginCheck(Profile & Prof)
{
	for each (Profile var in profile)
	{
		if (Prof == var) return true;
	}
	return false;
}

unsigned int AccFileManager::IndexOfElem(Profile & Prof)
{
	unsigned int count = 0;
	for each (Profile var in profile)
	{
		if (var == Prof) return count;
		count++;
	}
	return -1;
}

AccFileManager::AccFileManager()
{
	ifstream fin("PsevdoBasa.txt", ios_base::in);
	while (true)
	{
		Profile tmp;
		if (fin.eof()) break;
		fin >> tmp;
		if (tmp.getname() == "") break;
		profile.push_front(tmp);
	}
	fin.close();
}


AccFileManager::~AccFileManager()
{
	ofstream fout("PsevdoBasa.txt", ios_base::out| ios_base::trunc);
	unsigned int size = profile.size();
	for (unsigned int i = 0; i <size; i++)
	{
		fout << profile.front();
		profile.pop_front();
	}
	fout.close();
}
