#pragma once
#include <vector>
#include <stdio.h>
#include <string>

struct coord
{
	int verticeNumber ;
	float xyz[3];
};

struct coordinate
{
	int verticeNumber ;
	float xyz[3];
	
	
};

struct face
{
	
	int faceNumb[3];
	int vt[3];
	int vn[3];

};
class freading
{
public:
	freading(char name[]);
	~freading(void);
	std::vector<coordinate>getvector();
	std::vector<face>getfaces();

private:
	FILE * pFile;
	char checkbuff[2];
	long sizeT;
	float x;
	int verticesInFile;
	std::vector<coordinate> vertices;//The vector holding al the vertices of the object.
		coordinate testP;
		
		face testF;
		std::vector<face> surfaces;
};

