/*
 * File: life.cpp
 * --------------
 * Your name and comments go here!
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include "genlib.h"
#include "extgraph.h"
#include "simpio.h"
#include "grid.h"
#include "random.h"
#include "lifeGraphics.h"
#include "vector.h"

void Welcome();
string readignoringcomments(ifstream &file);
void randomlyplace(Grid<int> &grd, int width, int height);
void populatefromfile(ifstream &file,Grid<int> &grd, int width, int height);
string readlinefromfile(ifstream &file);
void graphicaldisplay(Grid<int> &grd, int width, int height);
void populatefromfile(ifstream &file,Grid<int> &grd, int width, int height);
int countneighbours(Grid<int> &grd,int x,int y,int width,int height);
void processgeneration(Grid<int> &grd,int width,int height);

int main()
{
	int width,height;
	ifstream file;
	string filename, answer;
	SetWindowTitle("Life!");
	InitGraphics();
	Welcome();
		cout << "You can start your colony with random cells or read from a prepared file." << endl;
		answer=filename="";
		while (1)
		{
			cout << "Do u want to seed from population file:";
			answer = GetLine();
			if (answer == "yes" || answer =="no"||answer=="YES"||answer=="NO")
				break;
			cout << "Answer yes or no...." << endl;
		}

		if (answer == "no"||answer=="NO")
		{
			cout << "Seeded randomly" << endl;
			width = 50;
			height = 50;
		} 
		else
		{
			while (1)
			{
				cout << "Enter the name of population file:";
				filename = GetLine();
				file.open(filename.c_str());
				if (!file.fail()) 
					break;
				else
					cout <<"Error opening file.Please try again."<<endl;
			}
			width = StringToInteger(readignoringcomments(file));
			height= StringToInteger(readlinefromfile(file));
		}
	Grid<int> grd(width,height);
	
	if (answer =="no")
			randomlyplace(grd,width,height);
	else
	{
			populatefromfile(file,grd,width,height);
			file.close();
	}
	graphicaldisplay(grd,width,height);
	return 0;
}
void graphicaldisplay(Grid<int> &grd, int width, int height)
{
	InitLifeGraphics(width,height);
	while (1)
	{
		UpdateDisplay();
		for (int x =0;x<width;x++)
			for (int y =0;y < height;y++)
				DrawCellAt(x,y,grd[x][y]);
	}	
}

string readignoringcomments(ifstream &file)
{
	string lineread;
	while (!file.eof())
	{
		lineread = readlinefromfile(file);
		if (lineread[0] != '#') //ignoring lines starting with # which denote bakwass by Julie.Welcome to Julie's fish aquarium.kya bakwass.kha jha tal ke saali.
			break;
	}
	return lineread;
}
void randomlyplace(Grid<int> &grd, int width, int height)
{
	Randomize();
	for (int x = 0; x < width; x++)
		for (int y = 0; y< height; y++)
			grd[x][y] = RandomInteger(0,1);//assign 0 or 1 randomly
}
void populatefromfile(ifstream &file,Grid<int> &grd, int width, int height)
{
	for (int x =0;x<width; x++)
	{													
		string readline = readlinefromfile(file);
		for (int y=0;y<strlen(readline.c_str());y++)
		{
			if(readline[y]=='-')
				grd[x][y]=0;
			else
				grd[x][y]=1;//'x' hai to one
		}	
	}
}
string readlinefromfile(ifstream &file)
{
	string returnstr;
	getline(file,returnstr, '\n');
	return returnstr;
}
void Welcome()
{
	SetPointSize(48);
	MovePen(GetWindowWidth()/2-TextStringWidth("Welcome To Life!")/2,GetWindowHeight()/2);
	DrawTextString("Welcome To Life!");
	UpdateDisplay();
	cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
	cout << "Cells live and die by the following rules:" << endl;
	cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
	cout << "\tLocations with 2 neighbors remain stable" << endl;
	cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
	cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl;
	cout << "In the animation, new cells are dark and lighten as they age." << endl;
	cout << "Hit RETURN when ready: ";
	GetLine();
}
