//
//  sensorData.h
//  mapTest
//
//  Created by 张 钊 on 9/8/13.
//
//

#ifndef __mapTest__sensorData__
#define __mapTest__sensorData__

#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include "Step.h"
#ifndef __DELTA_TIME__
#define __DELTA_TIME__

#define DELTATIME   0.025
#endif
class sensorData{
public:
	sensorData();
	~sensorData();
	void countRowNumber();
	void readInData();
	void writeOut();
	
	///////////////Step detection////////////////
	
	void linearFilter(int column, int time, int windowSize);
	void linearFilter(double * data, int time, int windowSize);
	
	void calcCompondAcce();
	void calcVarience(int windowSize);
	double calcAveLocalAcce(int j, int windowSize);
	void clustering(double threshold);
	void dynamicClustering(double ceiling, double floor);
	void findStep(int threshold);
	
	///////////Step length estimation//////////
	
	void calcStepLength();
	
	
	/////////Step Orientation estimation//////
	
	
	
	
	
	int row;
	int column;
	int countTrue;
	int countFalse;
	int stepCount;
	std::string  inFilePath;
	std::string outFilePath;
	
	double clusteringCenterUp;
	double clusteringCenterDown;
	int clusterUpCount;
	int clusterDownCount;
	
	int StepStart;
	int StepStop;
	
	double ** data;
	
	double * compondAcce;
	double * varienceAcce;
	bool * biggerThanThreshold;
	std::list <Step> StepData;
};
#endif /* defined(__mapTest__sensorData__) */




