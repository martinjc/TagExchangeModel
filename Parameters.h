/*
 * Parameters.h
 *
 *  Created on: Jan 26, 2011
 *      Author: martin
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <boost/random/mersenne_twister.hpp>

class Parameters {
public:
	Parameters(int randomSeed);
	virtual ~Parameters();
	double getNextRandomDouble(double start, double end);
	int getNextRandomInt(int start, int end);
	double getAcceptExchange();
	double getAcceptFriend();
	int getCacheSize();
	double getDirectUtil();
	double getDirectUtilContent();
	double getIndirectUtil();
	double getIndirectUtilContent();
	int getIterations();
	double getNeighbourProb();
	double getNonFriendExchange();
	int getNumNodes();
	int getNumTags();
	int getSeed();
	unsigned int getUtilityMemoryWindow();
	int getCurrentIteration();
	void setAcceptExchange(double acceptExchange);
	void setAcceptFriend(double acceptFriend);
	void setCacheSize(int cacheSize);
	void setDirectUtil(double directUtil);
	void setDirectUtilContent(double directUtilContent);
	void setIndirectUtil(double indirectUtil);
	void setIndirectUtilContent(double indirectUtilContent);
	void setIterations(int iterations);
	void setNeighbourProb(double neighbourProb);
	void setNonFriendExchange(double nonFriendExchange);
	void setNumNodes(int numNodes);
	void setNumTags(int numTags);
	void setSeed(int seed);
	void setUtilityMemoryWindow(unsigned int utilityMemoryWindow);
	void setCurrentIteration(int iteration);


private:
	int numNodes;
	int cacheSize;
	int numTags;

	int iterations;
	int currentIteration;

	double acceptExchange;
	double acceptFriend;
	double neighbourProb;

	double nonFriendExchange;

	double directUtil;
	double indirectUtil;

	double directUtilContent;
	double indirectUtilContent;

	unsigned int utilityMemoryWindow;

	int seed;

	boost::mt19937 rng;
};

#endif /* PARAMETERS_H_ */
