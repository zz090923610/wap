//
//  particle.cpp
//  mapTest
//
//  Created by 张 钊 on 9/18/13.
//
//

#include "particle.h"



particle::particle()
{
	this->particleId = 0;
	this->stepCount = 0;
	this->isAlive = true;
	this->wrongRoom = false;
	this->currentX = 0;
	this->currentY = 0;
	this->map = NULL;

}

particle:: particle(ImageSet * map,int particleId, int currentX, int currentY)
{
	this->particleId = particleId;
	this->isAlive = true;
	this->currentX = currentX;
	this->currentY = currentY;
	this->wrongRoom = false;
	this->stepCount = 0;
	this->map = map;
}


void particle:: updatePosition(double stepLength, double orientation, int timeStamp, int mainX, int mainY)
{
	//std::cout<<"step info "<<stepLength<<" "<<orientation<<"\n";
	if(this->isAlive == false)
	{
		//std::cout<<"\tparticle "<<this->particleId<<" is already dead\n#########################\n";
		return;
	}
	if(orientation < 0) orientation += 360;
	float nextX = currentX - std::sin((float)orientation / 180. * 3.14) * stepLength ;
	float nextY = currentY + std::cos((float)orientation / 180. * 3.14) * stepLength ;

	//std::cout<<"next pos of "<< this->particleId<<" is "<< nextX<<" "<< nextY<<" current is "<<this->currentX<<" "<<this->currentY<<"\n";
	this->stepCount ++;
	
	if(this->bresenhanLineHitCheck(nextX,nextY,this->currentX, this->currentY, this->map->grayMap) == true && this->bresenhanLineHitCheck(nextX,nextY,mainX, mainY, this->map->grayMap) == true)
	{
		//std::cout<<"\t\tparticle "<<this->particleId<<" dead now\n#########################\n;";
		this->isAlive = false;
	}
	else
	{
	this->currentX = nextX, this->currentY = nextY;
	positionList.push_back(particlePosition(this->stepCount, nextX, nextY, timeStamp));
	//std::cout<<"Pos updated to "<<this->currentX<<" "<<this->currentY<<"\n#########################\n";
	}

}

bool particle::checkWallHit(int  nextX, int nextY)
{
	return this->bresenhanLineHitCheck(this->currentX, this->currentY, nextX, nextY, this->map->grayMap);
}


bool particle::bresenhanLineHitCheck(  int pt1x, int pt1y, int pt2x, int pt2y, int ** data)
{
	if(data[pt2x][pt2y] < 255)
	{
		//std::cout<<"updating hit wall\n";
		
		return true;
	}
	if(pt1x == pt2x)
	{
		if(pt1y < pt2y)
		{
			for(int loop = pt1y; loop <= pt2y; loop ++)
				if(data[pt1x][loop] < 255) return true;
		}
		if(pt1y >= pt2y)
		{
			for(int loop = pt2y; loop <= pt1y; loop ++)
				if(data[pt1x][loop] < 255) return true;
		}
		
	}
	
	if(pt1y == pt2y)
	{
		if(pt1x < pt2x)
		{
			for(int loop = pt1x; loop <= pt2x; loop ++)
				if(data[loop][pt1y] < 255) return true;
		}
		if(pt1x >= pt2x)
		{
			for(int loop = pt2x; loop <= pt1x; loop ++)
				if(data[pt1x][loop] < 255) return true;
		}
		
	}
	
	
	//std::cout<<"check line "<<pt1x<<" "<<pt1y<<" "<<pt2x<<" "<<pt2y<<"\n";
	int dx = pt2x - pt1x, dy = pt2y - pt1y, steps, k;
	float xInc, yInc, x = pt1x, y = pt1y;
	
	if(std::fabs(dx)> std::fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xInc = float(dx) / float(steps);
	yInc = float(dy) / float(steps);
	
	if(data[(int)round(x)][(int)round(y)] < 255) return true;
	
	for(k = 0; k < steps; k++)
	{
		x += xInc;
		y += yInc;
		if(data[(int)round(x)][(int)round(y)] < 255) return true;
	}
	
	return false;

	
}







