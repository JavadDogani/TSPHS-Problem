// RandomSearch_TSPHS.cpp
//	
// Author         : Javad Dogani
// Contact        : dogani.javad@gmail.com
// Date Created   : 20/02/2019
//	
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#define PATH "test.tsphs"
using namespace std;
int Readcityhotelno(int &cityno, int &hotelno, double &dailytime)
{
	fstream file;
	file.open (PATH, ios::out | ios::in );
	file>>hotelno;
	file>>cityno;
	file>>dailytime;
}
int ReadData(double** dist, double** distc2h, double* citytime)
{
	fstream file;
	file.open (PATH, ios::out | ios::in );
	int hotelno;
	int cityno;
	double dailytime;
	
	char ch;
	file>>hotelno;
	file>>cityno;
	file>>dailytime;
	double** hotelarray=(double**)malloc (hotelno*sizeof (double*));
	for (int i=0;i<hotelno;i++)
	{
		hotelarray[i]= (double*)malloc (2*sizeof (double));
	}
	for(int i=0;i<hotelno;i++)
	{
		int data;
		file>>data;
		file>>hotelarray[i][0];
		file>>hotelarray[i][1];
	}
	double** cityarray=(double**)malloc (cityno*sizeof (double*));
	for (int i=0;i<cityno;i++)
	{
		cityarray[i]= (double*)malloc (2*sizeof (double));
	}
	for(int i=0;i<cityno;i++)
	{
		int data;double ddata;
		file>>data; 
		file>>cityarray[i][0];
		file>>cityarray[i][1];
		file>>ddata;
		citytime[i]=ddata;
		//	printf ("\ncity %d  x:%lf  y:%lf  %lf",i,cityarray[i][0],cityarray[i][1], citytime[i]);			
	}
	for (int i=0;i<cityno;i++)
		dist[i]= (double*)malloc ((cityno)*sizeof (double));
	for (int i=0;i<cityno;i++)
		for (int j=0;j<cityno;j++)
		{
			if (i==j)
				dist[i][j]=0;
			else
				dist[i][j]=sqrt((cityarray[i][0]-cityarray[j][0])*(cityarray[i][0]-cityarray[j][0])+(cityarray[i][1]-cityarray[j][1])*(cityarray[i][1]-cityarray[j][1]));
		}
	for (int i=0;i<cityno;i++)
		distc2h[i]= (double*)malloc ((hotelno)*sizeof (double));
		
	for (int i=0;i<cityno;i++)
		for (int j=0;j<hotelno;j++)
			distc2h[i][j]=sqrt((cityarray[i][0]-hotelarray[j][0])*(cityarray[i][0]-hotelarray[j][0])+(cityarray[i][1]-hotelarray[j][1])*(cityarray[i][1]-hotelarray[j][1]));
		
}
int makesolution(double **dist,double **distc2h,double **citymin2h, int solution[300][50], int solutionhotel[], int solutioncitycount[], double *citytime, int cityno, int dailytime,int &tripindex, double &currenttime)
{
	int cities[cityno];
	int citycount=cityno-1;
	for (int i=0;i<cityno;i++)
		cities[i]=i;
	tripindex=0;
	int colindex=0;
	currenttime=0;
	int lastcity=-1;
	int lasthotel=0;
	while(citycount>=0)
	{
		if (lastcity==-1)
		{
			int randnum=rand()%(citycount+1);
			int cityindex=cities[randnum];
		//		printf("\n  $$$      %d   %d", randnum,cityindex);
			solution [tripindex][colindex]=cityindex;
			currenttime+=distc2h[cityindex][lasthotel]+citytime[cityindex];
		
		//	printf ("\n ---- trip:%d colindex:%d choose:%d time:%lf", tripindex,colindex,cityindex,currenttime);
			colindex++;
			cities[randnum]=cities[citycount];
			citycount--;
			lastcity=cityindex;			
		}
		else
		{
			int randnum=rand()%(citycount+1);
			int cityindex=cities[randnum];
		//	printf("\n        %d   %d", randnum,cityindex);
		//	printf("\n if  last: %d index: %d dist : %lf min2hindex:%lf  ",lastcity, cityindex, dist[lastcity][cityindex],citymin2h[cityindex][0]);
			if (currenttime+ dist[lastcity][cityindex]+citytime[cityindex]+citymin2h[cityindex][0]<=(tripindex+1)*dailytime)
			{
				solution [tripindex][colindex]=cityindex;
			    currenttime+=dist[lastcity][cityindex]+citytime[cityindex];
			    	
			  //  printf ("\n +++++ trip:%d colindex:%d choose:%d time:%lf  citycount:%d", tripindex,colindex,cityindex,currenttime,citycount);
			    lastcity=cityindex;
				colindex++;
				cities[randnum]=cities[citycount];
				citycount--;				
				if (citycount==-1)
				{
				//	printf("\n-------------end-------------");
					solutionhotel[tripindex]=0;
					solutioncitycount[tripindex]=colindex;
				//	int t=(int)citymin2h[lastcity][1];
					currenttime+=distc2h[lastcity][0];
				}				
			}
			else// no other path found and go to hotel
			{
				solutionhotel[tripindex]=(int)citymin2h[lastcity][1];
				currenttime+=citymin2h[lastcity][0];
			//	printf ("\n *** trip:%d colindex:%d hotel:%d time:%lf", tripindex,colindex,(int)citymin2h[lastcity][1],currenttime);
				solutioncitycount[tripindex]=colindex;					
				lasthotel=citymin2h[lastcity][1];
				tripindex++;
				colindex=0;				
				lastcity=-1;
				
			}
		}
	}
	
}

void PrintSolution(int solution[300][50],int solutionhotel[], int solutioncitycount[],int tripindex)
{
	printf("\n");
	for (int i=0;i<=tripindex;i++)
	{
		for (int j=0;j<solutioncitycount[i];j++)
			printf("\t %d", solution[i][j]);
		printf("\t Hotel %d",solutionhotel[i] );
		printf("\n");
	}
}

void PrintSolutionTime(double **dist,double **distc2h,int solution[300][50],int solutionhotel[], int solutioncitycount[],int tripindex)
{
	printf("\n Solution Time:");
	double time=0;
	for (int i=0;i<=tripindex;i++)
	{	
		printf("\n trip %d:",i);
		if (i==0)
		{
			printf(" h0: %.2lf", distc2h[solution[i][0]][0]);time+=distc2h[solution[i][0]][0];
		}
		else
		{
			printf(" h%d: %.2lf",solutionhotel[i-1], distc2h[solution[i][1]][solutionhotel[i-1]]);
			time+=distc2h[solution[i][1]][solutionhotel[i-1]];
		}
			int j;
		
		for ( j=1;j<solutioncitycount[i];j++)
		{
			printf("  %.2lf", dist[solution[i][j-1]][solution[i][j]]);
			time+=dist[solution[i][j-1]][solution[i][j]];
		}
		printf(" h%d: %.2lf",solutionhotel[i], distc2h[solution[i][j-1]][solutionhotel[i]]);
		time+=distc2h[solution[i][j-1]][solutionhotel[i]];
		printf("   ime: %lf:\n",time);
	}
}

int main()
{
	int iteration=20000000;
	int timelimit=-1;
	srand (time(NULL));
	int hotelno;
	int cityno;
	double dailytime;
	Readcityhotelno(cityno,hotelno, dailytime);
	printf("%d %d %lf",cityno,hotelno, dailytime);
	double **dist=(double**)malloc (cityno*sizeof (double*));
	double **distc2h=(double**)malloc (cityno*sizeof (double*));	
	double *citytime=(double*) malloc (cityno*sizeof (double));
	ReadData(dist,distc2h, citytime);	
	double **citymin2h=(double**)malloc (cityno*sizeof (double*));
	for (int i=0;i<cityno;i++)
	{
		citymin2h[i]=(double*)malloc(2*sizeof(double));
		double min= distc2h[i][0];
		int index=0;
		for (int j=1;j<hotelno;j++)
			if (distc2h[i][j]<min)
			{			
				min=distc2h[i][j];
				index=j;
			}
			citymin2h[i][0]=min;
			citymin2h[i][1]=(double)index;
	}
				
	clock_t begin = clock();	
	int bestsolutiontrip=1000000;
	double bestsolutuiontime=9999999.0;
	clock_t end=clock();
	if (timelimit!=-1)
	{
		for (int i=0;(end - begin) / CLOCKS_PER_SEC<timelimit;i++)
		{
			int solution[300][50];
			int solutionhotel[300];
			int solutioncitycount[300];
			int tripindex=0;
			double currenttime=0;
			makesolution(dist,distc2h,citymin2h, solution,solutionhotel, solutioncitycount,citytime, cityno, dailytime,tripindex,currenttime);	
			PrintSolution(solution,solutionhotel,solutioncitycount,tripindex);
		//	printf(" \n...... %d.......%lf", tripindex,currenttime);
			if (tripindex+1<bestsolutiontrip)
			{
				bestsolutiontrip=tripindex+1;
				bestsolutuiontime=currenttime;
			}
			else if (((tripindex+1)==bestsolutiontrip)&&(currenttime<bestsolutuiontime))
			{
				bestsolutuiontime=currenttime;
			}
			end = clock();
		}
	}
	else
	{
		for (int i=0;i<iteration;i++)
		{
			int solution[300][50];
			int solutionhotel[300];
			int solutioncitycount[300];
			int tripindex=0;
			double currenttime=0;
			makesolution(dist,distc2h,citymin2h, solution,solutionhotel, solutioncitycount,citytime, cityno, dailytime,tripindex,currenttime);	
			
			if (tripindex+1<bestsolutiontrip)
			{
				bestsolutiontrip=tripindex+1;
				bestsolutuiontime=currenttime;
			
			}
			else if (((tripindex+1)==bestsolutiontrip)&&(currenttime<bestsolutuiontime))
			{
				bestsolutuiontime=currenttime;
			}
		
		}		
	}
	end = clock();
	printf("\n\n Best Solution:\n \t %d    %lf", bestsolutiontrip,bestsolutuiontime);
	printf("\n Time: %d Second", (end - begin) / CLOCKS_PER_SEC);
		return 0;
}


