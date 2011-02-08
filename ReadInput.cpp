/*
 * ReadInput.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#include "ReadInput.h"
#include "Output.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

template <class T> bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

ReadInput::ReadInput() {
	// TODO Auto-generated constructor stub

}

ReadInput::~ReadInput() {
	// TODO Auto-generated destructor stub
}

Parameters* ReadInput::readInputFromFile(const char* filename)
{
	Output::out("reading file");
	std::ifstream ifs(filename, std::ifstream::in);
	if(ifs.good())
	{
		Output::out("file open");
	}
	else
	{
		Output::out("file not open");
		exit(-1);
	}
	std::map<std::string, double> *params = new std::map<std::string, double>();
	while(ifs.good())
	{
		std::string str;
		std::getline(ifs, str);
		Output::out(str);
 		std::size_t separator;
 		separator = str.find("::");
 		if(separator != std::string::npos)
 		{
 			std::string param = str.substr(0,separator);
 			Output::out(param);
 			std::string value = str.substr(separator+2);
 			double v;
			if(from_string<double>(v, value, std::dec))
			{
				Output::out(v);
			}
			else
			{
				Output::out("from_string failed");
			}
			params->insert(std::pair<std::string, double>(param, v));
 		}
	}
	Parameters *p = new Parameters((int)params->find("seed")->second);
	p->setAcceptExchange(params->find("acceptexchange")->second);
	p->setAcceptFriend(params->find("acceptfriend")->second);
	p->setCacheSize((int)params->find("cachesize")->second);
	p->setDirectUtil(params->find("directutil")->second);
	p->setDirectUtilContent(params->find("directutilcontent")->second);
	p->setIndirectUtil(params->find("indirectutil")->second);
	p->setIndirectUtilContent(params->find("indirectutilcontent")->second);
	p->setIterations((int)params->find("iterations")->second);
	p->setNeighbourProb(params->find("neighbourprob")->second);
	p->setNonFriendExchange(params->find("nonfriendexchange")->second);
	p->setNumNodes((int)params->find("numnodes")->second);
	p->setNumTags((int)params->find("numtags")->second);
	p->setUtilityMemoryWindow((unsigned int)params->find("utilitymemorywindow")->second);
	p->setCurrentIteration(0);
	ifs.close();
	return p;
}


