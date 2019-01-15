void make_background(GLuint name1,GLuint name2,float min_x,float max_x,float min_y,float max_y,float far_z,float near_z);

void make_player(void);


/*ucitaj podatke za podloge(leva ivica, desna ivica, na kojoj je visini) i pronadji krajnje ivice nivoa*/
void init_level(int* num, float** data , float* min_floor, float* max_floor);


void make_block(GLuint name,float min_x, float max_x, float floor_y);

static void init_lights(void);
