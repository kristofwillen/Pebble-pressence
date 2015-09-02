#pragma once
#include "pebble.h"

  
static const GPathInfo MINUTE_HAND_POINTS = {
  5,
  (GPoint []) {
    {-5, 0 },
    { 0, 10 },
    { 5, 0 },
    {1,-60},
    {-1,-60}  
   }
};

static const GPathInfo HOUR_HAND_POINTS = {
  5, (GPoint []){
    {-5, 0},
    {0, 8},
    {5, 0},
    {1,-32},
    {-1,-32}
  }
};

static const GPathInfo FUEL_HAND_POINTS = {
  4, (GPoint []){
    {-4, 0},
    {0, 5},
    {4, 0},
    {0,-22}
  }
};

static const GPathInfo WEEK_HAND_POINTS = {
  4, (GPoint []){
    {-5, 0},
    {0, 5},
    {5, 0},
    {0,-22}
  }
};
