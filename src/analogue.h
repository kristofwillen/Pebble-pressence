#pragma once
#include "pebble.h"

  
static const GPathInfo MINUTE_HAND_POINTS = {
  5,
  (GPoint []) {
    {-5, 0 },
    { 0, 10 },
    { 5, 0 },
    {2,-42},
    {-2,-42}  
   }
};

static const GPathInfo HOUR_HAND_POINTS = {
  5, (GPoint []){
    {-4, 0},
    {0, 8},
    {4, 0},
    {1,-25},
    {-1,-25}
  }
};

static const GPathInfo FUEL_HAND_POINTS = {
  4, (GPoint []){
    {-4, 0},
    {0, 5},
    {4, 0},
    {0,-20}
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
