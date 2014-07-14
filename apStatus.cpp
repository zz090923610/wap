//
//  apStatus.cpp
//  mapTest
//
//  Created by 张 钊 on 9/23/13.
//
//

#include "apStatus.h"


apStatus:: apStatus()
{
	this->timeStamp = 0;
}

apStatus::apStatus(long timeStamp)
{
	this->timeStamp = timeStamp;
}