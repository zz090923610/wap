//
//  apStatus.h
//  mapTest
//
//  Created by 张 钊 on 9/23/13.
//
//

#ifndef __mapTest__apStatus__
#define __mapTest__apStatus__

#include "accessPoint.h"
#include <iostream>
#include <list>

class apStatus
{
public:
	
	apStatus();
	apStatus(long timeStamp);
	long timeStamp;
	std::list<accessPoint> apList;
	

};


#endif /* defined(__mapTest__apStatus__) */
