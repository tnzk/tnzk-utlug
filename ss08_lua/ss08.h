#include <math.h>

typedef struct {
  int id;
  int type;
  int x;
  int y;
  double theta;
} TkbEnemy;

static const int NUM_ENEMY = 2;
static const double D_OMG = M_PI / 32.0;

#define RAD2DEG(rad)   (   (rad) * 180.0 / M_PI)
#define DEG2RAD(theta) ( (theta) / 180.0 * M_PI )
