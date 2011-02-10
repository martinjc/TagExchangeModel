/*
 * Tag.h
 *
 *  Created on: Jan 27, 2011
 *      Author: martin
 */

#ifndef TAG_H_
#define TAG_H_

#include <string>
#include <vector>
#include "Parameters.h"

class Node;

class Tag {

public:
	Tag(Node *, Parameters *);
	virtual
	~Tag();
	bool operator==(Tag* t);
    int getId() const;
    Node* getSource() const;
    void setId(int id);
    void setSource(Node *);
    std::string getColour();

private:

	int id;
	Node *source;
	std::string colour;
	static int tagCounter;
	static std::vector<std::string> *colours;
};



#endif /* TAG_H_ */
