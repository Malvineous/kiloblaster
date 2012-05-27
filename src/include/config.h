extern struct cf_t {
	int16_t firstthru;									// Are we set up?
	int16_t joyflag0,joyxl0, joyxc0, joyxr0, joyyu0, joyyc0, joyyd0;
	int16_t x_ourmode0;
	int16_t sb_exists0;
	int16_t musicflag0;
	int16_t vocflag0;
	} cf;

void cfg_init(int16_t argc, char *argv[]);
int16_t doconfig (void);
extern int16_t nosnd;
// extern int16_t cfgdemo;
