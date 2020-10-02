#include "LightSource.h"

LightSource::LightSource(): active(false) {}

LightSource::LightSource(Vec3f light_position, Vec3f color): light_position(light_position), color(color), active(false) {}
