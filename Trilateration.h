//
//  Trilateration.h
//  mapTest
//
//  Created by 张 钊 on 9/11/13.
//
//

#ifndef __mapTest__Trilateration__
#define __mapTest__Trilateration__

#include "accessPoint.h"
#include "point.h"

#include <iostream>

#include <list>
#include <fstream>
#include <iterator>
#include <cmath>
#include <gsl/gsl_randist.h>

class Trilateration
{
public:
	Trilateration();
	Trilateration(accessPoint * ap1, accessPoint * ap2, accessPoint * ap3, point * result);
	~Trilateration();

	double calcAPDistance(accessPoint * ap);
	void TrilaterationProcess();
	void MatrixMul(double ** mat1, double ** mat2, double ** mat3);
	void MatrixInverse(double ** mat1, double ** mat2);

	double ** A ;
	double ** AT;
	double ** ATA;
	double ** ATAi;
	
	accessPoint * ap1;
	accessPoint * ap2;
	accessPoint * ap3;
	point * result;
	
	
	double distance1;
	double distance2;
	double distance3;
	
};

#endif /* defined(__mapTest__Trilateration__) */
