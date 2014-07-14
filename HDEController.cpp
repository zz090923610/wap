//
//  HDEController.cpp
//  mapTest
//
//  Created by 张 钊 on 9/10/13.
//
//

#include "HDEController.h"


HDEController::HDEController()
{
	this->IControllerOut = 0;
	this->delta = 90;
	this->Ic = 0.001;
	this->deltaTime = 0.025;
}

double HDEController::HDEProcess(double orientation, double gyro)
{
	
	double epsilon = gyro * 180 /3.14;

	std::cout<<"epsilon "<<epsilon<<"\t";
	double psi = (this->IControllerOut + epsilon) * this->deltaTime;
	std::cout<<"psi "<<psi<<"\t";
	double psiStar =psi - delta * (int)(psi / delta);
	std::cout<<"psiStar "<<psiStar<<"\t";
	double Ei =   psiStar;
	std::cout<<"Ei "<<Ei<<"\t";
	this->IControllerOut = this->IController(this->IControllerOut, this->Ic, Ei);
	std::cout<<"New Orientation "<<orientation + psi<<"\n";
	return orientation + psi;
}

double HDEController::IController(double Ibefore, double Ic, double Ei)
{
	if(Ei < 0) return  Ibefore - Ic;
	if(Ei == 0) return Ibefore;
	if(Ei > 0) return Ibefore + Ic;
	return 0;
}

double HDEController::integrator(double IiPulsEpsilon, double deltaTime)
{
	return IiPulsEpsilon * deltaTime;
	
}

double HDEController::MOD(double phiISub1, double delta)
{
	return phiISub1 - delta * (int)(phiISub1 / delta);
}


void HDEController::reset()
{
	this->IControllerOut = 0;
	
	this->delta = 90;

	this->Ic = 0.001;
	this->deltaTime = 0.025;
}


