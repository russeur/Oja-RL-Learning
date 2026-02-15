#include "pong.hpp"
#include "OjaLearn.hpp"
#include "drawer.hpp"
#include <fstream>
#include <string>

void load_net(const char* fileName, ABCDNet** agent){
	
	std::ifstream ifs(fileName, std::fstream::in | std::fstream::binary);
	if (ifs.is_open()){
		
		int layer_count;
		ifs.read(reinterpret_cast<char*>(&layer_count), sizeof(layer_count));
		*agent = new ABCDNet(layer_count);
		for (int l = 0; l<layer_count; l++){
			bool has_bais;
			ActivationFunc activation_func;
			int neuron_count;
			ifs.read(reinterpret_cast<char*>(&has_bais), sizeof(has_bais));
			ifs.read(reinterpret_cast<char*>(&activation_func), sizeof(activation_func));
			ifs.read(reinterpret_cast<char*>(&neuron_count), sizeof(neuron_count));
			(*agent)->add_layer(activation_func, neuron_count, has_bais);
			std::cout<<"l: "<<l<<", b: "<<has_bais<<", a: "<<activation_func<<", n: "<<neuron_count<<"\n";
		}
		ifs.read(reinterpret_cast<char*>((*agent)->connections), (*agent)->connections_count * sizeof(Connection));
		std::cout<<"loaded model\n";
		
		
	}else{
		std::cout<<"unable to open file name: "<<fileName<<"\n";
	}
	ifs.close();
}



bool auto_play = false;

void make_action(World* env, char* arg, SDL_Keycode key, bool is_p1, ABCDNet* agent, float* action){
	
	if (auto_play){
		if (key == SDLK_DELETE) auto_play = false;
		
		if (strcmp(arg, "self") ==0){
			*action = 0;
		}else if (strcmp(arg, "simple")==0){
			env->simple_ai_move(action, is_p1);
		}else{
			if (is_p1) agent->predict_and_train(env->state()+0);//p1
			else agent->predict_and_train(env->state()+8);//p2
			*action = *(agent->net.get_output()) * 5;//amplify action
			cout_all_values(&agent->net);
		}
		
		
	}
	
	switch (key){
		case SDLK_UP:
			if (strcmp(arg, "self") ==0){
				*action = 0;
			}else if (strcmp(arg, "simple")==0){
				env->simple_ai_move(action, is_p1);
			}else{
				if (is_p1) agent->predict_and_train(env->state()+0);//p1
				else agent->predict_and_train(env->state()+8);//p2
				*action = *(agent->net.get_output()) * 5;//amplify action
				cout_all_values(&agent->net);
			}
			
			
			
			break;
		case SDLK_DOWN:
			if (strcmp(arg, "self") ==0){
				*action = 0;
			}else if (strcmp(arg, "simple")==0){
				env->simple_ai_move(action, is_p1);
			}else{
				if (is_p1) agent->predict_and_train(env->state()+0);//p1
				else agent->predict_and_train(env->state()+8);//p2
				*action = *(agent->net.get_output()) * 5;//amplify action
				cout_all_values(&agent->net);
			}
			
			break;
		case SDLK_LEFT:
			if (strcmp(arg, "self") == 0){
				*action = -1;
			}else if (strcmp(arg, "simple")==0){
				env->simple_ai_move(action, is_p1);
			}else{
				if (is_p1) agent->predict_and_train(env->state()+0);//p1
				else agent->predict_and_train(env->state()+8);//p2
				*action = *(agent->net.get_output()) * 5;//amplify action
				cout_all_values(&agent->net);
			}
			
			break;
		case SDLK_RIGHT:
			if (strcmp(arg, "self") == 0){
				*action = 1;
			}else if (strcmp(arg, "simple")==0){
				env->simple_ai_move(action, is_p1);
			}else{
				if (is_p1) agent->predict_and_train(env->state()+0);//p1
				else agent->predict_and_train(env->state()+8);//p2
				*action = *(agent->net.get_output()) * 5;//amplify action
				cout_all_values(&agent->net);
			}
			
			break;
		case SDLK_SPACE:
		
			auto_play = true;
			break;
		
	}
}

int main(int argc, char*argv[]){
	srand(time(0));
	if (argc != 3){
		std::cout<<"Unable to proccess "<<argc<<" arguments\n";
		return 0;
	}
	TTF_Init();
	SDL_drawer drawer("PathFinder", 600, 600, SDL_Color{0, 0, 0, 255});
	World env;
	env.bounds = Vector{600, 600};
	env.friction = Vector{1, 1};
	env.ball.w = 10.0;
	env.ball.h = 10.0;
	env.p1.w = 100.0;
	env.p1.h = 15.0;
	env.p2.w = 100.0;
	env.p2.h = 15.0;
	env.reset();
	
	float actions[2];
	float cumm_rewards[2] = {0.0f, 0.0f};
	
	
	ABCDNet* agent_p1;
	ABCDNet* agent_p2;
	SDL_Event e;
	
	
	std::cout<<argv[1]<<"\n";
	
	std::string agent_1_file_location = "../saved_models/"+std::string(argv[1]); 
	std::string agent_2_file_location = "../saved_models/"+std::string(argv[2]); 
	
	if ((strcmp(argv[1], "self") != 0) && (strcmp(argv[1], "simple") != 0)){
		load_net(agent_1_file_location.c_str(), &agent_p1);
		agent_p1->net.set_random();
	}
	if ((strcmp(argv[2], "self") != 0) && (strcmp(argv[2], "simple") != 0)){
		load_net(agent_2_file_location.c_str(), &agent_p2);
		agent_p2->net.set_random();
	}
	
	bool is_quit = false;
	int frame_count = 0;
	while (!is_quit){
		
		drawer.clear_render();
		drawer.draw_rect((int)(env.ball.pos.x), (int)(env.ball.pos.y), (int)(env.ball.w), (int)(env.ball.h), SDL_Color{255, 255, 255, 255});
		drawer.draw_rect((int)(env.p1.pos.x), (int)(env.p1.pos.y), (int)(env.p1.w), (int)(env.p1.h), SDL_Color{255, 255, 255, 255});
		drawer.draw_rect((int)(env.p2.pos.x), (int)(env.p2.pos.y), (int)(env.p2.w), (int)(env.p2.h), SDL_Color{255, 255, 255, 255});
		drawer.draw_rect((int)(env.bound_ball_intercept), (int)(env.p2.pos.y), (int)(5), (int)(5), SDL_Color{0, 255, 255, 255});
		
		drawer.show_render();
		
		if (auto_play){
			make_action(&env, argv[1], e.key.keysym.sym, true, agent_p1, &actions[0]);//p1 action
			make_action(&env, argv[2], e.key.keysym.sym, false, agent_p2, &actions[1]);//p2 action
			
			env.step(&actions[0]);
			cumm_rewards[0] += *(env.get_rewards()+0);
			cumm_rewards[1] += *(env.get_rewards()+1);
			std::cout<<"Reward: "<<*(env.get_rewards()+0)<<", "<<*(env.get_rewards()+1)<<"\naction: "<<*(actions+0)<<", "<<*(actions+1)<<"\n";
			frame_count++;
			std::cout<<env.p1_win<<" : "<<env.p2_win<<"\n";
			std::cout<<cumm_rewards[0]<<" : "<<cumm_rewards[1]<<"\n";
			
			
			
		}
		
		
		
		while (SDL_drawer::poll_events(&e)){
			switch (e.type){
				case SDL_QUIT:
					is_quit = true;
					break;
				case SDL_KEYDOWN:
					make_action(&env, argv[1], e.key.keysym.sym, true, agent_p1, &actions[0]);//p1 action
					make_action(&env, argv[2], e.key.keysym.sym, false, agent_p2, &actions[1]);//p2 action
					
					env.step(&actions[0]);
					cumm_rewards[0] += *(env.get_rewards()+0);
					cumm_rewards[1] += *(env.get_rewards()+1);
					std::cout<<"Reward: "<<*(env.get_rewards()+0)<<", "<<*(env.get_rewards()+1)<<"\naction: "<<*(actions+0)<<", "<<*(actions+1)<<"\n";
					frame_count++;
					std::cout<<env.p1_win<<" : "<<env.p2_win<<"\n";
					std::cout<<cumm_rewards[0]<<" : "<<cumm_rewards[1]<<"\n";
					break;
			}
		}
	}
	std::cout<<"frame count: "<<frame_count<<"\n";
	delete agent_p1;
	delete agent_p2;
	return EXIT_SUCCESS;
}
