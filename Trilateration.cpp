//
//  Trilateration.cpp
//  mapTest
//
//  Created by 张 钊 on 9/11/13.
//
//

#include "Trilateration.h"
#define N 0


Trilateration::Trilateration()
{

	
	this->A = new double *[3];
	this->AT = new double *[3];
	this->ATA = new double *[3];
	this->ATAi = new double *[3];
	for(int loop = 0; loop < 3; loop ++)
	{
		this->A[loop]  = new double [3];
		this->AT[loop]  = new double [3];
		this->ATA[loop]  = new double [3];
		this->ATAi[loop]  = new double [3];
	}
	this->ap1 = NULL;
	this->ap2 = NULL;
	this->ap3 = NULL;
	this->distance1 = 0;
	this->distance2 = 0;
	this->distance3 = 0;
	this->result = NULL;

}

Trilateration::Trilateration(accessPoint * ap1, accessPoint * ap2, accessPoint * ap3, point * result)
{
	this->A = new double *[3];
	this->AT = new double *[3];
	this->ATA = new double *[3];
	this->ATAi = new double *[3];
	for(int loop = 0; loop < 3; loop ++)
	{
		this->A[loop]  = new double [3];
		this->AT[loop]  = new double [3];
		this->ATA[loop]  = new double [3];
		this->ATAi[loop]  = new double [3];
	}
	this->ap1 = new accessPoint (*ap1);
	this->ap2 = new accessPoint (*ap2);
	this->ap3 = new accessPoint (*ap3);
	this->distance1 = calcAPDistance(ap1);
	this->distance2 = calcAPDistance(ap2);
	this->distance3 = calcAPDistance(ap3);
	this->result = result;
}

Trilateration::~Trilateration()
{
	for(int loop = 0; loop < 3; loop ++)
	{
		delete this->A[loop];
		delete this->AT[loop];
		delete this->ATA[loop];
		delete this->ATAi[loop];
	}
	delete this->A;
	delete this->AT;
	delete this->ATA;
	delete this->ATAi;
	
	delete this->ap1;
	delete this->ap2;
	delete this->ap3 ;
	

}


double Trilateration::calcAPDistance(accessPoint * ap)
{
	double distance = 0;
	double exp = (ap->refRSSI - ap->strength - N ) / (10 * ap->gamma) ;
	distance = pow(10, exp);
//	std::cout<<"distance "<<distance<<"\n";
	return distance;
}


void Trilateration::TrilaterationProcess()
{
	
	
	double p1x = ap1->posX, p1y = ap1 ->posY;
	double p2x = ap2->posX, p2y = ap2->posY;
	double p3x = ap3->posX, p3y = ap3->posY;
	p1x /= 100,p1y /= 100,p2x /= 100,p2y /= 100,p3x /= 100,p3y /= 100;
	std::cout<<"p1 "<< p1x<<" "<<p1y<<" p2 "<< p2x<<" "<<p2y<< " p3 "<<p3x<<" "<<p3y<<"\n";
	double exx = 0, exy = 0;
	double eyx = 0, eyy = 0;
	double d = 0;
	double i = 0;
	double j = 0;
	double x = 0;
	double y= 0;
	exx = (p2x - p1x)/ std::sqrt((p2x - p1x)*(p2x - p1x) + (p2y - p1y)*(p2y - p1y));
	exy = (p2y - p1y)/ std::sqrt((p2x - p1x)*(p2x - p1x) + (p2y - p1y)*(p2y - p1y));
	
	i = exx *(p3x - p1x) + exy *(p3y - p1y);
	
	eyx = (p3x - p1x - i * exx) /std::sqrt((p3x - p1x - i * exx)*(p3x - p1x - i * exx) + (p3y - p1y - i * exy)*(p3y - p1y - i * exy));
	eyy = (p3y - p1y - i * exy) /std::sqrt((p3x - p1x - i * exx)*(p3x - p1x - i * exx) + (p3y - p1y - i * exy)*(p3y - p1y - i * exy));
	
	d = std::sqrt((p2x - p1x)*(p2x - p1x) + (p2y - p1y)*(p2y - p1y));
	j = eyx *(p3x - p1x) +eyy * (p3y - p1y);
	
	x = (distance1 * distance1 - distance2 * distance2 + d * d)/ (2 * d);
	y = (distance1 * distance1 - distance3 * distance3 + i * i + j * j)/ (2 * j) - i * x / j;
	
	double finalX = p1x + x * exx + y * eyx;
	double finalY = p1y + y * eyy + x * exy;
	this->result->x = (int)(finalX * 100);
	this->result->y = (int)(finalY * 100);
	if(ap1->BSSID != "NULL")std::cout<<"distance to "<<ap1->BSSID <<" is "<< distance1<<"\n";
	if(ap2->BSSID != "NULL")std::cout<<"distance to "<<ap2->BSSID <<" is "<< distance2<<"\n";
	if(ap3->BSSID != "NULL")std::cout<<"distance to "<<ap3->BSSID <<" is "<< distance3<<"\n";
	std::cout<<finalX <<" "<<finalY <<"\n";//<< exx<<" "<<exy<<" "<< eyx<<" "<<eyy<<" "<<d<<" "<<i<<" "<<j<<"\n";
	
	

}

void Trilateration::MatrixMul(double **mat1, double **mat2, double ** mat3)
{
	mat3[0][0] = mat1[0][0] * mat2[0][0] + mat1[0][1] * mat2[1][0] + mat1[0][2] * mat2[2][0];
	mat3[0][1] = mat1[0][0] * mat2[0][1] + mat1[0][1] * mat2[1][1] + mat1[0][2] * mat2[2][1];
	mat3[0][2] = mat1[0][0] * mat2[0][2] + mat1[0][1] * mat2[1][2] + mat1[0][2] * mat2[2][2];
	
	mat3[1][0] = mat1[1][0] * mat2[0][0] + mat1[1][1] * mat2[1][0] + mat1[1][2] * mat2[2][0];
	mat3[1][1] = mat1[1][0] * mat2[0][1] + mat1[1][1] * mat2[1][1] + mat1[1][2] * mat2[2][1];
	mat3[1][2] = mat1[1][0] * mat2[0][2] + mat1[1][1] * mat2[1][2] + mat1[1][2] * mat2[2][2];

	mat3[2][0] = mat1[2][0] * mat2[0][0] + mat1[2][1] * mat2[1][0] + mat1[2][2] * mat2[2][0];
	mat3[2][1] = mat1[2][0] * mat2[0][1] + mat1[2][1] * mat2[1][1] + mat1[2][2] * mat2[2][1];
	mat3[2][2] = mat1[2][0] * mat2[0][2] + mat1[2][1] * mat2[1][2] + mat1[2][2] * mat2[2][2];
}

void Trilateration::MatrixInverse(double **mat1, double **mat2)
{
	for(int loopx = 0; loopx < 3; loopx ++)
	{
		for(int loopy = 0; loopy < 3; loopy ++)
		{
			mat2[loopx][loopy] = mat1[loopy][loopx];
		}
	}
}







