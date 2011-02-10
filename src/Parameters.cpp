/*
 * Parameters.cpp
 *
 *  Created on: Jan 26, 2011
 *      Author: martin
 */

#include "Parameters.h"

#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

Parameters::Parameters(int randomSeed) {
	this->setSeed(randomSeed);
	rng.seed(randomSeed);
}

Parameters::~Parameters() {
	// TODO Auto-generated destructor stub
}

int Parameters::getCurrentIteration()  {
	return this->currentIteration;
}

void Parameters::setCurrentIteration(int currentIteration) {
	this->currentIteration = currentIteration;
}

double Parameters::getAcceptExchange()  {
	return acceptExchange;
}

double Parameters::getAcceptFriend() {
	return acceptFriend;
}

int Parameters::getCacheSize()  {
	return cacheSize;
}

double Parameters::getDirectUtil()  {
	return directUtil;
}

double Parameters::getDirectUtilContent()  {
	return directUtilContent;
}

double Parameters::getIndirectUtil(){
	return indirectUtil;
}

double Parameters::getIndirectUtilContent() {
	return indirectUtilContent;
}

int Parameters::getIterations() {
	return iterations;
}

double Parameters::getNeighbourProb() {
	return neighbourProb;
}

double Parameters::getNonFriendExchange()  {
	return nonFriendExchange;
}

int Parameters::getNumNodes() {
	return numNodes;
}

int Parameters::getNumTags() {
	return numTags;
}

int Parameters::getSeed(){
	return seed;
}

unsigned int Parameters::getUtilityMemoryWindow()  {
	return utilityMemoryWindow;
}

void Parameters::setAcceptExchange(double acceptExchange) {
	this->acceptExchange = acceptExchange;
}

void Parameters::setAcceptFriend(double acceptFriend) {
	this->acceptFriend = acceptFriend;
}

void Parameters::setCacheSize(int cacheSize) {
	this->cacheSize = cacheSize;
}

void Parameters::setDirectUtil(double directUtil) {
	this->directUtil = directUtil;
}

void Parameters::setDirectUtilContent(double directUtilContent) {
	this->directUtilContent = directUtilContent;
}

void Parameters::setIndirectUtil(double indirectUtil) {
	this->indirectUtil = indirectUtil;
}

void Parameters::setIndirectUtilContent(double indirectUtilContent) {
	this->indirectUtilContent = indirectUtilContent;
}

void Parameters::setIterations(int iterations) {
	this->iterations = iterations;
}

void Parameters::setNeighbourProb(double neighbourProb) {
	this->neighbourProb = neighbourProb;
}

void Parameters::setNonFriendExchange(double nonFriendExchange) {
	this->nonFriendExchange = nonFriendExchange;
}

void Parameters::setNumNodes(int numNodes) {
	this->numNodes = numNodes;
}

void Parameters::setNumTags(int numTags) {
	this->numTags = numTags;
}

void Parameters::setSeed(int seed) {
	this->seed = seed;
}

int Parameters::getNextRandomInt(int start, int end) {
	boost::uniform_int<> dist(start, end);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng,
			dist);
	return die();
}

double Parameters::getNextRandomDouble(double start, double end) {
	boost::uniform_real<> dist(start, end);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(rng,
			dist);
	return die();
}

void Parameters::setUtilityMemoryWindow(unsigned int utilityMemoryWindow) {
	this->utilityMemoryWindow = utilityMemoryWindow;
}

void Parameters::setDirectory(boost::filesystem::path directory) {
	this->working_directory = directory;
}

boost::filesystem::path Parameters::getDirectory() {
	return this->working_directory;
}

