/*
 * NetworkFactory.h
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#ifndef NETWORKFACTORY_H_
#define NETWORKFACTORY_H_

#include "Node.h"
#include "Parameters.h"
#include <vector>

class NetworkFactory {
public:
	NetworkFactory();
	virtual ~NetworkFactory();
	std::vector<Node*>* buildRandomNetwork(std::vector<Node*> *population, Parameters *p);
};

#endif /* NETWORKFACTORY_H_ */
