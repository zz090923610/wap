//
//  Step.h
//  mapTest
//
//  Created by 张 钊 on 9/9/13.
//
//

#ifndef __mapTest__Step__
#define __mapTest__Step__

#include <iostream>


class Step{

public:
	Step();
	Step(int start, int stop);
	void calcOrientation(double * oriList);
	
	int StepStart;
	int StepStop;
	double StepLength;
	int orientation;
	long timeStamp;
	int currentX,currentY;
	
};


#endif /* defined(__mapTest__Step__) */
