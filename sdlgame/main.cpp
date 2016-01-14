#include <SDL.h>
#include <math.h>
#include <sstream>
#include <string.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include<vector>
#include "freading.h"

//To DO: fix trnaslation matrix. Currently applys both the x and y at the same time.

#define mapWidth 24
#define mapHeight 24
#define sWidth 640
#define sHeight 480
const std::string dubstoStr(double x);

SDL_Window* g_pWindow =0;
SDL_Renderer* g_pRenderer = 0;



struct mat4
{
	float cel[16];
};

mat4 newmat(int Tx,int Ty,int Tz,float sca, float rotx, float roty, float rotz);

coordinate operator*(const mat4& mat, const coordinate& b)
{
	coordinate pos;
	
			pos.xyz[0]= (b.xyz[0]*mat.cel[0]) + (b.xyz[1]*mat.cel[1]) + (b.xyz[2]*mat.cel[2]) + (1*mat.cel[3]);
			pos.xyz[1]= (b.xyz[0]*mat.cel[4]) + (b.xyz[1]*mat.cel[5]) + (b.xyz[2]*mat.cel[6]) + (1*mat.cel[7]);
			pos.xyz[2]= (b.xyz[0]*mat.cel[8]) + (b.xyz[1]*mat.cel[9]) + (b.xyz[2]*mat.cel[10]) + (1*mat.cel[11]);
			//pos.xyz[0]= (b.xyz[0]*mat.cel[12]) + (b.xyz[1]*mat.cel[13]) + (b.xyz[2]*mat.cel[14]) + (1*mat.cel[12]);
		return pos;
}
//
mat4 operator*(const mat4& mat, const mat4& matT)		//matrices multiplication. for concatanations
{
	mat4 temp;
	temp.cel[0] = (mat.cel[0]*matT.cel[0]) + (mat.cel[1]*matT.cel[4]) + (mat.cel[2]*matT.cel[8]) + (mat.cel[3]*matT.cel[12]);
	temp.cel[1] = (mat.cel[0]*matT.cel[1]) + (mat.cel[1]*matT.cel[5]) + (mat.cel[2]*matT.cel[9]) + (mat.cel[3]*matT.cel[13]);
	temp.cel[2] = (mat.cel[0]*matT.cel[2]) + (mat.cel[1]*matT.cel[6]) + (mat.cel[2]*matT.cel[10]) + (mat.cel[3]*matT.cel[14]);
	temp.cel[3] = (mat.cel[0]*matT.cel[3]) + (mat.cel[1]*matT.cel[7]) + (mat.cel[2]*matT.cel[11]) + (mat.cel[3]*matT.cel[15]);

	temp.cel[4] = (mat.cel[4]*matT.cel[0]) + (mat.cel[5]*matT.cel[4]) + (mat.cel[6]*matT.cel[8]) + (mat.cel[7]*matT.cel[12]);
	temp.cel[5] = (mat.cel[4]*matT.cel[1]) + (mat.cel[5]*matT.cel[5]) + (mat.cel[6]*matT.cel[9]) + (mat.cel[7]*matT.cel[13]);
	temp.cel[6] = (mat.cel[4]*matT.cel[2]) + (mat.cel[5]*matT.cel[6]) + (mat.cel[6]*matT.cel[10]) + (mat.cel[7]*matT.cel[14]);
	temp.cel[7] = (mat.cel[4]*matT.cel[3]) + (mat.cel[5]*matT.cel[7]) + (mat.cel[6]*matT.cel[11]) + (mat.cel[7]*matT.cel[15]);

	temp.cel[8] = (mat.cel[8]*matT.cel[0]) + (mat.cel[9]*matT.cel[4]) + (mat.cel[10]*matT.cel[8]) + (mat.cel[11]*matT.cel[12]);
	temp.cel[9] = (mat.cel[8]*matT.cel[1]) + (mat.cel[9]*matT.cel[5]) + (mat.cel[10]*matT.cel[9]) + (mat.cel[11]*matT.cel[13]);
	temp.cel[10] = (mat.cel[8]*matT.cel[2]) + (mat.cel[9]*matT.cel[6]) + (mat.cel[10]*matT.cel[10]) + (mat.cel[11]*matT.cel[14]);
	temp.cel[11] = (mat.cel[8]*matT.cel[3]) + (mat.cel[9]*matT.cel[7]) + (mat.cel[10]*matT.cel[11]) + (mat.cel[11]*matT.cel[15]);

	temp.cel[12] = (mat.cel[12]*matT.cel[0]) + (mat.cel[13]*matT.cel[4]) + (mat.cel[14]*matT.cel[8]) + (mat.cel[15]*matT.cel[12]);
	temp.cel[13] = (mat.cel[12]*matT.cel[1]) + (mat.cel[13]*matT.cel[5]) + (mat.cel[14]*matT.cel[9]) + (mat.cel[15]*matT.cel[13]);
	temp.cel[14] = (mat.cel[12]*matT.cel[2]) + (mat.cel[13]*matT.cel[6]) + (mat.cel[14]*matT.cel[10]) + (mat.cel[15]*matT.cel[14]);
	temp.cel[15] = (mat.cel[12]*matT.cel[3]) + (mat.cel[13]*matT.cel[7]) + (mat.cel[14]*matT.cel[11]) + (mat.cel[15]*matT.cel[15]);
	return temp;

};
//coordinate operator+(const coordinate& b)
//{
//	coordinate newCoord;
//}

int i;
SDL_Event event;
int main(int argc, char* args[])
{

	float movex=0 , movey=0 , movez=0;
	movex=0;
	movey=0;
	movez=0;
	float rotx = 0, roty = 0, rotz = 0;


	int k=0;
	float scale = 1;
	
	//object.push_back(coordinate());
	freading objo("raiden.obj");
	std::vector<coordinate> object(objo.getvector());
	std::vector<face>  linesToDraw(objo.getfaces());

	//object = objo.getvector();

	/*std::string line, v;
	std::ifstream myfile ("cube10.obj");
	if (myfile.is_open())
	{
    while ( getline (myfile,line) )
    {
		object.push_back(coordinate());
		std::istringstream iss( line );
		if(line[0] == 'v')
		{
			iss >> v >> object[k].xyz[0]>> object[k].xyz[1]>> object[k].xyz[2];//Currently it only works for cubes because of the design of the rest of my code.
		//cout << k << "\t" << xco[k] << "\t" << yco[k] << "\t" << zco[k] << endl;
		k++;
		
		}
    }
	//cout<<xco[0];
	myfile.close();
	}*/


	bool quitFunc = false;
	//initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		//If suceeded create our window
		g_pWindow = SDL_CreateWindow("Chapter 1: setting up SDL",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN);

		//If window creation succeedede create our renderer
		if(g_pWindow != 0)
		{
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);

		}
	}
	else
	{
		return 1; //sdl could not initialize
	}
	// everything succeeded lets draw the window
	// set to black // This function expects Red, Green, Blue and
	// Alpha as color values
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

	

	while (quitFunc == false)
	{
		movex= 0;
		movey= 0;
		movez= 0;
		rotx = 0;
		roty = 0;
		rotz = 0;
		scale = 5;
		// clear the window to black
		SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

		SDL_RenderClear(g_pRenderer);

		SDL_SetRenderDrawColor(g_pRenderer, 41, 255, 0, 255);
		//SDL_RenderDrawLine(g_pRenderer, 50,50,100,100);
			

		for(i=0; i<object.size(); i++)
		{
			
			SDL_RenderDrawPoint(g_pRenderer, object[i].xyz[0],object[i].xyz[1]);
			
							if(i != object.size()-1)
							{
							//SDL_RenderDrawLine(g_pRenderer,object[i].xyz[0], object[i].xyz[1],object[i+1].xyz[0], object[i+1].xyz[1]) ;	
							}
							
		}
	
		for (int k=0; k<linesToDraw.size();k++)
		{
			
			SDL_RenderDrawLine(g_pRenderer,object[linesToDraw[k].faceNumb[0] - 1].xyz[0], object[linesToDraw[k].faceNumb[0] - 1].xyz[1],object[linesToDraw[k].faceNumb[1] - 1].xyz[0], object[linesToDraw[k].faceNumb[1] - 1].xyz[1]);
			SDL_RenderDrawLine(g_pRenderer,object[linesToDraw[k].faceNumb[1] - 1].xyz[0], object[linesToDraw[k].faceNumb[1] - 1].xyz[1],object[linesToDraw[k].faceNumb[2] - 1].xyz[0], object[linesToDraw[k].faceNumb[2] - 1].xyz[1]);
			SDL_RenderDrawLine(g_pRenderer,object[linesToDraw[k].faceNumb[2] - 1].xyz[0], object[linesToDraw[k].faceNumb[2] - 1].xyz[1],object[linesToDraw[k].faceNumb[0] - 1].xyz[0], object[linesToDraw[k].faceNumb[0] - 1].xyz[1]);
			
		}
	

			while( SDL_PollEvent( &event ) ){
			switch( event.type ){
				/* Look for a keypress */
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch( event.key.keysym.sym )
				{
				case SDLK_ESCAPE:
					quitFunc = true;
					break;
				case SDLK_LEFT:
					{
						
						movex=-1;/*
						movement.cel[3] = movex;
						movement.cel[7] = movey;
						movement.cel[11] = movez;*/
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];//You apply the translation here. But you don't know where you SHould apply.
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
					}
					break;
				case SDLK_RIGHT:
					{
						
						movex=1;/*
						movement.cel[3] = movex;
						movement.cel[7] = movey;
						movement.cel[11] = movez;*/
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						


					}
					break;
				case SDLK_UP:
					{
					
						movey=-1;
					
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];//You apply the translation here. But you don't know where you SHould apply.
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}

					}
					break;
				case SDLK_DOWN:
					{
					
						movey=1;
						
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];//You apply the translation here. But you don't know where you SHould apply.
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
					}
					break;
				case SDLK_SPACE:
					{
						scale= 2.25;
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];//You apply the translation here. But you don't know where you SHould apply.
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
					}
					break;
					case SDLK_b:
					{
						scale= 0.75;
						for(i=0; i<object.size(); i++)
						{
							//object[i] = movement * object[i];//You apply the translation here. But you don't know where you SHould apply.
							object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
					}
					break;
				case SDLK_d:
					{
					rotx=1;
					for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
					}
					}
					break;
				case SDLK_e:
					{
						rotx=-1;
						for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						}
					break;
				case SDLK_s:
					{
						roty=-1;
						for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						}
					break;
				case SDLK_w:
					{
						roty=1;
						for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						}
					break;
					case SDLK_q:
					{
						rotz=1;
						for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						}
					break;
					case SDLK_a:
					{
						rotz=-1;
						for(i=0; i<object.size(); i++)
						{
						object[i] = newmat(movex, movey, movez, scale, rotx, roty, rotz) * object[i];
						}
						}
					break;


				}
			}
			}

			



		// show the window
		SDL_RenderPresent(g_pRenderer);

		// set a delay before quitting
		//SDL_Delay(1);
		// clean up SDL

		i++;
	}
	SDL_Quit();
	return 0;
}


const std::string dubstoStr(double x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

mat4 newmat(int Tx, int Ty, int Tz, float sca, float rotX, float rotY, float rotZ)
{
	mat4 newMatrix;
	mat4 rotationMatX;
	mat4 rotationMatY;
	mat4 rotationMatZ;
	mat4 scale;

	mat4 finalMat;

	for (int i=0; i<16; i++)
	{
		
		newMatrix.cel[i] = 0;
		rotationMatX.cel[i] = 0;
		rotationMatY.cel[i] = 0;
		rotationMatZ.cel[i] = 0;
		finalMat.cel[i] = 0;
		scale.cel[i] = 0;


	}

	newMatrix.cel[3] = Tx;
	newMatrix.cel[7] = Ty;
	newMatrix.cel[11] = Tz;
	newMatrix.cel[0] = 1;
	newMatrix.cel[5] = 1;
	newMatrix.cel[10] = 1;
	newMatrix.cel[15] = 1;

	

	rotationMatX.cel[0] = 1;
	rotationMatX.cel[5] = cos(rotX);
	rotationMatX.cel[6] = sin(rotX);
	rotationMatX.cel[9] = -sin(rotX);
	rotationMatX.cel[10] = rotationMatX.cel[5];
	rotationMatX.cel[15] = 1;

	finalMat = newMatrix * rotationMatX;

	rotationMatY.cel[5] = 1;
	rotationMatY.cel[0] = cos(rotY);
	rotationMatY.cel[2] = -sin(rotY);
	rotationMatY.cel[8] = sin(rotY);
	rotationMatY.cel[10] = rotationMatY.cel[0];
	rotationMatY.cel[15] = 1;

	finalMat = finalMat * rotationMatY;

	rotationMatZ.cel[10] = 1;
	rotationMatZ.cel[0] = cos(rotZ);	
	rotationMatZ.cel[4] = -sin(rotZ);
	rotationMatZ.cel[1] = sin(rotZ);
	rotationMatZ.cel[5] = rotationMatZ.cel[0];
	rotationMatZ.cel[15] = 1;

	finalMat = finalMat * rotationMatZ;

	scale.cel[0] = sca;
	scale.cel[5] = sca;
	scale.cel[10] = sca;
	scale.cel[15] = 1;

	finalMat = finalMat * scale;

	
	return finalMat;
}