#ifndef _NODES_H
#define _NODES_H

using namespace std;

class Nodes {
private:
	int id;
	int initialTime;
	int endTime;
	int durationTime;
	int demand;
	int coordX;
	int coordY;
	double initialServiceTime;

public:
	Nodes(int id, int initialTime, int endTime, int durationTime, int demand, int coordX, int coordY, double initialServiceTime);
	int getId();
	int getDemand();
	int getCoordX();
	int getCoordY();
	int getInitialTime();
	int getEndTime();
	int getDurationTime(); 
	double getInitialServiceTime();
	double setInitialServiceTime(double time);

};

#endif
