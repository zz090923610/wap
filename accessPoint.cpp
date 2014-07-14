//
//  accessPoint.cpp
//  mapTest
//
//  Created by 张 钊 on 9/11/13.
//
//

#include "accessPoint.h"


accessPoint::accessPoint()
{
	this->BSSID = "NULL";

	this->posX = 0;
	this->posY = 0;
	this->strength = 0;
	this->isSelected = false;
	this->refRSSI = 0;
	this->frequency = 0;
	this->timeStamp = 0;
	this->gamma = 0;
	this->moveStatus = UNDEFINEDSTATUS;
	this->statusStrength = 0;
}
accessPoint::accessPoint(const accessPoint & toCopy)
{
	this->BSSID = toCopy.BSSID;
	this->frequency = toCopy.frequency;
	this->posX = toCopy.posX;
	this->posY = toCopy.posY;
	this->refRSSI = toCopy.refRSSI;
	this->strength = toCopy.strength;
	this->timeStamp = toCopy.timeStamp;
	this->isSelected = toCopy.isSelected;
	this->gamma = toCopy.gamma;
	this->moveStatus = UNDEFINEDSTATUS;
	this->statusStrength = 0;
}

accessPoint:: accessPoint(std::string BSSID, int posX, int posY, double strength, double refRSSI, double gamma)
{
	this->BSSID = BSSID;
	this->posX = posX;
	this->posY = posY;
	this->strength = strength;
	this->refRSSI = refRSSI;
	this->isSelected = false;
	this->frequency = 0;
	this->timeStamp = 0;
	this->gamma = gamma;
	this->moveStatus = UNDEFINEDSTATUS;
	this->statusStrength = 0;
}

accessPoint::accessPoint(std::string BSSID, int frequency, double strength ,long timeStamp)
{
	this->BSSID = BSSID;
	this->posX = 0;
	this->posY = 0;
	this->strength = strength;
	this->refRSSI = 0;
	this->isSelected = false;
	this->frequency = 0;
	this->timeStamp = 0;
	this->gamma = 0;
	this->moveStatus = UNDEFINEDSTATUS;
	this->statusStrength = 0;

}