//============================================================================
// Name        : TagExchange.cpp
// Author      : Martin Chorley
// Version     :
// Copyright   : Licensed under a Creative Commons Attribution 3.0 Licence (http://creativecommons.org/licenses/by/3.0/)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>

#include "Parameters.h"
#include "Node.h"
#include "Tag.h"
#include "ReadInput.h"
#include "NetworkFactory.h"
#include "Output.h"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
using namespace std;

Parameters *p;

std::vector<Node *> *population = new vector<Node *>();
std::vector<Tag *> *tags = new vector<Tag *>();

int iteration = 0;

void initialise();
void iterate();
void finalise();

int main() {
	initialise();
	for(int i = 0; i < 100; i++)
	{
		iterate();
	}
	finalise();

}

void initialise()
{
	ReadInput ri;
	Output::out("testing reading input");
	p = ri.readInputFromFile("/Users/martin/Documents/EclipseWorkspace/TagExchange/Debug/INPUT");
	Output::out(p->getNumNodes());
	Output::out(p->getCacheSize());
	Output::out(p->getNumTags());
	Output::out(p->getIterations());
	Output::out(p->getAcceptExchange());
	Output::out(p->getAcceptFriend());
	Output::out(p->getNeighbourProb());
	Output::out(p->getNonFriendExchange());
	Output::out(p->getDirectUtil());
	Output::out(p->getIndirectUtil());
	Output::out(p->getDirectUtilContent());
	Output::out(p->getIndirectUtilContent());
	Output::out(p->getUtilityMemoryWindow());
	Output::out(p->getSeed());

	for(int i = 0; i < p->getNumNodes(); i++)
	{
		population->push_back(new Node(p));
	}

	Output::out("population created");
	NetworkFactory *nf = new NetworkFactory();
	population = nf->buildRandomNetwork(population, p);

	Output::out("network created");

	// Generate tags and set producers of those tags randomly
	for(int i = 0; i < p->getNumTags(); i++)
	{
		int node = (int)(p->getNextRandomInt(0, p->getNumNodes()-1));
		// Only want to produce one tag per node currently
		while(population->at(node)->isProducer()) {
			node = (int)(p->getNextRandomInt(0, p->getNumNodes()-1));
		}
		population->at(node)->setProducer(true);
		Tag *t = population->at(node)->produceTag();
		tags->push_back(t);
	}

	Output::out("Tags created");

	// Randomly choose interests for population
	foreach(Node *n, *population)
	{
		int numInterest = p->getNextRandomDouble(0, p->getNumTags()-1);
		for(int i = 0; i < numInterest; i++)
		{
			Tag *t = tags->at(p->getNextRandomInt(0, p->getNumTags()-1));
			while(n->interestedIn(t))
			{
				t = tags->at(p->getNextRandomInt(0, p->getNumTags()-1));
			}
			n->addInterest(t);
		}
	}
	Output::sdoWriteOutput(p,population,tags);
}

void iterate()
{
	Output::out("Iteration", iteration);
	p->setCurrentIteration(iteration);
	// For every node in the population
	foreach(Node *i, *population)
	{
		Output::out("node", i->getID());
		// Pick a node to try and exchange with
		// Prioritise friends over non-friends according to value of nonFriendExchangeProbability
		double chooseNonFriendProb = p->getNextRandomDouble(0,1);
		Node *j;
		// Pick a random friend
		if(chooseNonFriendProb > p->getNonFriendExchange() && i->getFriends()->size() > 0)
		{
			std::vector<Node*> *friends = i->getFriends();
			j = friends->at(p->getNextRandomInt(0, friends->size()-1));
		}
		// Otherwise, pick a non-friend
		else
		{
			std::vector<Node*> *nonFriends = i->getNotFriends();
			// If we have any non-friends, pick one to exchange with
			if(nonFriends->size() > 0)
			{
				j = nonFriends->at(p->getNextRandomInt(0,nonFriends->size()-1));
			}
			// Otherwise just pick a friend if we can
			else
			{
				std::vector<Node*> *friends = i->getFriends();
				if(friends->size() > 0)
				{
					j = friends->at(p->getNextRandomInt(0,friends->size()-1));
				}
			}
		}
		Output::out("picked friend", j->getID());

		double util = 0.0;
		// node we pick gets to decide whether or not to reciprocate
		if(j->haveMet(i))
		{
			util = j->getAverageUtility(i);
		}
		else
		{
			util = std::numeric_limits<double>::max();;
		}

		bool exchanged = false;
		// if the chosen node thinks utility is high enough we make an exchange
		if(util > p->getAcceptExchange())
		{
			// exchange caches
			i->receiveContent(j, j->getCache());
			j->receiveContent(i, i->getCache());
			exchanged = true;
		}
		Output::out("exchanged");
		// check links and update social network
			// friend link, j didn't play, remove link. check caches
			// friend link, j did play, calc utility, if dropped too low, remove link
			// no friend link, j did play, calc utility, if high enough, make link
			// no friend link, if j didn't play do nothing
		if(i->isFriend(j))
		{
			if(!exchanged)
			{
				Output::out("removing friend", j->getID());
				i->removeFriend(j);
				j->removeFriend(i);
			}
			else
			{
				util = i->getAverageUtility(j);
				if(util < p->getAcceptFriend())
				{
					Output::out("removing friend", j->getID());
					i->removeFriend(j);
					j->removeFriend(i);
				}
			}
		}
		else
		{
			if(exchanged)
			{
				util = i->getAverageUtility(j);
				if(util > p->getAcceptFriend())
				{
					Output::out("adding friend", j->getID());
					i->addFriend(j);
					j->addFriend(i);
				}
			}
		}
		// If we are friends after all that, examine the received content and the content we
		// currently have in the cache and decide what we want to keep
		Output::out("adding content to cache");
		if(i->isFriend(j))
		{
			i->addContentToCache(j->getCache());
			j->addContentToCache(i->getCache());
		}
	}
	foreach(Node *n, *population)
	{
		n->cleanCache();
	}
	foreach(Node *n, *population)
	{
		n->calculateStatistics();
	}
	iteration++;
}

void finalise()
{
	foreach(Node *n, *population)
	{
		n->cleanCache();
	}
	Output::sdoWriteOutput(p,population,tags);
}
