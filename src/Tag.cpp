/*
 * Tag.cpp
 *
 *  Created on: Jan 27, 2011
 *      Author: martin
 */

#include "Tag.h"
#include "Parameters.h"

int Tag::tagCounter = 0;
std::string cols[] = {"666666","333333","FFCC00","FF9900","FF6600","FF3300","99CC00","CC9900","333300","666600","999900","CCCC00","FFFF00","CC9933","CC6633","330000","660000","990000","CC0000","FF0000","FF3366","FF0033","99FF00","CCFF66","99CC33","666633","999933","CCCC33","FFFF33","996600","993300","663333","993333","CC3333","FF3333","CC3366","FF6699","FF0066","66FF00","99FF66","66CC33","669900","999966","CCCC66","FFFF66","996633","663300","996666","CC6666","FF6666","990033","CC3399","FF66CC","FF0099","33FF00","66FF33","339900","66CC00","99FF33","CCCC99","FFFF99","CC9966","CC6600","CC9999","FF9999","FF3399","CC0066","990066","FF33CC","FF00CC","00CC00","33CC00","336600","669933","99CC66","CCFF99","FFFFCC","FFCC99","FF9933","FFCCCC","FF99CC","CC6699","993366","660033","CC0099","330033","33CC33","66CC66","00FF00","33FF33","66FF66","99FF99","CCFFCC","CC99CC","996699","993399","990099","663366","660066","006600","336633","009900","339933","669966","99CC99","FFCCFF","FF99FF","FF66FF","FF33FF","FF00FF","CC66CC","CC33CC","003300","00CC33","006633","339966","66CC99","99FFCC","CCFFFF","3399FF","99CCFF","CCCCFF","CC99FF","9966CC","663399","330066","9900CC","CC00CC","00FF33","33FF66","009933","00CC66","33FF99","99FFFF","99CCCC","0066CC","6699CC","9999FF","9999CC","9933FF","6600CC","660099","CC33FF","CC00FF","00FF66","66FF99","33CC66","009966","66FFFF","66CCCC","669999","003366","336699","6666FF","6666CC","666699","330099","9933CC","CC66FF","9900FF","00FF99","66FFCC","33CC99","33FFFF","33CCCC","339999","336666","006699","003399","3333FF","3333CC","333399","333366","6633CC","9966FF","6600FF","00FFCC","33FFCC","00FFFF","00CCCC","009999","006666","003333","3399CC","3366CC","0000FF","0000CC","000099","000066","000033","6633FF","3300FF","00CC99","0099CC","33CCFF","66CCFF","6699FF","3366FF","0033CC","3300CC","00CCFF","0099FF","0066FF","0033FF"};
std::vector<std::string> *Tag::colours = new std::vector<std::string>(cols, cols+sizeof(cols)/sizeof(std::string));

Tag::Tag(Node* source, Parameters *p) {
	this->source = source;
	this->id = Tag::tagCounter++;
	int randomColour = p->getNextRandomInt(0, colours->size()-1);
	this->colour = colours->at(randomColour);
	colours->erase(colours->begin() + randomColour);
}

Tag::~Tag() {
	// TODO Auto-generated destructor stub
}

bool Tag::Tag::operator ==(Tag* t) {
	return this->getId() == t->getId();
}

int Tag::getId() const {
	return id;
}

void Tag::setId(int id) {
	this->id = id;
}

std::string Tag::getColour() {
	return this->colour;
}
