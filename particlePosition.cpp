//
//  particlePosition.cpp
//  mapTest
//
//  Created by 张 钊 on 9/18/13.
//
//

#include "particlePosition.h"


particlePosition::particlePosition()
{
	this->stepNum = 0;
	this->posX = 0;
	this->posY = 0;
	this->timeWhenStart = 0;
}


particlePosition::particlePosition(int stepNum, int posX, int posY, int timeWhenStart)
{
	this->stepNum = stepNum;
	this->posX = posX;
	this->posY = posY;
	this->timeWhenStart = timeWhenStart;

}