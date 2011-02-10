/*
 * IterationData.cpp
 *
 *  Created on: Feb 1, 2011
 *      Author: martin
 */

#include "IterationData.h"

IterationData::IterationData(int iteration)
{
	this->iteration = iteration;
}

IterationData::~IterationData() {
	// TODO Auto-generated destructor stub
}

void IterationData::setSatisfied(int satisfied)
{
	this->satisfied = satisfied;
}



int IterationData::getIteration()
{
	return this->iteration;
}



double IterationData::getRecall()
{
	return this->recall;
}



int IterationData::getSatisfied()
{
	return this->satisfied;
}



void IterationData::setPrecision(double precision)
{
	this->precision = precision;
}



void IterationData::setRecall(double recall)
{
	this->recall = recall;
}



double IterationData::getPrecision()
{
	return this->precision;
}


