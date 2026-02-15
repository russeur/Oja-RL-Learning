#pragma once
#include <cmath>
struct Vector{
    float x, y;
    Vector operator +(Vector& rpos){
        return Vector{x+rpos.x, y+rpos.y};
    }
    Vector operator -(Vector& rpos){
        return Vector{x-rpos.x, y-rpos.y};
    }
    Vector operator *(Vector& rpos){
        return Vector{x*rpos.x, y*rpos.y};
    }
    Vector operator /(Vector& rpos){
        return Vector{x/rpos.x, y/rpos.y};
    }


    Vector operator +(Vector&& rpos){
        return Vector{x+rpos.x, y+rpos.y};
    }
    Vector operator -(Vector&& rpos){
        return Vector{x-rpos.x, y-rpos.y};
    }
    Vector operator *(Vector&& rpos){
        return Vector{x*rpos.x, y*rpos.y};
    }
    Vector operator /(Vector&& rpos){
        return Vector{x/rpos.x, y/rpos.y};
    }


    void operator +=(Vector& rpos){
        x += rpos.x;
        y += rpos.y;
    }
    void operator -=(Vector& rpos){
        x -= rpos.x;
        y -= rpos.y;
    }
    void operator *=(Vector& rpos){
        x *= rpos.x;
        y *= rpos.y;
    }
    void operator /=(Vector& rpos){
        x /= rpos.x;
        y /= rpos.y;
    }
    
    void operator +=(Vector&& rpos){
        x += rpos.x;
        y += rpos.y;
    }
    void operator -=(Vector&& rpos){
        x -= rpos.x;
        y -= rpos.y;
    }
    void operator *=(Vector&& rpos){
        x *= rpos.x;
        y *= rpos.y;
    }
    void operator /=(Vector&& rpos){
        x /= rpos.x;
        y /= rpos.y;
    }


    bool operator ==(const Vector& other){
        return ((x==other.x)&&(y==other.y));
    }


    bool operator ==(const Vector&& other){
        return ((x==other.x)&&(y==other.y));
    }
};


class Paddle{
	public: 
	float w, h;
	Vector pos, vel, accel;//top right pos
	
	
	void move(Vector bound, Vector friction);
};
class Ball{
	
	
	
	
	
	public:
	
	float w, h;
	Vector pos, vel;//top right pos
	bool is_colliding(Paddle* p);//used by bounce_on_paddle
	void move(Vector bound, Vector friction);
	void bounce_on_paddle(Paddle* p);
	
};

class World{
	protected:
	bool paddle_just_bounced_ball = false;
	bool paddle_already_bounced_ball = false;
	float current_state[16];//0-7 = player one state; 8-15 = player two state;
	float rewards[2];//0= player one reward; 1 = player two reward;
	void ball_paddles_bounce();
	public:
	int p1_win = 0;
	int p2_win = 0;
	float bound_ball_intercept;
	Paddle p1;
	Paddle p2;
	Ball ball;
	Vector friction;
	Vector bounds;
	
	
	void step(float* actions);//first action float = player one; 2nd action float = player two;
	void simple_ai_move(float* action, bool is_p1);//makes action = a simple ai's move
	float* state();//see current_state for output
	float* get_rewards();//see rewards for output
	void reset();
	
	
};
