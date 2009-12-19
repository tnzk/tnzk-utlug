#include <math.h>

typedef struct {
  int id;       // accessable
  int type;     // accessable
  int x;        // accessable
  int y;        // accessable
  double theta; // accessable
  int age;
  int attacked;
  int heat;
  int bital;
} TkbEnemy;

static const int NUM_ENEMY = 4;
static const int ENEMY_SIZE = 32;
static const double D_OMG = M_PI / 32.0;
static const int OLD_AGE = 40;

static const int def_x[] = {  300,-300,  300, -300};
static const int def_y[] = {  200, 200, -200, -200};

#define RAD2DEG(rad)    (   (rad) * 180.0 / M_PI)
#define DEG2RAD(theta)  ( (theta) / 180.0 * M_PI )
#define RANDOM(min,max) (min + (int)( rand() * (min - min + 1.0) / (1.0 + RAND_MAX)))
#define IS_OLD(enm) ((enm)->age > OLD_AGE)

int getfield(lua_State*, int, const char*);
void add_enminfo(lua_State*, int, TkbEnemy*);
void dump_stack( lua_State*);
