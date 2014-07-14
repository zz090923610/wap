//
//  particle.h
//  mapTest
//
//  Created by 张 钊 on 9/18/13.
//
//

#ifndef __mapTest__particle__
#define __mapTest__particle__

#include "particlePosition.h"
#include "ImageSet.h"


#include <iostream>
#include <list>



class particle
{
public:
	particle();
	particle(ImageSet * map ,int particleId, int currentX, int currentY);
	void updatePosition(double stepLength, double orientation, int  timeStamp, int mainX, int mainY);
	bool checkWallHit(int nextX, int nextY);
	bool bresenhanLineHitCheck(  int pt1x, int pt1y, int pt2x, int pt2y, int ** data);
	
	ImageSet * map;
	int particleId;
	int stepCount;
	int currentX, currentY;
	bool isAlive;
	bool wrongRoom;
	std::list<particlePosition> positionList;
	
};




#endif /* defined(__mapTest__particle__) */
