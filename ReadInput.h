/*
 * ReadInput.h
 *
 *  Created on: Feb 3, 2011
 *      Author: martin
 */

#ifndef READINPUT_H_
#define READINPUT_H_

#include <string>
#include "Parameters.h"

class Parameters;

class ReadInput {
public:
	ReadInput();
	virtual ~ReadInput();
	Parameters* readInputFromFile(const char*);
};

#endif /* READINPUT_H_ */
