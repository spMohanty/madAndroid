#include "loadRAW.h"

RAW::RAW(const char* fileName,int w,int h){

width=w;
height=h;

FILE *fp;
fp=fopen(fileName,"r");
data = (unsigned char*)malloc(w*h*3);

fread(data,1,width*height*3,fp);

int count=0;

for(int y=0;y<height;y++)
for(int x=0;x<width;x++)
  {
    for(int l=0;l<3;l++)
    {
      MAP[x][y][l]=data[count++];  
//      cout<<MAP[x][y][l];
    }
    grayScale[x][y]=0.3*MAP[x][y][0]+0.59*MAP[x][y][1]+0.11*MAP[x][y][2];
  }

fclose(fp);

}
/// For testing !!
/**
main(){
RAW r("grass.raw",64,64);

for(int x=0;x<512;x++)
  for(int y=0;y<256;y++)
  {
    for(int l=0;l<3;l++)
    {
      //MAP[x][y][l]=data[count++];      
      cout<<r.MAP[x][y][l];
    }
  }
}
**/
