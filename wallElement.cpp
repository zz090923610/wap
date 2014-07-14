#include "wallElement.h"


wallElement::wallElement()
{
	this->topLeftX = 0;
	this->topLeftY = 0;
	this->downRightX = 0;
	this->downRightY = 0;
	
}
wallElement::wallElement(int tlX,int tlY, int drX, int drY)
{
	this->topLeftX = tlX;
	this->topLeftY = tlY;
	this->downRightX = drX;
	this->downRightY = drY;
	
}