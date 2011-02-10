/*
 * Output.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#include "Output.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

Output::Output() {
	// TODO Auto-generated constructor stub

}

Output::~Output() {
	// TODO Auto-generated destructor stub
}

void Output::out(std::string message)
{
	std::cout << message << std::endl;
}

void Output::out(float message)
{
	std::cout << message << std::endl;
}

void Output::out(int message)
{
	std::cout << message << std::endl;
}

void Output::out(double message)
{
	std::cout << message << std::endl;
}

void Output::out(unsigned int message)
{
	std::cout << message << std::endl;
}

void Output::out(std::string text, int value)
{
	std::cout << text << " " << value << std::endl;
}

void Output::sdoWriteOutput(Parameters *p, std::vector<Node *> *population, std::vector<Tag *> *tags)
{
	time_t seconds;
	time(&seconds);
	std::ofstream outfile("/Users/martin/Documents/EclipseWorkspace/TagExchange/Debug/Graph-Output-2.gv");  //+ p->getNumNodes() + "-Nodes-" + p->getNumTags() + "-Tags-" + p->getCacheSize() + "-CacheSize-" +  ".gv");
	outfile << "digraph G {\ngraph [center rankdir=LR bgcolor=\"#999999\"]\nedge [dir=none]\nnode [width=0.6 height=0.6 label=\"\"]" << std::endl;
	outfile.flush();
	foreach(Node *n, *population)
	{
		outfile << "{ node [shape=circle label = \"";
		outfile.flush();
		outfile << n->getID();
		outfile.flush();
		outfile << "\" color=\"#";
		outfile.flush();
		if(n->isProducer())
		{
			outfile << n->produces()->getColour();
		}
		else
		{
			outfile << "000000";
			outfile.flush();
		}
		outfile << "\"]";
		outfile.flush();
		outfile << n->getID();
		outfile.flush();
		outfile << "}" << std::endl;
		outfile.flush();
	}
	outfile << "{ edge [color=\"#000000\"]";
	outfile.flush();
	std::vector<Node *> *nodesDone = new std::vector<Node *>();
	foreach(Node *n, *population)
	{
		std::vector<Node *> *neighbours = n->getNetwork();
		foreach(Node *m , *neighbours)
		{
			if(find(nodesDone->begin(), nodesDone->end(), m) == nodesDone->end())
			{
				outfile << n->getID() << "->" << m->getID() << std::endl;
				outfile.flush();
			}
		}
		nodesDone->push_back(n);
	}
	outfile << "}" << std::endl;
	outfile.flush();
	outfile << "{ edge [color=\"#ffffff\"]";
	outfile.flush();
	nodesDone = new std::vector<Node *>();
	nodesDone->clear();
	foreach(Node *n, *population)
	{
		std::vector<Node *> *friends = n->getFriends();
		foreach(Node *m, *friends)
		{
			if(find(nodesDone->begin(), nodesDone->end(), m) == nodesDone->end())
			{
				outfile << n->getID() << "->" << m->getID() << std::endl;
				outfile.flush();
			}
		}
		nodesDone->push_back(n);
	}
	outfile << "}" << std::endl;
	outfile.flush();

	foreach(Tag *t, *tags)
	{
		nodesDone = new std::vector<Node *>();
		nodesDone->clear();
		outfile << "{ edge [color=\"#";
		outfile.flush();
		outfile << t->getColour();
		outfile.flush();
		outfile << "\"]";
		outfile.flush();
		foreach(Node *n, *population)
		{
			std::vector<Node *> *friends = n->getFriends();

			foreach(Node *m, *friends)
			{
				if(n->providesTag(t) && m->providesTag(t))
				{
					if((find(nodesDone->begin(), nodesDone->end(), m) == nodesDone->end()) && (find(nodesDone->begin(), nodesDone->end(), n) == nodesDone->end()))
					{
						outfile << n->getID() << "->" << m->getID() << std::endl;
						outfile.flush();
					}
				}
			}
			nodesDone->push_back(n);
		}
		outfile << "}" << std::endl;
		outfile.flush();
	}
	outfile << "}" << std::endl;
	outfile.flush();
	outfile.close();
}
