/*
 * Node.cpp
 *
 *  Created on: Jan 27, 2011
 *      Author: martin
 */

#include "Node.h"
#include "Tag.h"
#include "Output.h"
#include <algorithm>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

int Node::nodeCounter = 0;

Node::Node(Parameters *p)
{
	this->id = Node::nodeCounter++;
	this->p = p;

	this->cache = new std::vector<Tag *>();
	this->interests = new std::vector<Tag *>();

	this->itemsReceived = new std::map<Node*, std::vector<Tag *>* >();
	this->utilityHistory = new std::map<Node*, std::vector<double>* >();
	this->myProviders = new std::map<Tag*, std::vector<Node *>* >();

	this->friends = new std::vector<Node *>();
	this->network = new std::vector<Node *>();
	this->notfriends = new std::vector<Node *>();

	this->iterationData = new std::vector<IterationData *>();

	this->setProducer(false);
}


Node::~Node()
{
	this->cache->clear();
	this->interests->clear();
	this->itemsReceived->clear();
	this->utilityHistory->clear();
	this->myProviders->clear();
	this->friends->clear();
	this->network->clear();
	this->notfriends->clear();
	this->iterationData->clear();
}

IterationData *Node::calculateStatistics()
{
	IterationData *iter = new IterationData(p->getCurrentIteration());
	iter->setPrecision(this->calculatePrecision());
	iter->setRecall(this->calculateRecall());
	iter->setSatisfied(this->calculateNumSatisfiedInterests());
	iterationData->push_back(iter);
	return iter;
}

double Node::calculateRecall()
{
	if(getInterests()->size() > 0)
	{
		return (double)calculateNumSatisfiedInterests()/(double)getInterests()->size();
	}
	else
	{
		return 0.0;
	}
}

double Node::calculatePrecision()
{
	if(cache->size() > 0)
	{
		return (double)calculateNumSatisfiedInterests()/(double)cache->size();
	}
	else
	{
		return 0.0;
	}
}

int Node::calculateNumSatisfiedInterests()
{
	int count = 0;
	foreach(Tag *t, *cache)
	{
		if((find(cache->begin(), cache->end(), t) != cache->end()))
		{
			count++;
		}
	}
	return count;
}

double Node::calculateUtility(std::vector<Tag*> *cache)
{
	int indirectTagCount = 0;
	int directTagCount = 0;

	foreach(Tag *t, *cache)
	{
		if(this->interestedIn(t))
		{
			directTagCount++;
		}
		std::pair<Node *, std::vector<Tag *>* > entry;
		foreach(entry, *itemsReceived)
		{
			std::vector<Tag *> *items = entry.second;
			if(find(items->begin(), items->end(), t) != items->end())
			{
				indirectTagCount++;
			}
		}
	}
	return p->getDirectUtil() * directTagCount + p->getIndirectUtil() * indirectTagCount;
}

double Node::getAverageUtility(Node *n)
{
	double utility = 0.0;
	std::map<Node *, std::vector<double>* >::iterator it = utilityHistory->find(n);
	if(it != utilityHistory->end())
	{
		std::vector<double> *utilityhist = it->second;
		if(utilityhist->size() > p->getUtilityMemoryWindow())
		{
			for(unsigned int i = 1; i <= p->getUtilityMemoryWindow(); i++)
			{
				utility += utilityhist->at(utilityhist->size()-i);
			}
			utility /= p->getUtilityMemoryWindow();
		}
		else
		{
			int count = 0;
			foreach(double d, *utilityhist)
			{
				utility += d;
				count++;
			}
			utility /= count;
		}
	}
	return utility;
}

void Node::addContentToCache(std::vector<Tag*> *receivedCache)
{
	std::map<Tag *, double> *utility = new std::map<Tag *, double>();
	std::vector<Tag *> *toBeChecked = new std::vector<Tag *>(*cache);

	// Add all the newly received tags to the toBeChecked list (except
	// duplicates)
	foreach(Tag *t, *receivedCache)
	{
		if(find(toBeChecked->begin(), toBeChecked->end(), t) == toBeChecked->end())
		{
			toBeChecked->push_back(t);
		}
	}
	std::vector<Tag *> *newCache = new std::vector<Tag *>();
	// We must keep tags we are a source of
	if(this->isProducer())
	{
		newCache->push_back(this->produces());
	}
	// Go through the list of tags
	foreach(Tag * t, *toBeChecked)
	{
		// If it's one of our interests, we definitely keep it
		if(find(interests->begin(), interests->end(), t) != interests->end())
		{
			if(find(newCache->begin(), newCache->end(), t) == newCache->end())
			{
				newCache->push_back(t);
			}
		}
		// Otherwise, calculate its value by asking friends if they are
		// interested
		// in the tag (directly or indirectly)
		else
		{
			int directlyInterestedCount = 0;
			int indirectlyInterestedCount = 0;
			double util = -std::numeric_limits<double>::max();
			foreach(Node *n, *friends)
			{
				if(n->interestedIn(t))
				{
					directlyInterestedCount++;
				}
				if(n->indirectlyInterestedIn(t))
				{
					indirectlyInterestedCount++;
				}
			}
			util = directlyInterestedCount * p->getDirectUtilContent() + indirectlyInterestedCount * p->getIndirectUtilContent();
			utility->insert(std::pair<Tag *, double>(t,util));
		}
	}
	// How much space do we have in the cache
	int freeSlots = p->getCacheSize() - newCache->size();
	int size = utility->size();
	// For all the items we've looked at, find the one worth most to us and
	// add to the cache
	for(int i = 0; i < size; i++)
	{
		double max = std::numeric_limits<double>::min();
		Tag *t;
		std::pair<Tag *, double> entry;
		foreach(entry, *utility)
		{
			if(entry.second > max)
			{
				max = entry.second;
				t = entry.first;
			}
		}
		// Make sure its not already in the cache and that it actually
		// exists
		if(find(newCache->begin(), newCache->end(), t) == newCache->end())
		{
			newCache->push_back(t);
			utility->erase(t);
			freeSlots--;
		}
	}
	this->cache = newCache;
}



std::vector<Tag*> *Node::getCache()
{
	return this->cache;
}



bool Node::haveMet(Node *n)
{
	return utilityHistory->find(n) != utilityHistory->end();
}



bool Node::providesTag(Tag *t)
{
	if(this->sourceOf == t)
	{
		return true;
	}
	else
	{
		return find(cache->begin(), cache->end(), t) != cache->end();
	}
}



Tag *Node::produceTag()
{
	Tag *t = new Tag(this, p);
	cache->push_back(t);
	this->sourceOf = t;
	std::vector<Node *> *providers = new std::vector<Node *>();
	providers->push_back(this);
	this->myProviders->insert(std::pair<Tag *, std::vector<Node *>* >(t, providers));
	return t;
}



bool Node::providesTag(Tag *t, std::vector<Node *> *providers)
{
	if(this->sourceOf == t)
	{
		return true;
	}
	else
	{
		std::map<Tag *, std::vector<Node *>* >::iterator it = myProviders->find(t);
		if(it != myProviders->end())
		{
			std::vector<Node *> *myProvidersForT = it->second;
			foreach(Node *n, *myProvidersForT)
			{
				if(find(providers->begin(), providers->end(), n) == providers->end())
				{
					return true;
				}
			}
		}
	}
	return false;
}



bool Node::isProducer()
{
	return this->producer;
}


void Node::setProducer(bool producer)
{
	this->producer = producer;
}


void Node::receiveContent(Node *source, std::vector<Tag*> *cache)
{
	itemsReceived->insert(std::pair<Node *, std::vector<Tag *>* >(source, cache));
	std::map<Node *, std::vector<double>* >::iterator it = utilityHistory->find(source);
	if(it != utilityHistory->end())
	{
		std::vector<double> *utilityHist = it->second;
		utilityHist->push_back(this->calculateUtility(cache));
	}
	else
	{
		std::vector<double> *utilityhist = new std::vector<double>();
		utilityhist->push_back(this->calculateUtility(cache));
		utilityHistory->insert(std::pair<Node*, std::vector<double>* >(source, utilityhist));
	}
	foreach(Tag *t , *cache)
	{
		std::map<Tag *, std::vector<Node* >* >::iterator it = myProviders->find(t);
		if(it != myProviders->end())
		{
			std::vector<Node *> *providers = it->second;
			if(find(providers->begin(), providers->end(), source) == providers->end())
			{
				providers->push_back(source);
			}
		}
		else
		{
			std::vector<Node *> *providers = new std::vector<Node *>();
			providers->push_back(source);
			myProviders->insert(std::pair<Tag*, std::vector<Node *>* >(t, providers));
		}
	}
}

bool Node::interestedIn(Tag *t)
{
	return (find(interests->begin(), interests->end(), t) != interests->end());
}



void Node::cleanCache()
{
	std::vector<Tag *> *toBeErased = new std::vector<Tag *>();
	foreach(Tag *t, *cache)
	{
		if(this->sourceOf != t)
		{
			if(myProviders->find(t) != myProviders->end())
			{
				std::vector<Node *> *providers = myProviders->find(t)->second;
				bool neighbourHasTag = false;
				foreach(Node *n, *friends)
				{
					if(n->providesTag(t, providers))
					{
						neighbourHasTag = true;
					}
				}
				if(!neighbourHasTag)
				{
					toBeErased->push_back(t);
					foreach(Node *n, *friends)
					{
						n->notifyTagRemoved(t, this);
					}
				}
			}
		}
	}
	foreach(Tag *t, *toBeErased)
	{
		cache->erase(find(cache->begin(), cache->end(), t));
	}
}



Tag *Node::produces()
{
	return this->sourceOf;
}



void Node::notifyTagRemoved(Tag *t, Node *source)
{
	if(!(this->sourceOf == t))
	{
		if(find(cache->begin(), cache->end(), t) != cache->end())
		{
			if(myProviders->find(t) != myProviders->end())
			{
				std::vector<Node *> *myProvidersForT = myProviders->find(t)->second;
				if(find(myProvidersForT->begin(), myProvidersForT->end(), source) != myProvidersForT->end())
				{
					myProvidersForT->erase(find(myProvidersForT->begin(), myProvidersForT->end(), source));
				}
				if(myProvidersForT->size() <= 0)
				{
					cache->erase(find(cache->begin(), cache->end(), t));
				}
			}
		}
	}
}



void Node::removeFriend(Node *n)
{
	friends->erase(find(friends->begin(), friends->end(), n));
	notfriends->push_back(n);
	foreach(Tag *t, *cache)
	{
		std::vector<Node *> *myProvidersForT = myProviders->find(t)->second;
		std::vector<Node *>::iterator it = find(myProvidersForT->begin(), myProvidersForT->end(), n);
		if(it != myProvidersForT->end())
		{
			myProvidersForT->erase(it);
		}
	}
}



bool Node::produces(Tag *t)
{
	if(!this->isProducer())
	{
		return false;
	}
	else
	{
		return this->sourceOf == t;
	}
}

std::vector<Tag*> *Node::getInterests()
{
	return this->interests;
}



void Node::addInterest(Tag *t)
{
	this->interests->push_back(t);
}



bool Node::isFriend(Node *n)
{
	return find(friends->begin(), friends->end(), n) != friends->end();
}



void Node::addFriend(Node *n)
{
	if(find(friends->begin(), friends->end(), n) == friends->end())
	{
		this->friends->push_back(n);
	}
}



void Node::addNetworkLink(Node *n)
{
	if(find(network->begin(), network->end(), n) == network->end())
	{
		network->push_back(n);
	}
}



bool Node::indirectlyInterestedIn(Tag *t)
{
	bool inCache = find(cache->begin(), cache->end(), t) != cache->end();
	bool notInterested = find(interests->begin(), interests->end(), t) == interests->end();
	bool notSourceOf = this->sourceOf != t;
	return inCache && notInterested && notSourceOf;
}

std::vector<Node*> *Node::getNetwork()
{
	return this->network;
}



std::string Node::getDetailedString()
{
	return std::string();
}



std::vector<Node*>* Node::getNotFriends()
{
	return this->notfriends;
}



int Node::getID()
{
	return this->id;
}



std::vector<Node*> *Node::getFriends()
{
	return this->friends;
}



bool Node::operator ==(Node *n)
{
	return this->getID() == n->getID();
}










