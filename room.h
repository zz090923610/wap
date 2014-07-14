//
//  room.h
//  mapTest
//
//  Created by 张 钊 on 9/27/13.
//
//

#ifndef __mapTest__room__
#define __mapTest__room__

#include <string>
#include <iostream>
#include <list>
#include "accessPoint.h"

class room
{
public:
	room();
	room(std::string roomName, std::string BSSID, int north,int south, int east, int west,int doorStart, int doorEnd,int doorUp,int doorDown);
	std::string roomName;
	std::string BSSID;
	int north, south, east, west;
	int doorStart, doorEnd, doorUp, doorDown;
	std::list<accessPoint> myAP;
};


#endif /* defined(__mapTest__room__) */
