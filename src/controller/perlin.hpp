#ifndef ocd_map_perlin
#define ocd_map_perlin
#include <math.h>
class perlinNoise3d{
  public:
  int seed;
  int num;
  double step;
  double height;
  perlinNoise3d(){
    num=4;
    step=8;
    height=1;
  }
  virtual int randint(int x,int y,int z){
    return (((x*48271+y&0x23456+z^98704)*seed)&0xfffffff)/10000.0d;
  }
  virtual double rand(int x,int y,int z){
    return randint(x,y,z)/10000.0d;
  }
  double smooth(int x,int y,int z){
    double t;
    t =rand(x,y,z)/2+rand(x+1,y,z)/4+rand(x-1,y,z)/8;
    t+=rand(x,y,z)/2+rand(x,y+1,z)/4+rand(x,y-1,z)/8;
    t+=rand(x,y,z)/2+rand(x,y,z+1)/4+rand(x,y,z-1)/8;
    return t;
  }
  double inoise(double x,double y,double z){
    int x0=(int)x;
    int x1=x0+1;
    int y0=(int)y;
    int y1=y0+1;
    int z0=(int)z;
    int z1=z0+1;
    double v1=smooth(x0,y0,z0);
    double v2=smooth(x1,y0,z0);
    double v3=smooth(x0,y1,z0);
    double v4=smooth(x1,y1,z0);
    double v5=smooth(x0,y0,z1);
    double v6=smooth(x1,y0,z1);
    double v7=smooth(x0,y1,z1);
    double v8=smooth(x1,y1,z1);
    double i1=interpolate(v1,v2,y-y0);
    double i2=interpolate(v3,v4,y-y0);
    double i3=interpolate(v5,v6,y-y0);
    double i4=interpolate(v7,v8,y-y0);
    i1=interpolate(i1,i2,z-z0);
    i3=interpolate(i3,i4,z-z0);
    return interpolate(i1,i3,x-x0);
  }
  double noise(double x,double y,double z){
    double  total=0.0f;
    int     n    =this->num;
    double  p    =this->height;

    int     f    =this->step;
    double  a    =1;
    int     i;
    for(i=0;i<n;i++){
        total+=inoise(x*f,y*f,z*f)*a;
        f=f*2;
        a=a*p;
    }
    return total;
  }
  virtual double interpolate(double a,double b,double x){
    double ft=x*3.1415926535;
    double f=(1-cos(ft))*0.5;
    return a*(1-f)+b*f;
  }
};
#endif