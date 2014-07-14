//
//  wifiVector.cpp
//  mapTest
//
//  Created by 张 钊 on 9/29/13.
//
//

#include "wifiVector.h"

wifiVector::wifiVector()
{
	this->timeStamp = 0;
	this->isStable = false;
	this->orientation = 0;
}

wifiVector::wifiVector(long timeStamp, bool isStable, int orientation)
{
	this->timeStamp = timeStamp;
	this->isStable = isStable;
	this->orientation = orientation;
}