





class World{
	protected:
	float current_state[2];
	void make_injury();
	public:
	int px, py;
	int tx, ty;
	int dx, dy;
	float mx, my;
	int hit_count = 0;
	int step_cntr = 0;
	
	
	void rand_target();
	void update_dist();
	float step(float* actions);//returns a reward, actions is a continouis action space
	float* state();//returns the state, size = 2
	void reset();//resets world
	
	
	World();
	
};




