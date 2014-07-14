//
//  Step.cpp
//  mapTest
//
//  Created by 张 钊 on 9/9/13.
//
//

#include "Step.h"


Step::Step()
{
	this->StepStart = 0;
	this->StepStop = 0;
	this->StepLength = 0;
	this->orientation = 0;
	this->timeStamp = 0;
	this->currentX = 0, this->currentY = 0;
}
Step::Step(int start, int stop)
{
	this->StepStart = start;
	this->StepStop = stop;
	this->StepLength = 0;
	this->orientation = 0;
	this->timeStamp = 0;
	this->currentX = 0, this->currentY = 0;
}


void Step::calcOrientation(double *oriList)
{
	/*
	int aveOri = 0;
	int count = 0;
	for(int loop = this->StepStart; loop <= this->StepStop; loop ++)
	{
		aveOri += oriList[loop];
		count ++;
	}
	if(count != 0) aveOri /= count;
	this->orientation = aveOri;
	 */
	this->orientation = oriList[StepStart];
}