#ifndef _INSTANCEFILEREADER_H
#define _INSTANCEFILEREADER_H
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm> 
#include <iostream>  
#include "nodes.h"
using namespace std;

class InstanceFileReader {

private:
	int numNodes;
    int numVehicles;
    int capacitated;
	std::vector< std::vector<double> > timeDistance;
	std::vector<Nodes> nodes;
	std::string filename;
	FILE* fileRead;
	

	void readDistanceMatrix();

public:
	InstanceFileReader(const char* filename);
	int getNumNodes();
	void setNumNodes(int n);
	int getCapacitated();
	void setCapacitated(int n);
	void readFile();
	void createGLPKDataFile(const char* glpkFileName);
	double getDurationTimeNode(int n);
	double getInitialTimeNode(int n);
	double getEndTimeNode(int n);
	const char* getFilename();
	const char* getInstanceName();
	//std::vector< std::vector<double> > getTimeDistanceMatrix();


};

#endif
