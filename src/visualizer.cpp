#include "drawer.hpp"
#include <SDL2\SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "game.hpp"
#include "OjaLearn.hpp" 
#include <fstream>

void load_net(char* fileName, ABCDNet** agent){
	
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


int main(int argc, char*argv[]){
	srand(time(0));
	if (argc != 2){
		std::cout<<"Unable to proccess "<<argc<<" arguments\n";
		return 0;
	}
	TTF_Init();
	SDL_drawer drawer("PathFinder", 600, 600, SDL_Color{0, 0, 0, 255});
	World env;
	env.reset();
	SDL_Event e;
	bool is_quit = false;
	std::cout<<argv[1]<<"\n";
	if (strcmp(argv[1], "self") != 0){
		ABCDNet* agent;
		load_net(argv[1], &agent);
		
		
		while (!is_quit){
			drawer.clear_render();
			drawer.draw_rect((env.px+100)*3-1, (env.py+100)*3-1, 3, 3, SDL_Color{255, 255, 255, 255});
			drawer.draw_rect((env.tx+100)*3-1, (env.ty+100)*3-1, 3, 3, SDL_Color{0, 255, 0, 255});
			drawer.show_render();
			float reward;
			while (drawer.poll_events(&e)){
				switch (e.type){
					case SDL_QUIT:
						is_quit = true;
						break;
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym){
							case SDLK_UP:
								agent->predict_and_train(env.state());
								reward = env.step(agent->net.get_output());
								std::cout<<"Reward: "<<reward<<"\n";
								break;
							case SDLK_RIGHT:
								env.mx = 1;
								env.my = 1;
								std::cout<<"1 , 1\n";
								break;
							case SDLK_LEFT:
								env.mx = -1;
								env.my = 1;
								std::cout<<"-1 , 1\n";
								break;
							case SDLK_DOWN:
								env.mx = 1;
								env.my = -1;
								std::cout<<"1 , -1\n";
								break;
						}
				}
				
			}
			
		}
		delete agent;
	}else {
		while (!is_quit){
			float UP[] = {0, -.01};
			float DOWN[] = {0, .01};
			float LEFT[] = {-.01, 0};
			float RIGHT[] = {.01, 0};
			drawer.clear_render();
			drawer.draw_rect((env.px+100)*3-1, (env.py+100)*3-1, 3, 3, SDL_Color{255, 255, 255, 255});
			drawer.draw_rect((env.tx+100)*3-1, (env.ty+100)*3-1, 3, 3, SDL_Color{0, 255, 0, 255});
			drawer.show_render();
			float reward;
			while (drawer.poll_events(&e)){
				switch (e.type){
					case SDL_QUIT:
						is_quit = true;
						break;
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym){
							case SDLK_UP:
								reward = env.step(&UP[0]);
								std::cout<<"Reward: "<<reward<<"\n";
								break;
							case SDLK_DOWN:
								reward = env.step(&DOWN[0]);
								std::cout<<"Reward: "<<reward<<"\n";
								break;
							case SDLK_LEFT:
								reward = env.step(&LEFT[0]);
								std::cout<<"Reward: "<<reward<<"\n";
								break;
							case SDLK_RIGHT:
								reward = env.step(&RIGHT[0]);
								std::cout<<"Reward: "<<reward<<"\n";
								break;
						}
						break;
				}
				
			}
			
		}
	}


	return EXIT_SUCCESS;
}


