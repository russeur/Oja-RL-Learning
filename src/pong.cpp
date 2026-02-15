#include "pong.hpp"
#include <iostream>
#include <random>
#include <cmath>



void Paddle::move(Vector bound, Vector friction){
	//update vel and position
	vel+=accel;
	pos+=vel;
	
	//clip pos in bounds
	if (pos.x<0){
		pos.x = 0;
		vel.x = 0;
	}else if(pos.x + w > bound.x){
		pos.x = bound.x-w;
		vel.x = 0;
	}
	if (pos.y<0){
		pos.y = 0;
		vel.y = 0;
	}else if(pos.y + h > bound.y){
		pos.y = bound.y-h;
		vel.y = 0;
	}
	
	vel *= friction;
}

bool Ball::is_colliding(Paddle* p){
	return (pos.x < p->pos.x+p->w) && (pos.x + w > p->pos.x) && (pos.y < p->pos.y+p->h) && (pos.y + h > p->pos.y);
}
void Ball::bounce_on_paddle(Paddle* p){
	//std::cout<<"bounce\n";
	
	float paddle_mid = p->pos.x+(p->w / 2);
	float ball_mid = pos.x + (w/2 );
	float total_vel = std::fabs(vel.x)+std::fabs(vel.y);
	
	float distance = (ball_mid - paddle_mid) / (p->w / 1.5f);
	
	vel.x = total_vel*distance;
	
	if (vel.y > 0) vel.y = -(total_vel-std::fabs(vel.x));
	else if (vel.y < 0) vel.y = total_vel-std::fabs(vel.x);
}

void Ball::move(Vector bound, Vector friction){
	//bounce off paddles
	
	//update pos
	pos+= vel;
	
	
	
	//clip and bounce on bounds
	
	if (pos.x<0){
		pos.x = 0;
		vel.x *= -1;
	}else if(pos.x + w > bound.x){
		pos.x = bound.x-w;
		vel.x *= -1;
	}
	if (pos.y<0){
		pos.y = 0;
		vel.y  = 0;
	}else if(pos.y + h > bound.y){
		pos.y = bound.y-h;
		vel.y = 0;
	}
	
}


void World::ball_paddles_bounce(){
	if (ball.pos.y < bounds.y/2){
		if (!paddle_already_bounced_ball){
			
			if (ball.is_colliding(&p1)){
				ball.bounce_on_paddle(&p1);
				paddle_just_bounced_ball = true;
				paddle_already_bounced_ball = true;
			}
		}else{
			paddle_just_bounced_ball = false;
			paddle_already_bounced_ball = ball.is_colliding(&p1);
		}
	}else {
		if (!paddle_already_bounced_ball){
			
			if (ball.is_colliding(&p2)){
				ball.bounce_on_paddle(&p2);
				paddle_just_bounced_ball = true;
				paddle_already_bounced_ball = true;
			}
		}else{
			paddle_just_bounced_ball = false;
			paddle_already_bounced_ball = ball.is_colliding(&p2);
		}
	}
		
		
}

void World::simple_ai_move(float* action, bool is_p1){
	if (is_p1){
		
		if (p1.pos.x + (p1.w/2) <ball.pos.x){
			*action = 1;//ball to right, accel to right
		}else if (p1.pos.x + (p1.w/2)> ball.pos.x){
			*action = -1;//ball to left, accel to left
		}else{
			*action = 0;//ball at pos, accel 0
		}
	}else{
		if (p2.pos.x + (p2.w/2) <ball.pos.x){
			*action = 1;//ball to right, accel to right
		}else if (p2.pos.x + (p2.w/2)> ball.pos.x){
			*action = -1;//ball to left, accel to left
		}else{
			*action = 0;//ball at pos, accel 0
	}}
}

void World::step(float* actions){
	*(rewards+0) = 0;
	*(rewards+1) = 0;
	//p1 movement
	p1.accel.x = *actions;
	
	//p2 movement
	p2.accel.x = *(actions+1);
	
	
	if (std::isnan(*actions) || std::isnan(*(actions+1))){
		std::cout<<"ACTION NANN\n";
	}
	//std::cout<<"accel: "<<p2.accel.x<<"\n";
	
	p1.move(bounds, friction);
	p2.move(bounds, friction);
	ball.move(bounds, friction);
	ball_paddles_bounce();
	
	//give fake rewards, and find bound_ball_intercept
	
	
	if (ball.vel.y < 0){//ball going towards player one
		float when_ball_intercept = (bounds.y - ball.pos.y- (bounds.y - p1.pos.y))/ball.vel.y;
		float unbound_ball__intercept = ball.vel.x*when_ball_intercept + ball.pos.x;
		bound_ball_intercept = (float)(-std::abs((std::abs((int)(unbound_ball__intercept)) % (int)(bounds.x * 2)) - bounds.x) + bounds.x);
		float diff = bound_ball_intercept - p1.pos.x - (p1.w/2.0);
		float ball_diff = ball.pos.x- p1.pos.x - (p1.w/2.0);
		// *(rewards+0) -= std::fabs((std::fabs(diff)/bounds.x) / (std::fabs(p1.vel.x)+1.0)) * std::fabs((std::fabs(diff)/bounds.x) / (std::fabs(p1.vel.x)+1.0));
		/*
		*(rewards+0) -= (std::fabs(diff)/bounds.x);
		if ((diff > 0 && p1.vel.x > 0) || (diff < 0 && p1.vel.x < 0)){
			//std::cout<<"add\n";
			*(rewards+0) += ((std::fabs(p1.accel.x)+.75)*(std::fabs(p1.accel.x)+.75));
		}else{
			*(rewards+0)-= ((std::fabs(p1.accel.x)+.75)*(std::fabs(p1.accel.x)+.75)) * 2;
		}
		*/
	}
	else if (ball.vel.y > 0){//ball going towards player two
		float when_ball_intercept = (bounds.y - ball.pos.y- (bounds.y - p2.pos.y))/ball.vel.y;
		float unbound_ball__intercept = ball.vel.x*when_ball_intercept + ball.pos.x;
		bound_ball_intercept = (float)(-std::abs((std::abs((int)(unbound_ball__intercept)) % (int)(bounds.x * 2)) - bounds.x) + bounds.x);
		float diff = bound_ball_intercept - p2.pos.x - (p2.w/2.0);
		float ball_diff = ball.pos.x- p2.pos.x - (p2.w/2.0);
		//*(rewards+1) -= std::fabs((std::fabs(diff)/bounds.x) / (std::fabs(p2.vel.x) + 1.0)) * std::fabs((std::fabs(diff)/bounds.x) / (std::fabs(p2.vel.x)+1.0));
		
		/*
		*(rewards+1) -= (std::fabs(diff)/bounds.x);
		
		if ((diff > 0 && p2.vel.x > 0) || (diff < 0 && p2.vel.x < 0)){
			//std::cout<<"add\n";
			*(rewards+1) += ((std::fabs(p2.accel.x)+.75)*(std::fabs(p2.accel.x)+.75));
		}else{ 
			*(rewards+1)-=  std::fabs(p2.vel.x);
		}
		*/
	}
	//std::cout<<"r0: "<<*(rewards+0)<<", r2: "<<*(rewards+1)<<"\n";
	//give concrete rewards
	if (ball.vel.y == 0){//if ball vel is every equal to 0, it means that someone one that match(got past someones paddle)
		if (ball.pos.y < bounds.y/2){//if hit player one's end
			*(rewards+0) -=5.0f;
			*(rewards+1) +=5.0f;
			p2_win++;
		}else{//if hit player two's end
			*(rewards+0) +=5.0f;
			*(rewards+1) -=5.0f;
			p1_win++;
		}
		reset();
	}
	
	if (std::isnan(*(rewards+0)) || std::isnan(*(rewards+1))){
		std::cout<<"NANNN REWARD";
	}
	
}
float* World::state(){
	//player one
	//ball.pos.x, ball.pos.y, ball.vel.x, ball.vel.y, p2.pos.x, p2.vel.x, p1.vel.x, bound_ball_intercept
	current_state[0] = (ball.pos.x - p1.pos.x) / (bounds.x);
	current_state[1] =  - (ball.pos.y - (bounds.y/2.0)) / (bounds.y / 2.0);
	current_state[2] = ball.vel.x / 20.0;
	current_state[3] =  - ball.vel.y / 20.0;
	current_state[4] = 0;//(p2.pos.x - p1.pos.x) / (bounds.x);
	current_state[5] = p2.vel.x / 30.0;
	current_state[6] = p1.vel.x / 30.0;
	if (ball.vel.y < 0)current_state[7] = (bound_ball_intercept - p1.pos.x) / bounds.x;
	else current_state[7] = 0.0;
	
	
	//player two
	//ball.pos.x, ball.pos.y, ball.vel.x, ball.vel.y, p1.pos.x, p1.vel.x, p2.vel.x, bound_ball_intercept
	current_state[8] = (ball.pos.x - p2.pos.x) / (bounds.x);
	current_state[9] = (ball.pos.y - (bounds.y/2.0)) / (bounds.y / 2.0);
	current_state[10] = ball.vel.x / 20.0;
	current_state[11] = ball.vel.y / 20.0;
	current_state[12] = 0;//(p1.pos.x - p2.pos.x) / (bounds.x);
	current_state[13] = p1.vel.x / 30.0;
	current_state[14] = p2.vel.x / 30.0;
	if (ball.vel.y > 0) current_state[15] = (bound_ball_intercept - p2.pos.x) / bounds.x;
	else current_state[15] = 0.0;
	
	
	return &current_state[0];
}
float* World::get_rewards(){
	return &rewards[0];
}

void World::reset(){
	bounds = Vector{600.0, 600.0};
	friction = Vector{1.0, 1.0};
	ball.w = 10.0;
	ball.h = 10.0;
	p1.w = 100.0;
	p1.h = 15.0;
	p2.w = 100.0;
	p2.h = 15.0;
	
	ball.pos = bounds/Vector{2.0, 2.0};
	
	float t = 20.0;
	ball.vel.x = t*static_cast<float>(rand())/static_cast<float>(RAND_MAX) - (t/2.0);
	if (rand() > RAND_MAX/2){
		ball.vel.y = t - std::fabs(ball.vel.x);
	}else{
		ball.vel.y = -(t - std::fabs(ball.vel.x));
	}
	
	p1.pos = Vector{bounds.x/2.0f, 20};
	p1.accel = Vector{0, 0};
	p1.vel = Vector{0, 0};
	
	p2.pos.x = bounds.x/2.0;
	p2.pos.y = bounds.y - 50;
	p2.accel = Vector{0, 0};
	p2.vel = Vector{0, 0};
	
}

