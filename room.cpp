//
//  room.cpp
//  mapTest
//
//  Created by 张 钊 on 9/27/13.
//
//

#include "room.h"

room::room()
{
	this->roomName = "NULL";
	this->BSSID = "NULL";
	this->north = 0, this->south = 0, this->east = 0, this->west = 0;
	this->doorStart = 0, this->doorEnd = 0,this->doorUp = 0, this->doorDown = 0;
	
}

room::room(std::string roomName, std::string BSSID, int north,int south, int east, int west,int doorStart, int doorEnd, int doorUp, int doorDown)
{
	this->roomName = roomName;
	this->BSSID = BSSID;
	this->north = north;
	this->south = south;
	this->east = east;
	this->west = west;
	this->doorStart = doorStart, this->doorEnd = doorEnd, this->doorUp = doorUp, this->doorDown = doorDown;
}