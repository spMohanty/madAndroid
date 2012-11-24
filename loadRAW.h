#ifndef LOAD_RAW
#define LOAD_RAW

#include<iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;



class RAW{
public:
int width;
int height;
unsigned char *data;

char MAP[1100][1100][3];
char grayScale[1100][1100];

RAW(const char* fileName, int w, int h);

};


#endif //LOAD_RAW
