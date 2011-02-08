/*
 * IterationData.h
 *
 *  Created on: Feb 1, 2011
 *      Author: martin
 */

#ifndef ITERATIONDATA_H_
#define ITERATIONDATA_H_

class IterationData {
public:

	IterationData(int iteration);
	virtual ~IterationData();

	int getIteration();
	int getSatisfied();
	double getPrecision();
	double getRecall();
	void setSatisfied(int satisfied);
	void setPrecision(double precision);
	void setRecall(double recall);

private:

	int iteration;
	int satisfied;
	double precision;
	double recall;
};

#endif /* ITERATIONDATA_H_ */
