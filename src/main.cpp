#include "../include/instanceFileReader.h"
#include "../include/nodes.h"
#include <iostream>
#include <vector>
using namespace std;

InstanceFileReader *data = NULL;


int main(int argc, char** argv){
    data = new InstanceFileReader(argv[1]);
    data->readFile();
}

