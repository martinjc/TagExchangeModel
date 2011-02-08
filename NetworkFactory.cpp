/*
 * NetworkFactory.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#include "NetworkFactory.h"
#include "Parameters.h"
#include "Node.h"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

NetworkFactory::NetworkFactory() {
	// TODO Auto-generated constructor stub

}

NetworkFactory::~NetworkFactory() {
	// TODO Auto-generated destructor stub
}

std::vector<Node*>* NetworkFactory::buildRandomNetwork(std::vector<Node*> *pop, Parameters *p)
{
	// Set up random network between nodes. No guarantee of connectedness or friends
	foreach(Node *n, *pop)
	{
		foreach(Node *m, *pop)
		{
			if(n != m)
			{
				double prob = p->getNextRandomDouble(0,1);
				if(prob < p->getNeighbourProb())
				{
					// To begin with, everyone is friends with all of their neighbours
					n->addNetworkLink(m);
					n->addFriend(m);
					m->addNetworkLink(n);
					m->addFriend(n);
				}
			}
		}
	}
	foreach(Node *n, *pop)
	{
		if(n->getNetwork()->size() == 0)
		{
			int node = p->getNextRandomInt(0,pop->size());
			Node *j = pop->at(node);
			while(j == n)
			{
				node = p->getNextRandomInt(0, pop->size());
				j = pop->at(node);
			}
			n->addNetworkLink(j);
			n->addFriend(j);
			j->addNetworkLink(n);
			j->addFriend(n);
		}
	}
	return pop;
}
