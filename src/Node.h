/*
 * Node.h
 *
 *  Created on: Jan 27, 2011
 *      Author: martin
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <map>
#include <string>
#include "Parameters.h"
#include "IterationData.h"

class Tag;

class Node
{

public:

  Node(Parameters *p);
  virtual
  ~Node();

  IterationData* calculateStatistics();
  double calculatePrecision();
  double calculateRecall();
  int calculateNumSatisfiedInterests();
  double calculateUtility(std::vector<Tag *> *cache);
  double getAverageUtility(Node *n);

  void receiveContent(Node *source, std::vector<Tag *> *cache);
  void addContentToCache(std::vector<Tag *> *receivedCache);
  std::vector<Tag *>* getCache();

  bool haveMet(Node *n);

  Tag* produceTag();
  bool isProducer();
  void setProducer(bool produces);
  bool providesTag(Tag *t);
  bool providesTag(Tag *t, std::vector<Node *> *providers);
  Tag* produces();
  bool produces(Tag *t);

  void cleanCache();

  void notifyTagRemoved(Tag *t, Node *source);
  void removeFriend(Node *n);

  bool interestedIn(Tag *t);
  bool indirectlyInterestedIn(Tag *t);

  void addInterest(Tag *t);
  std::vector<Tag *>* getInterests();

  void addFriend(Node * n);
  bool isFriend(Node *n);
  void addNetworkLink(Node *n);
  std::vector<Node *>* getFriends();
  std::vector<Node *>* getNetwork();
  std::vector<Node *>* getNotFriends();

  bool operator==(Node* n);

  int getID();


  std::string getDetailedString();


private:

    static int nodeCounter;

    int id;

    Parameters *p;

    bool producer;

    Tag *sourceOf;

    std::vector<Tag *> *cache;

    std::vector<Tag *> *interests;

    std::map<Node*, std::vector<Tag *>* > *itemsReceived;

    std::map<Node*, std::vector<double>* > *utilityHistory;

    std::vector<Node *> *friends;
    std::vector<Node *> *network;
    std::vector<Node *> *notfriends;

    std::map<Tag *, std::vector<Node *>* > *myProviders;

    std::vector<IterationData *> *iterationData;
};



#endif /* NODE_H_ */
