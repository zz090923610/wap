//
//  sensorData.cpp
//  mapTest
//
//  Created by 张 钊 on 9/8/13.
//
//

#include <math.h>
#include <cstdlib>
#include "sensorData.h"
sensorData::sensorData()
{
	this->column = 10;
	this->data = NULL;
	this->inFilePath = "NULL";
	this->outFilePath = "NULL";
	this->row = 0;
	this->compondAcce = NULL;
	this->varienceAcce = NULL;
	this->biggerThanThreshold = NULL;
	this->countTrue = 0;
	this->countFalse = 0;
	this->stepCount = 0;
	this->clusteringCenterUp = 0;
	this->clusteringCenterDown = 0;
	this->clusterUpCount = 0;
	this->clusterDownCount = 0;
	this->StepStart = 0;
	this->StepStop = 0;
}
sensorData::~sensorData()
{
	if(this->biggerThanThreshold != NULL) delete this->biggerThanThreshold;
	if(this->compondAcce != NULL) delete this->compondAcce;
	if(this->varienceAcce != NULL) delete this->varienceAcce;
	if (this->data != NULL)
	{
		for(int loop = 0; loop < this->row; loop ++)
			if(this->data[loop] != NULL)delete this->data[loop];
	}

}

void sensorData:: countRowNumber()
{
	std::ifstream fin;
	fin.open(inFilePath.c_str(),std::ios::in);
	if(!fin.is_open())
	{
		std::cout<< "Wrong Path\n";
		fin.clear();
	}
	while(!fin.eof())
	{
		double temp[this->column];
		for(int i = 0; i < this->column; i++)
		fin >> temp[i];
		row ++;
	}
	row -= 2;
	fin.clear();
	fin.close();
}

void sensorData:: readInData()
{
	data = new double *[row];
	this->compondAcce = new double [row];
	this->varienceAcce = new double [row];
	this->biggerThanThreshold = new bool[row];
	std::ifstream fin;
	fin.open(inFilePath.c_str(), std::ios::in);
	for(int loop = 0; loop < row; loop ++)
	{
		this->data[loop] = new double [this->column];
		for(int loopy = 0; loopy < this->column; loopy ++)
			fin>> this->data[loop][loopy];
	}
		
	fin.clear();
	fin.close();
	 

}

void sensorData:: linearFilter(int column, int time, int windowSize)
{
	int leftBoundary = 0, rightBoundary = row -1;
	
	for(int loop0 = 0 ; loop0 < row; loop0 ++)
	{
		double tempSum = 0;
		for(int loop1 = loop0 - windowSize/2; loop1 <= loop0 + windowSize /2; loop1 ++)
		{
			if(loop1 < leftBoundary || loop1 > rightBoundary) tempSum += data[loop0][column];
			else tempSum += data[loop1][column];
		}
		data[loop0][column] = tempSum / windowSize;
		//if(windowSize != 0)
		//else std::cerr<< "divide by zero";
	}
	
	if(time > 0) this->linearFilter(column, time - 1 ,windowSize);
	else return;
}

void sensorData::linearFilter(double * data, int time, int windowSize)
{
	int leftBoundary = 0, rightBoundary = row -1;
	
	for(int loop0 = 0 ; loop0 < row; loop0 ++)
	{
		double tempSum = 0;
		for(int loop1 = loop0 - windowSize/2; loop1 <= loop0 + windowSize /2; loop1 ++)
		{
			if(loop1 < leftBoundary || loop1 > rightBoundary) tempSum += data[loop0];
			else tempSum += data[loop1];
		}
		data[loop0] = tempSum / windowSize;
		//if(windowSize != 0)
		//else std::cerr<< "divide by zero";
	}
		if(time > 0) this->linearFilter(data, time - 1 ,windowSize);
	else return;
}
void sensorData:: writeOut()
{
	std::ofstream fout;
	fout.open(outFilePath.c_str(), std::ios::out);
	for(int loopx = 0; loopx < row; loopx ++)
	{
		for(int loopy = 0; loopy < this->column; loopy ++)
		fout<< data[loopx][loopy]<<" ";
		fout <<std::endl;
	}
	fout.clear();
	fout.close();
}


void sensorData::calcCompondAcce()
{
	for(int loop = 0; loop < this->row; loop ++)
	{
		this->compondAcce[loop] = sqrt(this->data[loop][1]*this->data[loop][1] + this->data[loop][2]*this->data[loop][2] + this->data[loop][3]*this->data[loop][3]);
		//this->compondAcce[loop] = sqrt(this->data[loop][1]*this->data[loop][1] + this->data[loop][2]*this->data[loop][2] + this->data[loop][3]*this->data[loop][3]);
		//this->compondAcce[loop] = abs(this->data[loop][3]);
	}

}

void sensorData::calcVarience(int windowSize)
{
	for(int i = 0; i < this->row; i ++)
	{
		this->varienceAcce[i] = 0;
		for(int j = i - windowSize; j <= i+ windowSize; j++)
		{
			if(j < 0 || j >= this->row)
				this->varienceAcce[i] +=(this->compondAcce[i] - this->calcAveLocalAcce(i, windowSize));
			else
				this->varienceAcce[i] += (this->compondAcce[j] - this->calcAveLocalAcce(j, windowSize));
		}
		this->varienceAcce[i] /= (2 * windowSize + 1);
	}
	
}


double sensorData:: calcAveLocalAcce(int j, int windowSize)
{
	double aveLocalAcce = 0;
	for(int loop = j - windowSize; loop <= j + windowSize; loop ++)
	{
		if(loop < 0 || loop >= this->row)
			aveLocalAcce += this->compondAcce[j];
		else
		aveLocalAcce += this->compondAcce[loop];
	}
	aveLocalAcce /= (2 * windowSize + 1);
	return aveLocalAcce;
}


void sensorData::clustering(double threshold)
{
	for(int loop = 0; loop < this->row; loop ++)
	{
		if(this->varienceAcce[loop] > threshold)
		this->biggerThanThreshold[loop] = true;
		else
			this->biggerThanThreshold[loop] = false;
	}
}


void sensorData::dynamicClustering(double ceiling, double floor)
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++\nceiling $ floor "<<ceiling<<" "<<floor<<"\n";
	this->clusteringCenterUp = ceiling;
	this->clusteringCenterDown = floor;
	this->clusterUpCount  = 1;
	this->clusterDownCount = 1;
	int ave = 0;
	for(int loop = 0; loop < this->row; loop ++)
	{
		this->varienceAcce[loop] *= 1000;
		if(abs(this->clusteringCenterUp - this->varienceAcce[loop]) < abs(this->varienceAcce[loop] - this->clusteringCenterDown))
		{
			this->biggerThanThreshold[loop] = true;
			//this->clusteringCenterUp = (this->clusteringCenterUp * this->clusterUpCount + this->varienceAcce[loop]) / (this->clusterUpCount + 1);
			this->clusterUpCount ++;
			//std::cout<< "center up "<<this->clusteringCenterUp<<" "<< this->clusterUpCount <<std::endl;
		}
		if(abs(this->clusteringCenterUp - this->varienceAcce[loop]) > abs(this->varienceAcce[loop] - this->clusteringCenterDown))
		{
			this->biggerThanThreshold[loop] = false;
			//this->clusteringCenterDown = (this->clusteringCenterDown * this->clusterDownCount + this->varienceAcce[loop])/ (this->clusterDownCount + 1);
			this->clusterDownCount ++;
			//std::cout<< "center down "<<this->clusteringCenterDown<<" "<< this->clusterDownCount <<std::endl;
		}
		
	}
	std::cout<<"cluster points count "<<this->clusterUpCount << " "<< this->clusterDownCount<<"\n";
	this->clusteringCenterUp = 0;
	this->clusteringCenterDown = 0;
	for(int loop = 0; loop < this->row; loop ++)
	{
		ave += this ->varienceAcce[loop];
		if(this->biggerThanThreshold[loop] == true) this->clusteringCenterUp += this->varienceAcce[loop];
		else this->clusteringCenterDown += this->varienceAcce[loop];
		
	}
	ave /= this->row;
	std::cout<<"ave "<<ave<<std::endl;
	this->clusteringCenterUp /= (this->clusterUpCount + 1);
	this->clusteringCenterDown /= (this->clusterDownCount + 1);
	if(abs(this->clusteringCenterUp - ceiling) > 0.0001 || abs(this->clusteringCenterDown - floor) > 0.0001) dynamicClustering(this->clusteringCenterUp, this->clusteringCenterDown);
	else return;
}


void sensorData::findStep(int threshold)
{
	
	int procStatus = 0;
	int count1 = 0;
	int count2 = 0;
	this->StepStart = 0;
	bool * change = new bool [row];
	for(int loop = 1; loop < this->row; loop ++)
	{
		
		if(this->biggerThanThreshold [loop] == this->biggerThanThreshold [loop - 1]) change[loop] = false;
		else change[loop] = true;
	}
	for(int loop = 1; loop < this->row; loop ++)
	{
		if(change[loop] == true )
		{
			if(procStatus == 0)
			{
				this->StepStart = loop;
				procStatus = 1;
				count1 = loop;
			}
			else if(procStatus == 1)
			{
				count1 = loop - count1;
				count2 = loop;
				procStatus = 2;
			}
			else if(procStatus == 2)
			{
				count2 = loop - count2;
				this->StepStop = loop - 1;
				procStatus = 0;
				loop --;
				if(count1 >= threshold && count2 >= threshold)
				{
					this->stepCount ++;
					//std::cout<< "a step is detected "<<StepStart <<" "<<StepStop <<std::endl;
					Step temp (StepStart, StepStop);
					this->StepData.push_back(temp);
				}
			}
			
		}
		
	}
}


void sensorData::calcStepLength()
{
	std::list<Step> :: iterator loop;
	for(loop = this->StepData.begin(); loop != this -> StepData.end(); loop ++)
	{
		for(int loop2 = loop->StepStart; loop2 <= loop ->StepStop; loop2 ++)
			loop->StepLength += abs(this->compondAcce [loop2]);
		

		loop -> StepLength /= (loop->StepStop - loop -> StepStart + 1);
		loop -> StepLength = pow (loop -> StepLength, 1.0 / 3);
		loop -> StepLength *= 0.33;
			
	}


}



			



