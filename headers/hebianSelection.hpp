#include "OjaLearn.hpp"
#include "pong.hpp"

#include <algorithm>
#include <random>




struct Playground{
	World  world;
	ABCDNet agent;
	float cummulative_reward = 0;
	
	
	Playground(int net_layer_count);//sets up net
	Playground();
};


class NaturalSelector{
	protected:
	std::random_device rd;
	std::mt19937* gen;
	std::uniform_real_distribution<>* dis;
	float sampleDist();
	float get_rand_hebian_change();
	
	
	int generation_count = 0;//current generation
	
	void hebian_train();//train all agents' wegihts with their hebian connections
	
	int connections_count;
	
	
	Connection* best_hebians;
	Connection* best_hebians_delta;
	void sort_reward_indexes();
	int* index_reward_sort;
	
	
	int population_count;//number of nets in population, DO NOT CHANGE AFTER INIT
	
	
	public:
	
	void step_and_train_all();
	
	float learning_rate = .2f;
	float learning_decay = .998f;
	float sigma = .1f; 
	Playground* population;// all games and nets
	
	
	int max_steps;//maximum number of steps inside one episode
	
	
	
	
	
	
	void update_best_policy();//updates best_hebians based on population
	void update_best_policy_sort();//updates best_hebians based on population
	void populate(); //makes the population, from the best_hebians
	
	void ready_new_gen();//finds survivors, then populates next generation with the survivors of current population
	void run_generation();//runs for a full generation
	
	
	int get_population_count();
	
	void add_layer(ActivationFunc func, int neuron_count, bool has_bais);//adds layer for all networks
	
	void randomize_best_hebians();//sets the hebian constants randomly
	void set_random_weights();//sets weights randomly
	void set_glorot_weights();//sets weights using glorot
	void set_ones_weights();//set weights to one
	Connection* get_best_hebians(); 
	int get_connection_count();
	int get_layer_count();
	bool get_bais(int target_layer);
	ActivationFunc get_activation(int target_layer);
	int get_neuron_count(int target_layer);
	
	
	//for testing
	float get_average_cummulative_reward();
	float get_average_hits();
	
	NaturalSelector(int population_count, int layer_count);
	~NaturalSelector();
	
	
};


void cout_all_reward(NaturalSelector*);//couts all the cummulative rewards of each agent in this nature

void cout_last_values(NaturalSelector*);//couts all the outputs of the each agent in this nature



