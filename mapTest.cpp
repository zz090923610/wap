//main function part, for test and debug only
//just initialize and use the object you need to test and choose the function you want to use


#include "ImageSet.h"
#include "sensorData.h"
#include "HDEController.h"
#include "particle.h"
#include "apStatus.h"
#include "Constants.h"
#include "pixel.h"
#include "point.h"
#include "Trilateration.h"
#include "room.h"
#include "wifiVector.h"
#include "ProbTableElement.h"
#include <sstream>
#include <iterator>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


int particleCount = 0;
long particleID = 0;
int startUpX = 0;
int startUpY = 0;
int currentX = 0, currentY = 0;
float nextX = 0, nextY = 0;
int ori = 0;
int stepLength = 0;
int stepCount =0;
long timeStampStart = 0;
long timeStampStop = 0;
bool specialStatus = false;
point* WifiResult = new point(0,0);
Constants params;


std::list<particle> particleList;
std::list<pixel> pixelList;
std::list<pixel> pointTrace;
std::list<pixel> centerPointTrace;
std::list<accessPoint> AllowedList;
std::list<accessPoint> allApList;
std::list<apStatus> apStatusList;
std::list<room> roomList;
std::list<wifiVector> wifiVectorList;
std::list<ProbTableElement> * problist = NULL;

void addPointToCanvas(ImageSet * map, int x, int y);
void loadStartPoint(std::string path);
//////////////////// IMU PART ////////////////////////////////////

double calcOrientation(double orientation, double gyro, double deltaTime);

// TODO mag ori function defined here
double calcOrientationMag(double origination, double magX, double magY, double magZ);

//////////////////// PF  PART ////////////////////////////////////
int countLiveParticles();
void addEnoughParticlesFirstTime(ImageSet * map);
void addEnoughParticles(ImageSet * map);


void updateParticlesPosition(ImageSet * map,std::list<particle> * particleList, double stepLength, int ori, long timeStamp);
void updatePositionCoordination(ImageSet * map);
void addPosAndParticlesToCanvas(int currentX, int currentY, std::list<particle> * particleList, ImageSet * map);
void addTraceLineToCanvas(ImageSet * map);
void cleanPixelsFromCanvas(ImageSet * map);
bool bresenhanLineHitCheck( int pt1x, int pt1y, int pt2x, int pt2y, ImageSet * map, std::string doorOrWall);
void generateProbTable();

//////////////////// WIFI PART ////////////////////////////////////
void loadAllowedList();
void loadRoomList();
void loadWifiInfo(std::string wifiInfoPath);
bool isInAllowedList(accessPoint * toCheck);
void generateApStatus();
void wifiVectorAnalysis(long timeStamp);
void addWifiAPPosToCanvas(ImageSet * map);
std::string wifiRoomDiff(long timeStamp);
int doorCheckAndCorrection(long timeStamp, ImageSet * map, int curX, int curY, int nexX, int nexY);
std::string checkStepIntoWhichRoom(ImageSet * map, int curX, int curY, int nexX, int nexY);
void WIFIPositioningProc(ImageSet * map);
void KCDCMultilateration(apStatus * toCalc);
point * trilaterationProcess(accessPoint * a1, accessPoint * a2, accessPoint * a3);
void checkIfIsStable();
//////////////////// ADDITIONAL PART ////////////////////////////////////
void calc_last_distance();
void drawLine(int pt1x, int pt1y, int pt2x, int pt2y, ImageSet * map);





//////////////////// MAIN ENTERANCE ///////////////////////////////
int main (int argc, const char * argv [])
{
	/////////////////////parameter handler/////////////////
	if(argc == 1)
	{
		std::cout<<"you need to give the sensor data path and wifi data path(optional)\n";
		return 0;
	}
	std::string sensorPath = "";
	std::string wifiPath = "";
	if(argc == 2)
	{
		sensorPath = argv[1];
	}
	if(argc == 3)
	{
		sensorPath = argv[1];
		wifiPath = argv[2];
	}
	if(argc == 4)
	{
		sensorPath = argv[1];
		wifiPath = argv[2];
		loadStartPoint(argv[3]);
	}
	if(argc > 4)
	{
		std::cout<<"too many parameters\n";
		return 0;
	}
	//////////////end of parameter handler/////////////////
	
	
	/////////////////////MAP LOAD PART//////////////////////////

	std::cout<<"######## Hello, now is MAP LOAD PART ############\n";
	ImageSet map;
	map.loadMap(params.map_file_path);
	map.loadDoorPosMap(params.room_list_path);
	for(int i = 0; i < map.wallNumber; i++)		std::cout<<map.mapData[i].topLeftX<<" "<<map.mapData[i].topLeftY<<" "<<map.mapData[i].downRightX<<" "<<map.mapData[i].downRightY<<"\n";
	map.loadCanvas(map.height, map.width,CV_8UC1);
	std::cout<<map.height<<" "<<map.width<<std::endl;
	map.drawMap();
	std::cout<<"Map size is "<<map.width<<" X "<<map.height<<"\n";
	std::cout<<"######## END OF MAP LOAD PART ############\n";
	///////////////////// END OF MAP LOAD PART//////////////////////////
	
	
	
	//////////////////WIFI SENSOR PART //////////////////////////////
	
	loadAllowedList();
	loadRoomList();
	loadWifiInfo(wifiPath);
	generateApStatus();
	//WIFIPositioningProc(&map);
	addWifiAPPosToCanvas(&map);
	{
		std::list<apStatus>:: iterator it;
		for(it = apStatusList.begin(); it != apStatusList.end(); it++)
		{
			wifiVectorAnalysis(it->timeStamp);
		}
	}
	checkIfIsStable();
	///////add a point to canvas
	/*CvScalar iter;
	 
	 iter.val[0] = 20;
	 iter.val[1] = 155, iter.val[2] = 155, iter.val[3] = 0;
	 int x = 2513,y = 1300;
	 for(int loopx = -5; loopx < 5; loopx ++)
	 {
	 for(int loopy = -5; loopy < 5; loopy ++)
	 {
	 if(x + loopx >= 0 && x + loopx < map.height && y + loopy >= 0 && y + loopy < map.width)
	 {
	 cvSet2D(map.canvas, x + loopx, y + loopy, iter);
	 }
	 }
	 }*/
	{
		cv::Mat  toShow = map.canvas;
		cv::Mat resized(map.height / params.zoom_scale * 2,map.width / params.zoom_scale * 2,CV_32F);
		cv::resize(toShow,resized,resized.size());
		imshow("Map B 3 ", resized);
		cv::waitKey(100);
	}
	
	//////////////////END OF WIFI SENSOR PART////////////////////////
	

	
	
	/////////////////////IMU PART//////////////////////////
	sensorData asensordata;
	asensordata.inFilePath = sensorPath;
	asensordata.countRowNumber();
	asensordata.readInData();
	asensordata.calcCompondAcce();
	std::cout<<"Compond Acceleration\n";
	asensordata.calcVarience(7);
	/*for(int loop = 0; loop < asensordata.row; loop ++)
	{
		std::cout<<asensordata.varienceAcce[loop]<<"\n";
	}*/
	asensordata.linearFilter(asensordata.varienceAcce, 50, 3);
	std::cout<<"After filter process\n";
	/*for(int loop = 0; loop < asensordata.row; loop ++)
	{
		std::cout<<asensordata.varienceAcce[loop]<<"\n";
	}
	*/
	asensordata.clustering(0);
	std::cout<<"After clustering process\n";
	/*
	for(int loop = 0; loop < asensordata.row; loop ++)
	{
		std::cout<<asensordata.biggerThanThreshold[loop]<<"\n";
	}*/
	asensordata.findStep(7);
	asensordata.calcStepLength();
	std::cout<< asensordata.stepCount<<std::endl;
	std::list<Step>::iterator loop;
//	HDEController aHDEController;
	double * orientation = new double [asensordata.row];
	orientation[0] = ori;
	for(int loop = 1; loop < asensordata.row; loop ++)
	{
		orientation[loop] = calcOrientation(orientation[loop - 1],asensordata.data[loop][6], 0.025);
		//TODO: if you want to enable mag way, uncomment following line and comment out above line
		//orientation[loop] = calcOrientationMag(orientation[loop - 1], asensordata.data[loop][7],asensordata.data[loop][8],asensordata.data[loop][9]);
		

		/*if(loop * 25 >= 15800 && loop * 25 < 31675)
		if(std::abs(orientation[loop]) > 3)
		{
			std::list<wifiVector>:: iterator wifiVectorIterator;
			for(wifiVectorIterator = wifiVectorList.begin(); wifiVectorIterator!= wifiVectorList.end(); wifiVectorIterator ++)
			{
				if(wifiVectorIterator->timeStamp < loop * 25) continue;
				if(wifiVectorIterator ->isStable == true && std::abs(wifiVectorIterator->timeStamp - loop * 25) <= 2000) orientation[loop] = orientation[loop - 1];
			}
		
		}
		if(loop * 25 >= 97850 && loop * 25 < 111500)
			if(std::abs(orientation[loop]) > 3)
			{
				std::list<wifiVector>:: iterator wifiVectorIterator;
				for(wifiVectorIterator = wifiVectorList.begin(); wifiVectorIterator!= wifiVectorList.end(); wifiVectorIterator ++)
				{
					if(wifiVectorIterator->timeStamp < loop * 25) continue;
					if(wifiVectorIterator ->isStable == true && std::abs(wifiVectorIterator->timeStamp - loop * 25) <= 2000) orientation[loop] = orientation[loop - 1];
				}
				
			}*/
	}
	std::cout<<"########### IMU ############\n";
	std::cout<<"TimeStamp\t StartTime\tEndTime\tStepLength\tStepOrientation\n";
	for(loop = asensordata.StepData.begin(); loop != asensordata.StepData.end(); loop ++)
	{
		loop->calcStepOrientation(orientation);
		std::cout<<loop->StepStart <<"\t"<<loop ->StepStop <<"\t"<<loop -> StepLength <<"\t"<< loop->orientation<<std::endl;
	}
	std::cout<<"######## END OF IMU ############\n";
	/////////////////////END OF IMU PART//////////////////////////

	
	

	

	
	
	
	
	////////////////////PARTICLE FILTER PART///////////////////////////
	std::cout<<"######## START OF PARTICLE FILTER PART ############\n";
	//currentX = start_pos_x, currentY = start_pos_y;
	std::cout<<"oriengal position set to "<<currentX
	<<" "<<currentY<<"\n";
	map.drawMap();
	addWifiAPPosToCanvas(&map);
	addEnoughParticlesFirstTime(&map);
	
	addPosAndParticlesToCanvas(currentX, currentY, &particleList, &map);
	
	cv::Mat toShow = map.canvas;
	cv::Mat resized(map.height / params.zoom_scale * 2,map.width / params.zoom_scale * 2,CV_32F);
	cv::resize(toShow,resized,resized.size());
	imshow("Map B 3 ", resized);
	cleanPixelsFromCanvas(&map);
	cv::waitKey(100);
	
	std::list<Step>:: iterator it;
	for(it = asensordata.StepData.begin(); it!= asensordata.StepData.end(); it++)
	{
//system("clear");
		addEnoughParticles(&map);
		stepLength = it->StepLength * params.zoom_scale * params.zoom_scale;
		timeStampStart = it->StepStart ,timeStampStop = it->StepStop;
		updateParticlesPosition(&map, &particleList, stepLength  ,it->orientation ,it->StepStart * 25);
		ori = orientation [it->StepStart];
		timeStampStop = it->StepStop * 25;
		
		updatePositionCoordination(&map);
		//std::cout<<it->StepStop * 25<<"\n";
		addPosAndParticlesToCanvas(currentX, currentY, &particleList, &map);
		cv::Mat  toShow = map.canvas;
		cv::Mat resized(map.height / params.zoom_scale * 2,map.width / params.zoom_scale * 2,CV_32F);
		cv::resize(toShow,resized,resized.size());
		imshow("Map B 3 ", resized);
		cleanPixelsFromCanvas(&map);
		calc_last_distance();
		cv::waitKey(100);
		
		
		
	}
	std::cout<<"######## END OF PARTICLE FILTER PART ############\n";
	std::cout<<"all steps are proceeded, press any key to exit.\n";
	{
		map.drawMap();
		addTraceLineToCanvas(&map);
		
		cv::Mat  toShow = map.canvas;
		cv::Mat resized(map.height / params.zoom_scale * 2,map.width / params.zoom_scale * 2,CV_32F);
		cv::resize(toShow,resized,resized.size());
		//cleanPosAndParticles(currentX, currentY, &particleList, &map);
		imshow("Map B 3 ", resized);
		cleanPixelsFromCanvas(&map);
		cv::waitKey();
	}

	/////////////END OF PARTICLE FILTER PART///////////////////////////
	
	//////////////////CLEANNING UP PART//////////////////////////////
	if(orientation != NULL)delete orientation;
	return 0;
	//////////////////END OF CLEANNING UP PART///////////////////////

}






double calcOrientation(double orientation, double gyro, double deltaTime)
{
	return orientation + gyro * deltaTime * 180 / 3.14;
}

// TODO: calc ori mag way implementation comes here
double calcOrientationMag(double origination, double magX, double magY, double magZ)
{
	return 0;
}

int countLiveParticles()
{
	if(particleList.size() == 0)
	{
		std::cout<<"now living particles num is  0\n";
		return  0;
	}
	else
	{
		int count = 0;
		std::list<particle> :: iterator it;
		for(it = particleList.begin(); it != particleList.end(); it++)
		{
			if(it->isAlive == true) count ++;
		}
		particleCount = count;
		std::cout<<"now living particles num is "<<count<<"\n";
		return count;
	}

}


void addEnoughParticlesFirstTime(ImageSet * map)
{
	if(params.particle_number == 0)return ;
	
	std::cout<<"######ADDING NEW PARTICLES#####\n";
	countLiveParticles();
	gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
	std::cout<<"CUrrent x y "<<currentX<<" "<<currentY<<"\n";
	if(particleList.size() < params.particle_number)
	{
		std::cout<<"here\n";
		for(int loop = 0; loop < params.particle_number - particleCount; loop ++ )
		{
			double distance = gsl_rng_uniform(r) * params.circus;
//			double distance = gsl_ran_ugaussian (r) * circus;
			int angle = gsl_rng_uniform(r) * 180;
			int tempX = currentX - sin(angle) * distance;
			int tempY = currentY + cos(angle) * distance;
			if(tempX <0 || tempX >= map->height || tempY < 0 || tempY >= map->width)
			{
				//std::cout<<"pos invalid, re choose \n";
				loop --;
				continue;
			}
			bool isHitWall =bresenhanLineHitCheck(currentX, currentY, tempX, tempY, map, "both");
			
			if(isHitWall == false)
			{
				particleList.push_back(particle(map, particleID, tempX, tempY));
				particleID ++;
			}
			else
			{
				loop --;
				continue;
			}
			
		}
	}
	if(particleCount == 0)
	{
		std::list<particle>::iterator it;
		for(it = particleList.begin(); it != particleList.end();it ++)
		{
			double distance = gsl_rng_uniform(r) * params.circus;
			//			double distance = gsl_ran_ugaussian (r) * circus;
			int angle = gsl_rng_uniform(r) * 180;
			int tempX = currentX - sin(angle) * distance;
			int tempY = currentY + cos(angle) * distance;
			if(tempX <0 || tempX >= map->height || tempY < 0 || tempY >= map->width)
			{
				//std::cout<<"pos invalid, re choose \n";
				it --;
				continue;
			}
			bool isHitWall =bresenhanLineHitCheck(currentX, currentY, tempX, tempY, map, "both");
			
			if(isHitWall == false)
			{
				it->currentX = tempX, it->currentY = tempY;
				it->isAlive = true;
			}
			else
			{
				it --;
				continue;
			}
			
		}
	}

}


void addEnoughParticles(ImageSet * map)
{
	
	if(params.particle_number == 0)return ;
	std::cout<<"######ADDING NEW PARTICLES#####\n";
	generateProbTable();
	countLiveParticles();
	if(particleCount == params.particle_number) return;
	if(particleCount > 5 * std::sqrt(params.particle_number)) return;
	gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
	std::list<particle>::iterator it;
	for(it = particleList.begin(); it != particleList.end();it ++)
	{
		if(it->isAlive == true)continue;
		double randomNum = gsl_rng_uniform (r);
		int selectedId = -5;
		int selectedPosX = 0, selectedPosY = 0;
		std::list<ProbTableElement>::iterator it2;
		for(it2 = problist->begin(); it2 != problist->end(); it2++)
		{
			if(it2->start<= randomNum && it2->end> randomNum)
				selectedId = it2->idNum;
		}
		std::list<particle>:: iterator it3;
		for(it3 = particleList.begin(); it3!= particleList.end(); it3++)
		{
			if(it3->particleId == selectedId)
			{
				selectedPosX = it3->currentX;
				selectedPosY = it3->currentY;
			}
		}
		//std::cout<<randomNum<<" "<<selectedId<<" "<<selectedPosX<<" "<<selectedPosY<<"\n";
		bool success = false;
		int tempX = 0, tempY = 0;
		while(!success)
		{
			double distance = gsl_ran_ugaussian (r) * params.circus;
			int angle = gsl_rng_uniform(r) * 180;
			tempX = selectedPosX - sin(angle) * distance;
			tempY = selectedPosY + cos(angle) * distance;
			if(tempX <0 || tempX >= map->height || tempY < 0 || tempY >= map->width)
			{
				success = false;
				tempX = 0, tempY = 0;
				continue;
			}
			bool isHitWall =bresenhanLineHitCheck(selectedPosX, selectedPosY, tempX, tempY, map,"wall");
			if(isHitWall == false)
			{
				success = true;
				break;
			}
			else
			{
				success = false;
			}
		}
		it->currentX = tempX, it->currentY = tempY, it ->isAlive = true, it->positionList.clear();
	}
	std::cout<<"######FINISH ADDING NEW PARTICLES#####\n";
}

void updateParticlesPosition(ImageSet * map,std::list<particle> * particleList, double stepLength, int ori, long timeStamp)
{
	std::list<particle>:: iterator particleIterator;
	for(particleIterator = particleList->begin(); particleIterator != particleList->end(); particleIterator ++)
	{
		if(ori < 0) ori += 360;
		float newX = particleIterator-> currentX - std::sin((float)ori / 180. * 3.14) * stepLength ;
		float newY = particleIterator-> currentY + std::cos((float)ori / 180. * 3.14) * stepLength ;
		//std::cout<<"==== check point 4 ====="<< particleIterator->particleId<<"\n";
		
		if(bresenhanLineHitCheck(newX, newY,particleIterator-> currentX, particleIterator-> currentY, map, "wall") == false)
		{
			//doorCheckAndCorrection(timeStamp, map, particleIterator-> currentX, particleIterator-> currentY, newX, newY);
			if(doorCheckAndCorrection(timeStamp, map, particleIterator-> currentX, particleIterator-> currentY, newX, newY) == 1)
				;//particleIterator->stepCount = particleIterator->stepCount * 2;
			else if((doorCheckAndCorrection(timeStamp, map, particleIterator-> currentX, particleIterator-> currentY, newX, newY) == -1))
			{
				//particleIterator->stepCount = 0;
				//particleIterator->wrongRoom = true;
				//particleIterator->isAlive = false;
			}
			particleIterator-> currentX = newX;
			particleIterator-> currentY = newY;
			particleIterator->stepCount++;
			//std::cout<<"==== finished check point 4 ====="<< particleIterator->particleId<<"\n";
			//particleIterator->positionList.push_back(particlePosition(particleIterator->stepCount, nextX, nextY, timeStamp));
		}
		else
		{
			particleIterator->isAlive = false;
		}
		
		
	}
	
}


void updatePositionCoordination(ImageSet * map)
{
	std::cout<<"#################now update Position coordination #####################\n";
	
	countLiveParticles();
	if(particleCount == 0) addEnoughParticlesFirstTime(map);
	double particleX = 0, particleY = 0;
	std::list<particle>:: iterator particleIterator;
	
	int sum = 0;
	int liveCount = 0;
	std::list<particle>::iterator it;
	//std::cout<<"================== check live count ==================\n";
	for(it = particleList.begin(); it != particleList.end();it ++)
	{
		
		if(it->isAlive)
		{
			sum += it->stepCount;
			liveCount ++;
			//			std::cout<<it -> particleId<<" "<<it->stepCount<<"\n";
		}
		
	}

	
	
	
	for(particleIterator = particleList.begin(); particleIterator != particleList.end(); particleIterator ++)
	{
		if(particleIterator ->isAlive == true)
		{
			if (sum != 0)
			{
				particleX += particleIterator ->currentX * (double)(particleIterator->stepCount) / (double)sum ;
				particleY += particleIterator ->currentY * (double)(particleIterator->stepCount) / (double)sum ;
			}
			else
			{
				particleX += (double)particleIterator ->currentX / (double)liveCount;
				particleY += (double)particleIterator ->currentY / (double)liveCount;
			}
			
			
		}
	}
	currentX =(int)particleX,currentY = (int)particleY;
	std::cout<<"step "<<stepCount<< " updated to "<<currentX<<" "<<currentY<<" "<<timeStampStart * 25<<"\n";//<<timeStampStop * 25<<"\n";
	stepCount ++;
	std::cout<<"################# Position coordination updated   ######################\n";
}


void addPosAndParticlesToCanvas(int currentX, int currentY, std::list<particle> * particleList, ImageSet * map)
{
	
	CvScalar iter;
	
	
	if(params.particle_number != 0)
	{
	iter.val[0] = 200;
	std::list<particle> :: iterator it;
	for(it = particleList->begin(); it != particleList ->end(); it++ )
	{
		if(it->isAlive == true)
		{
			for(int loopx = -5; loopx < 5; loopx ++)
			{
				for(int loopy = -5; loopy < 5; loopy ++)
				{
					if(it->currentX + loopx >= 0 && it->currentX + loopx < map->height && it->currentY + loopy >= 0 && it->currentY + loopy < map->width)
					{
						if(map->grayMap[it->currentX + loopx][it->currentY + loopy] == 255)
						{
							cvSet2D(map->canvas, it->currentX + loopx, it->currentY + loopy, iter);
							pixelList.push_back(pixel(it->currentX + loopx, it->currentY + loopy));
						}
					}
				}
			}
		
			
		}
		else
		{
			//std::cout<<"dead particle "<<it->particleId<<" not shown \n";
		}
		
	}
	}
	iter.val[0] = 0;
	iter.val[1] = 0, iter.val[2] = 0, iter.val[3] = 0;
	centerPointTrace.push_back(pixel(currentX,currentY));
	for(int loopx = -10; loopx < 10; loopx ++)
	{
		for(int loopy = -10; loopy < 10; loopy ++)
		{
			if(currentX + loopx >= 0 && currentX + loopx < map->height && currentY + loopy >= 0 && currentY + loopy < map->width)
			{
				//if(map->grayMap[currentX + loopx][currentY + loopy] == 255)
				{
					cvSet2D(map->canvas, currentX + loopx, currentY + loopy, iter);
					pointTrace.push_back(pixel(currentX + loopx, currentY + loopy));
					pixelList.push_back(pixel(currentX + loopx, currentY + loopy));
				}
			}
		}
	}
	
}


void addTraceLineToCanvas(ImageSet * map)
{
CvScalar iter;
	iter.val[0] = 50;
	iter.val[1] = 50, iter.val[2] = 50, iter.val[3] = 50;
	std::list<pixel> :: iterator pixelIterator;
	int startX = pointTrace.begin()->posX;
	int startY = pointTrace.begin()->posY;
	for(pixelIterator = pointTrace.begin(); pixelIterator != pointTrace.end(); pixelIterator ++)
	{
		cvSet2D(map->canvas, pixelIterator->posX, pixelIterator->posY, iter);
	}
	pixelIterator = centerPointTrace.begin();
	pixelIterator ++;
	for(; pixelIterator != centerPointTrace.end(); pixelIterator ++)
	{
		drawLine(startX, startY, pixelIterator->posX, pixelIterator->posY, map);
		startX =pixelIterator->posX;
		startY =pixelIterator->posY;
	}

}

void cleanPixelsFromCanvas(ImageSet * map)
{
	CvScalar iter;
	iter.val[0] = 255;
	iter.val[1] = 0, iter.val[2] = 0, iter.val[3] = 0;

	std::list<pixel> :: iterator it;
	if(!pixelList.empty())
		for(it = pixelList.begin(); it != pixelList.end(); it++)
		{
			cvSet2D(map->canvas, it->posX, it->posY, iter);
		}
	pixelList.clear();
}



bool bresenhanLineHitCheck(int pt1x, int pt1y, int pt2x, int pt2y, ImageSet * map ,std::string doorOrWall)
{
	if(doorOrWall == "both")
	{
		int ** mapToCheck = NULL;
		mapToCheck = map->grayMap;
		
		
		if(pt1x < 0 || pt1x >= map->height || pt1y < 0 || pt1y >= map->width)
			return true;
		if(mapToCheck[pt2x][pt2y] < 255)
		{
			//std::cout<<"adding hit wall \n";
			
			return true;
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
		
		if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
		
		for(k = 0; k < steps; k++)
		{
			x += xInc;
			y += yInc;
			if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
		}
		
		
		mapToCheck = map->doorPosMap;
		
		{
			if(pt1x < 0 || pt1x >= map->height || pt1y < 0 || pt1y >= map->width)
				return true;
			if(mapToCheck[pt2x][pt2y] < 255)
			{
				//std::cout<<"adding hit wall \n";
				
				return true;
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
			
			if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
			
			for(k = 0; k < steps; k++)
			{
				x += xInc;
				y += yInc;
				if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
			}

		}
		
		return false;
		
	}
	else
	{
		int ** mapToCheck = NULL;
		if(doorOrWall == "wall") mapToCheck = map->grayMap;
		if(doorOrWall == "door") mapToCheck = map->doorPosMap;
		
		if(pt1x < 0 || pt1x >= map->height || pt1y < 0 || pt1y >= map->width)
			return true;
		if(mapToCheck[pt2x][pt2y] < 255)
		{
			//std::cout<<"adding hit wall \n";
			
			return true;
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
		
		if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
		
		for(k = 0; k < steps; k++)
		{
			x += xInc;
			y += yInc;
			if(mapToCheck[(int)round(x)][(int)round(y)] < 255) return true;
		}
		
		return false;
	}

}

void drawLine(int pt1x, int pt1y, int pt2x, int pt2y, ImageSet * map)
{
	CvScalar iter;
	iter.val[0] = 50;
	int dx = pt2x - pt1x, dy = pt2y - pt1y, steps, k;
	float xInc, yInc, x = pt1x, y = pt1y;
	
	if(std::fabs(dx)> std::fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xInc = float(dx) / float(steps);
	yInc = float(dy) / float(steps);
	for(int loopx = -5; loopx < 5; loopx ++)
	{
		for(int loopy = -5; loopy < 5; loopy ++)
		{
			if((int)round(x)+ loopx >= 0 && (int)round(x) + loopx < map->height && (int)round(y) + loopy >= 0 && (int)round(y) + loopy < map->width)
			cvSet2D(map->canvas, (int)round(x)+ loopx, (int)round(y) + loopy, iter);
		}
	}
	
	for(k = 0; k < steps; k++)
	{
		x += xInc;
		y += yInc;
		for(int loopx = -5; loopx < 5; loopx ++)
		{
			for(int loopy = -5; loopy < 5; loopy ++)
			{
				if((int)round(x)+ loopx >= 0 && (int)round(x) + loopx < map->height && (int)round(y) + loopy >= 0 && (int)round(y) + loopy < map->width)
					cvSet2D(map->canvas, (int)round(x)+ loopx, (int)round(y) + loopy, iter);
			}
		}
	}

}




void loadAllowedList()
{
	std::ifstream in;
	in.open(params.allowed_list_path.c_str() , std::ios::in);
	if(!in.is_open())
	{
		std::cout<< "Wrong Path\n";
		in.clear();
	}
	std::cout<< "opened\n";
	while(!in.eof())
	{
		accessPoint tmp ;
		in>> tmp.BSSID >> tmp.posX >> tmp.posY >> tmp.refRSSI >> tmp.gamma;
		std::cout<<tmp.BSSID<<" "<<tmp.posX<<" "<<tmp.posY<<" "<<tmp.refRSSI<<" "<< tmp.gamma<<"\n";
		AllowedList.push_back(tmp);
	}
}


void loadRoomList()
{
	std::ifstream in;
	in.open(params.room_list_path.c_str() , std::ios::in);
	if(!in.is_open())
	{
		std::cout<< "Wrong Path\n";
		in.clear();
	}
	std::cout<< "opened\n";
	while(!in.eof())
	{
		room tmp ;
		in>> tmp.roomName >> tmp.BSSID >> tmp.north >> tmp.south >> tmp.west>> tmp.east>> tmp.doorStart>>tmp.doorEnd>>tmp.doorUp>> tmp.doorDown;
		roomList.push_back(tmp);
	}
	std::list<room> ::iterator it;
	for(it = roomList.begin(); it != roomList.end(); it++)
	{
		std::cout<<it->roomName<<" "<<it->BSSID<<"\n";
	}

}

void addWifiAPPosToCanvas(ImageSet * map)
{
	CvScalar iter;
	iter.val[0] = 50;
	iter.val[1] = 155, iter.val[2] = 155, iter.val[3] = 0;
	std::list<accessPoint> ::iterator it;
	for(it = AllowedList.begin(); it!= AllowedList.end(); it++)
	{
		for(int loopx = -10; loopx < 10; loopx ++)
		{
			for(int loopy = -10; loopy < 10; loopy ++)
			{
				if(it->posX + loopx >= 0 && it->posX + loopx < map->height && it->posY + loopy >= 0 && it->posY + loopy < map->width)
				{
					cvSet2D(map->canvas, it->posX + loopx, it->posY + loopy, iter);
				}
			}
		}

		
	}
	
}






void loadWifiInfo(std::string wifiInfoPath)
{
	std::ifstream in;
	in.open(wifiInfoPath.c_str() , std::ios::in);
	if(!in.is_open())
	{
		std::cout<< "Wrong Path\n";
		in.clear();
	}
	while(!in.eof())
	{
		accessPoint tmp ;
		in>> tmp.BSSID  >> tmp.frequency >> tmp.strength >> tmp.timeStamp;
		if(isInAllowedList(&tmp))
			allApList.push_back(tmp);
	}
}



bool isInAllowedList(accessPoint * toCheck)
{
	std::list<accessPoint> ::iterator it;
	for(it = AllowedList.begin(); it != AllowedList.end(); it ++)
	{
		if(toCheck->BSSID == it ->BSSID)
		{
			toCheck -> posX = it -> posX;
			toCheck -> posY = it -> posY;
			toCheck -> refRSSI = it ->refRSSI;
			toCheck -> gamma = it -> gamma;
			return true;
		}
	}
	return false;
}



void generateApStatus()
{
	std::list<accessPoint> ::iterator it;
	std::list<apStatus> :: iterator statusIt;

	for(it = allApList.begin(); it != allApList.end(); it++)
	{
		if(apStatusList.empty())
		{
			apStatus tmp;
			tmp.timeStamp = it->timeStamp;
			accessPoint tmp2(*it);
			tmp.apList.push_back(tmp2);
			apStatusList.push_back(tmp);
			statusIt = apStatusList.begin();
			continue;
			
		}
		else
		{
			if(std::abs(statusIt->timeStamp - it->timeStamp) < 20)
			{
				accessPoint tmp2(*it);
				statusIt->apList.push_back(tmp2);
			}
			else
			{
				apStatus tmp;
				tmp.timeStamp = it->timeStamp;
				accessPoint tmp2(*it);
				tmp.apList.push_back(tmp2);
				apStatusList.push_back(tmp);
				statusIt ++;
			}
			
		}
		
	}
	//for(statusIt = apStatusList.begin(); statusIt != apStatusList.end(); statusIt++)
	//{
		//std::cout<<"------------------------------------------------------------\n";
		//std::cout<<"status: "<<statusIt->timeStamp<<"\n";
	//	for(it = statusIt->apList.begin(); it !=  statusIt->apList.end(); it ++)
	//	{
	//		std::cout<<"\t"<<it->BSSID<<" "<<it->strength<<" "<<it->posX<<" "<<it->posY<<" "<<it->gamma<<" "<<it->timeStamp<<"\n";
	//	}
		
	//}
}


std::string wifiRoomDiff(long timeStamp)
{
	int checkCount = 0;
	std::list<apStatus> :: iterator statusIterator;
	if(!apStatusList.empty())
	for(statusIterator = apStatusList.begin(); statusIterator != apStatusList.end() && checkCount < params.delay_time_val; statusIterator ++)
	{
		if(statusIterator->timeStamp < timeStamp)continue;
		else{
			
			if(!statusIterator->apList.empty())
			{
				std::list<accessPoint> :: iterator apIterator= statusIterator->apList.begin();
				std::string maxBSSID = apIterator->BSSID; int maxStrength = apIterator-> strength;
				for(apIterator = statusIterator->apList.begin(); apIterator != statusIterator->apList.end(); apIterator++)
				{
					if(apIterator->strength > maxStrength)
					{
						maxBSSID = apIterator->BSSID;
						maxStrength = apIterator ->strength;
					}
				}
				if(maxStrength > params.wifi_room_check_threshold)
				{
					std::list<room>:: iterator roomIterator ;
					for(roomIterator= roomList.begin(); roomIterator!= roomList.end(); roomIterator++)
					{
						if(maxBSSID == roomIterator->BSSID)
						{
							return roomIterator->roomName;
						//	std::cout<<"Now into room "<<roomIterator->roomName<<"\n";
						}
					}
					
					
					
				}
			}
			checkCount ++;
		}
	}
	return "NULL";
}









void addPointToCanvas(ImageSet * map, int x, int y)
{
	CvScalar iter;
	iter.val[0] = 150;
	iter.val[1] = 155, iter.val[2] = 155, iter.val[3] = 0;
	
		for(int loopx = -5; loopx < 5; loopx ++)
		{
			for(int loopy = -10; loopy < 10; loopy ++)
			{
				if(x + loopx >= 0 && x + loopx < map->height && y + loopy >= 0 && y + loopy < map->width)
				{
					cvSet2D(map->canvas, x + loopx, y + loopy, iter);
				}
			}
		}
		
		
}

void loadStartPoint(std::string path)
{
	std::ifstream in;
	in.open(path.c_str(), std::ios::in);
	in>> currentX>>currentY>> ori;
	startUpX = currentX;
	startUpY = currentY;
	in.clear();
	in.close();
}


int doorCheckAndCorrection(long timeStamp, ImageSet * map, int curX, int curY, int nexX, int nexY)
{
	std::string stepIntoWhichRoom = checkStepIntoWhichRoom(map, curX, curY, nexX, nexY);
	if(stepIntoWhichRoom == "NULL") return 0;
	//std::cout<<"into which room "<<stepIntoWhichRoom;
	std::string whichRoomIsRight = wifiRoomDiff(timeStamp);
	//std::cout<<" which is right "<< whichRoomIsRight<<" time "<<timeStamp <<"\n";
	if(stepIntoWhichRoom == whichRoomIsRight)return 1;
	else return -1;
}

std::string checkStepIntoWhichRoom(ImageSet* map, int curX, int curY, int nexX, int nexY)
{
	if(bresenhanLineHitCheck(curX, curY, nexX, nexY, map, "door"))
	{
		std::list<room>::iterator roomIterator;
		for(roomIterator = roomList.begin(); roomIterator != roomList.end(); roomIterator ++)
		{
			if(nexX >= roomIterator->north && nexX < roomIterator->south && nexY >= roomIterator->west && nexY < roomIterator->east)
				return roomIterator->roomName;
		}
	}
	return  "NULL";
}

void wifiVectorAnalysis(long timeStamp)
{
	std::list<apStatus>::iterator apStatusIterator;
	for(apStatusIterator = apStatusList.begin(); apStatusIterator != apStatusList.end(); apStatusIterator ++)
	{
		if(apStatusIterator -> timeStamp < timeStamp)continue;
		std::list<accessPoint>::iterator allowedListIterator;
		for(allowedListIterator = AllowedList.begin(); allowedListIterator != AllowedList.end(); allowedListIterator ++)
		{
			if(allowedListIterator ->moveStatus == DISAPPEAR ) allowedListIterator->moveStatus = UNDEFINEDSTATUS;
			std::list<accessPoint>::iterator apIterator;
			bool isAppear = false;
			for(apIterator = apStatusIterator->apList.begin(); apIterator != apStatusIterator->apList.end(); apIterator ++)
			{
				if(allowedListIterator->BSSID == apIterator ->BSSID)
				{
					isAppear = true;
					if(allowedListIterator->moveStatus == UNDEFINEDSTATUS)
					{
						allowedListIterator->moveStatus = APPEAR;
						allowedListIterator->statusStrength = apIterator->strength;
					}
					if(allowedListIterator->moveStatus != UNDEFINEDSTATUS)
					{
						if (apIterator ->strength - allowedListIterator->statusStrength > 5 )
						{
							allowedListIterator->moveStatus = APPROACHING;
							allowedListIterator->statusStrength = apIterator->strength;
						}
						if(allowedListIterator->statusStrength - apIterator ->strength > 5  )
						{
							allowedListIterator->moveStatus = LEAVING;
							allowedListIterator->statusStrength = apIterator->strength;
						}
						
					}
					
				}
				
			}
			if(!isAppear)
			{
				allowedListIterator->moveStatus = DISAPPEAR;
				allowedListIterator->statusStrength = 0;
			}
		
		}
		break;
	}
	int countApproaching = 0;
	int countLeaving = 0;
	int approachX = 0, approachY = 0, leaveX = 0, leaveY = 0;
	std::list<accessPoint>::iterator allowedListIterator;
	for(allowedListIterator = AllowedList.begin(); allowedListIterator != AllowedList.end(); allowedListIterator ++)
	{
		if(allowedListIterator->moveStatus == APPROACHING || allowedListIterator->moveStatus == APPEAR)
		{
			countApproaching ++;
			approachX += allowedListIterator->posX;
			approachY += allowedListIterator->posY;
		}
		if(allowedListIterator->moveStatus == LEAVING || allowedListIterator->moveStatus == DISAPPEAR)
		{
			countLeaving ++;
			leaveX+= allowedListIterator->posX;
			leaveY+= allowedListIterator->posY;
		}
	}
	if(countApproaching!= 0)
	{
		approachX /= countApproaching;
		approachY /= countApproaching;
	}
	if(countLeaving != 0)
	{
		leaveX /= countLeaving;
		leaveY /= countLeaving;
	}
	if(countApproaching != 0 && countLeaving != 0)
	{
		double angle = 0;
		double sinTheta = std::abs(approachX - leaveX) / std::sqrt((approachX - leaveX) * (approachX - leaveX) + (approachY - leaveY) * (approachY - leaveY));
		//double cosTheta = (approachY - leaveY) / std::sqrt((approachX - leaveX) * (approachX - leaveX) + (approachY - leaveY) * (approachY - leaveY));
		if(approachX - leaveX < 0 && approachY - leaveY > 0) angle =   std::asin(sinTheta) * 180 / 3.14;
		if(approachX - leaveX < 0 && approachY - leaveY < 0) angle = 90 + std::asin(sinTheta) * 180 / 3.14;
		if(approachX - leaveX > 0 && approachY - leaveY < 0) angle = 180 + std::asin(sinTheta)* 180 / 3.14;
		if(approachX - leaveX > 0 && approachY - leaveY > 0) angle = 270 + std::asin(sinTheta)* 180 / 3.14;
		//if(angle < 0) angle += 360;
		wifiVectorList.push_back(wifiVector(timeStamp, false, angle));
		std::cout<<"Time "<<timeStamp<<" from "<<leaveX<<" "<<leaveY<<" to "<<approachX<<" "<<approachY<<" "<<angle<<"\n";
	}
	
	

}


void checkIfIsStable()
{
	std::list<wifiVector>:: iterator it1;
	for(it1 = wifiVectorList.begin(); it1 != wifiVectorList.end(); it1 ++)
	{
		std::list<wifiVector>:: iterator it2 = (it1 ++);
		it1 --;
		if(it2 != wifiVectorList.end())
		{
			if(std::abs(it1->orientation - it2->orientation) < 5)it2->isStable = true;
		}
	}
}


void generateProbTable()
{
	int sum = 0;
	double probStart = 0;
	int liveCount = 0;
	std::list<particle>::iterator it;
//std::cout<<"================== check live count ==================\n";
	for(it = particleList.begin(); it != particleList.end();it ++)
	{
		
		if(it->isAlive)
		{
			if(it->wrongRoom) it->stepCount = 0;
			sum += it->stepCount * it->stepCount;
			liveCount ++;
//			std::cout<<it -> particleId<<" "<<it->stepCount<<"\n";
		}
		
	}
//	std::cout<<"================== endof check live count ==================\n";
//	std::cout<<sum<<"\n";
	if ( problist != NULL ) delete problist;
	problist = new std::list<ProbTableElement>;
	for(it = particleList.begin(); it != particleList.end();it ++)
	{
		if(it->isAlive)
		{
			if(sum == 0)
			{
				ProbTableElement temp(it->particleId, probStart, probStart + 1.0/ liveCount);
				probStart +=1.0/ liveCount;
				problist->push_back(temp);
			}
			else
			{
				ProbTableElement temp = ProbTableElement(it->particleId, probStart, probStart + (double)it->stepCount * it->stepCount / (double)sum);
				probStart += (double)it->stepCount * it->stepCount / (double)sum;
				problist->push_back(temp);
			}
		}
	}
	if(problist->back().end != 1) problist->back().end = 1;
//	std::list<ProbTableElement>::iterator it2;
//	std::cout<<"================== Prob Table ==================\n";
//	for(it2 = problist->begin(); it2 != problist->end(); it2++)
//	{
//		std::cout<<it2->idNum<<" "<<it2->start<<" "<<it2->end<<"\n";
//	}
//	std::cout<<"================================================\n";
//	std::cout<<problist->size()<<"\n";

}








void WIFIPositioningProc(ImageSet * map)
{
	std::list<apStatus> ::iterator statusIt;
	for(statusIt = apStatusList.begin(); statusIt != apStatusList.end(); statusIt ++)
	{
		WifiResult->x = 0, WifiResult->y = 0;
		std::cout<<"------------------------------------------------------------\n";
		std::cout<<"Now proceeding "<<statusIt->timeStamp<<"\n";
		if(statusIt->apList.size() <= 2)
		{
			std::list<accessPoint>::iterator apIt = statusIt->apList.begin();
			
			
			std::cout<<"status "<< statusIt->timeStamp<<" don't have enough ap information\n";
			if(statusIt->apList.size() == 1)
			{
				accessPoint  a1(*apIt);
				accessPoint a2;
				accessPoint a3;
				trilaterationProcess(&a1, &a2, &a3);
				
			}
			if(statusIt->apList.size() == 2)
			{
				accessPoint  a1(*apIt);
				apIt++;
				accessPoint  a2(*apIt);
				accessPoint a3;
				trilaterationProcess(&a1, &a2, &a3);
			}
			continue;
		}
		if(statusIt->apList.size() == 3)
		{
			
			std::list<accessPoint>::iterator apIt = statusIt->apList.begin();
			accessPoint  a1(*apIt);
			apIt++;
			accessPoint  a2(*apIt);
			apIt++;
			accessPoint  a3(*apIt);
			trilaterationProcess(&a1, &a2, &a3);
			if(WifiResult->x > -100 && WifiResult->x < 20000 && WifiResult->y > -100 && WifiResult->y < 20000)
			{
				addPointToCanvas(map, WifiResult->x , WifiResult->y );
				/*
				 cv::Mat  toShow = map->canvas;
				 cv::Mat resized(map->height / zoom_scale * 2,map->width / zoom_scale * 2,CV_32F);
				 cv::resize(toShow,resized,resized.size());
				 imshow("Map B 3 ", resized);
				 cv::waitKey();*/
				
			}
			//std::cout<<"Status "<<statusIt ->timeStamp <<"position result "<< statusIt->timeStamp<<" "<< WifiResult->x<<" "<<WifiResult->y<<"\n";
			continue;
		}
		if(statusIt->apList.size() >= 4)
		{
			//to be added
			int tempx = 0, tempy = 0;
			int count = 0;
			accessPoint * templist = new accessPoint[statusIt->apList.size()];
			int loop = 0;
			std::list<accessPoint>::iterator it;
			for(it = statusIt->apList.begin(); it != statusIt->apList.end(); it++)
			{
				templist[loop].BSSID = it->BSSID;
				templist[loop].frequency = it->frequency;
				templist[loop].gamma = it->gamma;
				templist[loop].posX = it->posX;
				templist[loop].posY = it->posY;
				templist[loop].refRSSI = it->refRSSI;
				templist[loop].strength = it->strength;
				templist[loop].timeStamp = it->timeStamp;
				loop ++;
			}
			
			for(int loop0 = 0; loop0 < loop - 2; loop0++)
			{
				for(int loop1 = loop0 + 1; loop1 < loop - 1; loop1 ++)
				{
					for(int loop2 = loop1 + 1; loop2 < loop; loop2 ++)
					{
						
						trilaterationProcess(&templist[loop0], &templist[loop1], &templist[loop2]);
						tempx += WifiResult->x;
						tempy += WifiResult->y;
						count ++;
						
					}
				}
				
			}
			tempx /= count, tempy /= count;
			std::cout<<"ave position "<< tempx<<" "<<tempy<<"\n";
			if(tempx > -100 && tempx < 20000 && tempy > -100 && tempy < 20000)
			{
				addPointToCanvas(map, tempx , tempy );
				/*
				 cv::Mat  toShow = map->canvas;
				 cv::Mat resized(map->height / zoom_scale * 2,map->width / zoom_scale * 2,CV_32F);
				 cv::resize(toShow,resized,resized.size());
				 imshow("Map B 3 ", resized);
				 cv::waitKey();*/
				
			}
			
		}
		
	}
	
	
	
}

void KCDCMultilateration(apStatus * toCalc)
{
	
	
	
	
	
}

point * trilaterationProcess(accessPoint * a1, accessPoint * a2, accessPoint * a3)
{
	Trilateration* temp = new Trilateration(a1, a2, a3, WifiResult);
	//std::cout<< "distance "<< temp->distance1 <<" "<<temp->distance2<<" "<< temp ->distance3 << "\n";
	temp->TrilaterationProcess();
	std::cout<<"position "<< WifiResult->x<<" "<<WifiResult->y<<"\n";
	delete temp;
	return 0;
}


void calc_last_distance()
{
	std::cout<<"Distance from start point: "<< std::sqrt((double)((startUpX - currentX)* (startUpX - currentX) + (startUpY - currentY) * (startUpY - currentY)))<<"\n";
}


