//
//  HDEController.h
//  mapTest
//
//  Created by 张 钊 on 9/10/13.
//
//

#ifndef __mapTest__HDEController__
#define __mapTest__HDEController__

#include <iostream>


class HDEController {
	
public:
	HDEController();
	
	double HDEProcess(double orientation, double gyro);
	double IController(double Ibefore, double Ic, double Ei);
	double integrator(double IiPulsEpsilon, double deltaTime);
	double MOD(double phiISub1, double delta);
	void reset();
	
	double IControllerOut;
	double Ic;
	double deltaTime;
	double delta;
};

#endif /* defined(__mapTest__HDEController__) */
