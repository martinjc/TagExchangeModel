/*
 * Output.h
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "Parameters.h"
#include "Node.h"
#include "Tag.h"

#include <string>

class Output {
public:
	Output();
	virtual ~Output();
	static void out(std::string);
	static void out(float message);
	static void out(int message);
	static void out(double message);
	static void out(unsigned int message);
	static  void out(std::string text, int value);
	static void sdoWriteOutput(Parameters *p, std::vector<Node *> *population, std::vector<Tag *> *tag);
};

#endif /* OUTPUT_H_ */
