#include "Vec3.h"

class LightSource{
  private:
    Vec3f light_position = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f color = Vec3f(0.0f, 0.0f, 0.0f);
    bool active = false;
  public:
    LightSource();
    LightSource(Vec3f light_position, Vec3f color);
    void activeLightSource() {active = true;}
    void deactiveLightSource() {active = false;}
    Vec3f getColor() const {return color;}
    Vec3f getPosition() const {return light_position;}
    bool isActive() const {return active;}
};
