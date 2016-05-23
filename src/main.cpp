#include "../include/instanceFileReader.h"
#include "../include/nodes.h"
#include <iostream>
#include <vector>
using namespace std;

InstanceFileReader *data = NULL;


int main(int argc, char** argv){
    data = new InstanceFileReader(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.
    data->readFile();
	data->createGLPKDataFile(argv[2]); //Parâmetro 2: nome do arquivo de saída no formato padrão do GLPK.
}

