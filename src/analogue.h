#pragma once
#include "pebble.h"

  
static const GPathInfo MINUTE_HAND_POINTS = {
  5,
  (GPoint []) {
    {-5, 0 },
    { 0, 10 },
    { 5, 0 },
    {2,-60},
    {-2,-60}  
   }
};

static const GPathInfo HOUR_HAND_POINTS = {
  5, (GPoint []){
    {-4, 0},
    {0, 8},
    {4, 0},
    {1,-30},
    {-1,-30}
  }
};

static const GPathInfo FUEL_HAND_POINTS = {
  4, (GPoint []){
    {-5, 0},
    {0, 5},
    {5, 0},
    {0,-25}
  }
};

static const GPathInfo WEEK_HAND_POINTS = {
  4, (GPoint []){
    {-5, 0},
    {0, 5},
    {5, 0},
    {0,-25}
  }
};
