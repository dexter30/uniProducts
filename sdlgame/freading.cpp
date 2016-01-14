#include "freading.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

//The main purpose of this file is to load in vertexes from an obj.
freading::freading(char name[])
{
	verticesInFile = 0;//Records the number of vertexs in file.
	pFile = fopen (name, "rb");
	fseek (pFile , 0 , SEEK_END);
  sizeT = ftell (pFile);
  rewind (pFile);
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  for(int k=0;k<sizeT;k++)//Repeats for the duration of the feil length.
	{
		
		fread(checkbuff, 1,2,pFile);
		
		if (checkbuff[0] == 'v' && checkbuff[1] == ' ')//When a line begins with v_ we know instantly following it will be the three vectors.
		{
			int fSize = 0;//This records the length of the media to record.

			for(int i=0; i<3;i++)
			{
				//This loop statement will read every two characters and then stop when a space is detected. This way we can test each combination of letters. 
				//For example if a number ends it will detect the whitespace and return a value for the length of the word.
				while(!checkbuff[1] == ' ')
				{
					fread(checkbuff, 1,2,pFile);
					fSize+=2;
					fseek ( pFile , SEEK_CUR-1 , SEEK_CUR );//Go back one space.
				}
				fseek ( pFile , SEEK_CUR-(fSize+1) , SEEK_CUR );//set the pointer to the beginning of the value
			
			//x = (char*) malloc (sizeof(char)*fSize);

			//at this point we should be at the beginning of a value so we record it with fscanf
			fscanf(pFile, "%f", &testP.xyz[i]);
			
			
			std::cout<<std::endl<< testP.xyz[i] ;
			
			
			}
			verticesInFile++;//Increment the number of vertexes.

			//Add vertex to vector and update the number of vertices.
			testP.verticeNumber = verticesInFile;
			vertices.push_back(testP);
			//std::cout<<vertices[i].xyz[0]<<std::endl;
			std::cout<<"success";
			
		}

		//Now we will record the faces.
		if (checkbuff[0] == 'f' && checkbuff[1] == ' ')
		{
			
			int fSize = 0;
			for(int i=0; i<3;i++)
			{
				while(!checkbuff[1] == ' ')
				{
					fread(checkbuff, 1,2,pFile);
					fSize+=2;
					fseek ( pFile , SEEK_CUR-1 , SEEK_CUR );//Go back one space.
				}
				fseek ( pFile , SEEK_CUR-(fSize+1) , SEEK_CUR );//set the pointer to the beginning of the value
				//fscanf(pFile, "%i", &testF.faceNumb);
				fscanf(pFile, "%i/%i/%i", &testF.faceNumb[i], &testF.vt[i], &testF.vn[i]); //Save the three combinations on the line in one struct
			}
			surfaces.push_back(testF);

		}
	
		fseek ( pFile , SEEK_CUR-2 , SEEK_CUR );
	}
	
	/*for(int i=0; i<vertices.size(); i++)
	{
		std::cout<<vertices[i].xyz[0]<<std::endl;
		std::cout<<vertices[i].xyz[1]<<std::endl;
		std::cout<<vertices[i].xyz[2];

	}*/
	//fscanf (pFile, "%f", &f);
    //fscanf (pFile, "%s", str);
  fclose (pFile);
}


freading::~freading(void)
{
}


std::vector <coordinate> freading::getvector()
{
	return vertices;
}

std::vector <face> freading::getfaces()
{
	return surfaces;
}