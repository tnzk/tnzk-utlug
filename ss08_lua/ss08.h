#include <math.h>

typedef struct {
  int id;
  int type;
  int x;
  int y;
  double theta;
  int age;
} TkbEnemy;

static const int NUM_ENEMY = 4;
static const int ENEMY_SIZE = 32;
static const double D_OMG = M_PI / 32.0;

static const int def_x[] = {  300,-300,  300, -300};
static const int def_y[] = {  200, 200, -200, -200};

#define RAD2DEG(rad)    (   (rad) * 180.0 / M_PI)
#define DEG2RAD(theta)  ( (theta) / 180.0 * M_PI )
#define RANDOM(min,max) (min + (int)( rand() * (min - min + 1.0) / (1.0 + RAND_MAX)))

int getfield(lua_State*, int, const char*);
void add_enminfo(lua_State*, int, TkbEnemy*);
