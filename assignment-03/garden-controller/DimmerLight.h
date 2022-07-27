#ifndef __DIMMER_LIGHT__
#define __DIMMER_LIGHT__

class DimmerLight {
 
public:
  virtual void setIntensity(int intensity) = 0;
  virtual int getIntensity() = 0;
};

#endif