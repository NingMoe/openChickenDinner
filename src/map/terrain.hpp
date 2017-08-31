#ifndef ocd_map_terrain
#define ocd_map_terrain
#include "perlin.hpp"
class terrain{
  public:
  perlinNoise3d noise;
  double getHight(double x,double y){
    double a=noise.noise(x/100,y/100,10);
    double b=noise.noise(x/1000,y/1000,10000);
    return a>b?a:b;
  }
};
#endif