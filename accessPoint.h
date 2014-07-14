//
//  accessPoint.h
//  mapTest
//
//  Created by 张 钊 on 9/11/13.
//
//

#ifndef __mapTest__accessPoint__
#define __mapTest__accessPoint__

#include "Constants.h"
#include <iostream>
#include <string>

class accessPoint
{
public:
	accessPoint();
	accessPoint(const accessPoint& toCopy);
	accessPoint(std::string BSSID, int frequency, double strength ,long timeStamp);
	accessPoint(std::string BSSID,  int posX, int posY, double strength, double refRSSI, double gamma);
	
	
	std::string BSSID;
	int posX, posY;
	double gamma;
	double strength;
	double refRSSI;
	long timeStamp;
	int frequency;
	bool isSelected;
	int moveStatus;
	double statusStrength;
	
};

#endif /* defined(__mapTest__accessPoint__) */
