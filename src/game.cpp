#include "game.hpp"
#include "math.h"
#include <iostream>
#include <random>



World::World(){
	reset();
	
	make_injury();
	
}
void World::rand_target(){
	tx = (rand()%200)-100;
	ty = (rand()%200)-100;
}

void World::update_dist(){
	if (px < -100){
		px = -100;
	}else if(px>100){
		px = 100;
	}
	if (py < -100){
		py = -100;
	}else if (py >100){
		py = 100;
	}
	
	dx = tx-px;
	dy = ty-py;
}

float* World::state(){
	
	current_state[0] = (float)(dx)/100.0;
	current_state[1] = (float)(dy)/100.0;
	//current_state[2] = (float)(px)/100.0; 
	//current_state[3] = (float)(py)/100.0;
	//current_state[4] = (float)(tx)/100.0;
	//current_state[5] = (float)(ty)/100.0;
	return &current_state[0];
}

float World::step(float* actions){
	step_cntr++;
	
	
	
	px += std::round(*(actions+0)*100.0*mx);
	py += std::round(*(actions+1)*100.0*my);
	float reward = 0.0f;
	int dx_history = dx;
	int dy_history = dy;
	update_dist();
	/*
	if ((*(actions+0)!=*(actions+0))||(*(actions+1)!=*(actions+1))){
		reward-=150.0f;
	}
	*/
	if (std::abs(*(actions+0))>2.0){
		reward-=std::abs(dx);
	}
	if (std::abs(*(actions+1))>2.0){
		reward-=std::abs(dy);
	}
	if (abs(dx) <abs(dx_history)){
		reward += .1f;
	}else{
		reward-=.15f;
	}
	if (abs(dy)<abs(dy_history)){
		reward+=.1f;
	}else{
		reward-=.15f;
	}
	if (abs(dx)+abs(dy) <= 1){
		reset();
		hit_count++;
		reward += 5.0f;
	}
	
	return reward;
}
void World::make_injury(){
	float random_num = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
	//std::cout<<"rand: "<<random_num<<"\n";
	if (random_num > .5){
		mx = -1.0;
		my = 1.0;
	}else{
		mx = 1.0;
		my = 1.0;
	}
}

void World::reset(){
	px = 10;
	py = 10;
	step_cntr = 0;
	rand_target();
	update_dist();
	make_injury();
}



