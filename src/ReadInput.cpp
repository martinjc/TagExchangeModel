/*
 * ReadInput.cpp
 *
 * Unsurprisingly, ReadInput reads input parameters for the TagExchange simulation from file.
 * Input files should be specified as string::value pairs, where valid parameters are currently
 *
 * numnodes 			<- the number of nodes in the simulation
 * cachesize 			<- the size of each nodes cache
 * numtags 				<- the number of tags in the simulation
 * iterations			<- the number of iterations to simulate
 * acceptexchange 		<- the average utility required from interacting with a node for an exchange to be accepted
 * acceptfriend 		<- the average utility required from a node in order to maintain a friend relationship with that node
 * neighbourprob 		<- probability of any pair of nodes having a network link
 * nonfriendexchange 	<- probability of attempting an exchange with a node that is not a friend
 * directutil 			<- utility modifier for tags we are directly interested in
 * indirectutil 		<- utility modifier for tags we are not directly interested in
 * directutilcontent 	<- modifier for tags we are interested in when deciding which tags to keep
 * indirectutilcontent 	<- modifier for tags we are indirectly interested in when deciding which tags to keep
 * utilitymemorywindow 	<- memory window for remembering interactions - average utility received based on last X interactions
 * seed 				<- random seed for RNG controlling simulation
 *
 * Created on: Feb 3, 2011
 *      Author: martin
 */

#include "ReadInput.h"
#include "Output.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <time.h>

template< class T> inline std::string to_string( const T & value)
{
	// auxiliary method which returns a string from an object of the class T
	std::stringstream streamOut;
	streamOut << value;
	return streamOut.str( );
}

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

Parameters* ReadInput::readInputFromFile()
{
	boost::filesystem::path working_directory =  boost::filesystem::initial_path<boost::filesystem::path>();
	Output::out("reading input file");
	std::cout << working_directory << std::endl;
	boost::filesystem::path inputfile = boost::filesystem::initial_path<boost::filesystem::path>() / "INPUT";
	std::ifstream ifs(inputfile.string().c_str(), std::ifstream::in);
	if(!ifs.good())
	{
		Output::out("file error");
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
 			std::string value = str.substr(separator+2);
 			double v;
			if(from_string<double>(v, value, std::dec))
			{
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
	p->setDirectory(working_directory);
	ifs.close();
	return p;
}


