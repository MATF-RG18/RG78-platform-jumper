//sve globalne promenljive
static float x_cam=0;
static float dx=0.1;

static float y_cam=0.3;
//visina po y osi
static float y_jump=0;

static float jump_speed = 0.5;
static float g=0.07;
static float t=0;


static int movement_flag=0;
static int jump_flag=0;
static int fall_flag=0;


static int num;
static int floor=0;
static int next_floor=0;
static int prev_floor=0;
//najmanji i najveci x
static float min_floor;
static float max_floor;
static int floor_counter=0;
static float death_floor;
static int death_flag=0;


static int change_floors=1;


//trenutne pozicije ivica bloka na kome smo
static float curr_x_r;
static float curr_x_l;
static float next_x_l;
static float prev_x_r;


float* data;
