#include "hebianSelection.hpp"
#include <stdlib.h>
#include <ctime>
#include <cstdlib>





Playground::Playground(int net_layer_count):agent(net_layer_count){
	
}
Playground::Playground(){}

NaturalSelector::NaturalSelector(int population_count, int layer_count){
	srand(time(0));
	this->population_count = population_count;
	for (int i = 0; i<100; i++){
		rand();
	}
	gen = new std::mt19937(rd());
	dis = new std::uniform_real_distribution<>(0.001, 1.0);
	//allocate space
	this->population = new Playground[population_count]{};
	this->index_reward_sort = new int[population_count];
	
	
	
	
	
	
	
	
	
	
	
	//init population
	for (int p = 0; p<population_count; p++){
		
		*(index_reward_sort+p) = p;
		(population+p)->agent.reset_and_init(layer_count);
	}
	
	
	
	
}
NaturalSelector::~NaturalSelector(){
	delete gen;
	delete dis;
	delete [] population;
	delete [] best_hebians_delta;
	delete [] best_hebians;
	delete [] index_reward_sort;
	
	
}

void NaturalSelector::add_layer(ActivationFunc func, int neuron_count, bool has_bais){
	for (int i = 0; i<population_count; i++){
		(population+i)->agent.add_layer(func, neuron_count, has_bais);
	}
	if (population->agent.connections_count != 0){
		connections_count = population->agent.connections_count;
		this->best_hebians = new Connection[connections_count];
		this->best_hebians_delta = new Connection[connections_count];
		randomize_best_hebians();
		
		
	}
	
}

int NaturalSelector::get_population_count(){
	return population_count;
}


void NaturalSelector::step_and_train_all(){
	Playground* current_playground = population;
	float actions[2];
	
	for (int p = 0; p<population_count; p++){
		
		//take a step in the world based on how the agent wants to go in the current state of the world
		current_playground->agent.predict_and_train(current_playground->world.state() + 8);//decide actions that the agent predicts based on world state, and train weights
		current_playground->world.simple_ai_move(&actions[0], true);
		actions[1] = *current_playground->agent.net.get_output() * 5;
		current_playground->world.step(&actions[0]);//takes the action in world
		current_playground->cummulative_reward+= *(current_playground->world.get_rewards()+1); //, and adds to the cummulative reward for this agent
		if (std::isnan(current_playground->cummulative_reward)){
			cout_all_values(&(population+p)->agent.net);
			abort();
		}
		current_playground++;
		if (p ==0){
			//cout_all_values(&current_playground->agent);
		}
	}
	
	
}

void NaturalSelector::randomize_best_hebians(){
	for (int c = 0; c<connections_count; c++){
		(best_hebians+c)->ha = (2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f;//set rand value between -1 and 1
		(best_hebians+c)->hb = (2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f;//set rand value between -1 and 1
		(best_hebians+c)->hc = (2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f;//set rand value between -1 and 1
		(best_hebians+c)->hd = (2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f;//set rand value between -1 and 1
		(best_hebians+c)->hr = (2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f;//set rand value between -1 and 1
	}
}

void NaturalSelector::set_ones_weights(){
	for (int p = 0; p<population_count; p++){
		(population+p)->agent.net.set_ones();
	}
}
void NaturalSelector::set_glorot_weights(){
	for (int p = 0; p<population_count; p++){
		(population+p)->agent.net.set_glorot();
	}
}
void NaturalSelector::set_random_weights(){
	for (int p = 0; p<population_count; p++){
		(population+p)->agent.net.set_random();
	}
}

void NaturalSelector::hebian_train(){
	for (int p = 0; p<population_count; p++){
		(population+p)->agent.train();
	}
}

void NaturalSelector::run_generation(){
	for (int step = 0; step<max_steps; step++){
		//std::cout<<"new_step: \n";
		
		step_and_train_all();
		
		//cout_all_values(&population->agent);
		
		//cout_all_reward(this);
		/*
		std::cout<<"\n";
		cout_last_values(this);
		std::cout<<"\n";
		*/
	}

	if (generation_count%10  == 0){
		cout_all_hebians(&population->agent);
		
	}
	generation_count++;
}

void NaturalSelector::update_best_policy_sort(){
	for (int c = 0; c<connections_count; c++){//reset diferances
		(best_hebians_delta+c)->ha = 0;
		(best_hebians_delta+c)->hb = 0;
		(best_hebians_delta+c)->hc = 0;
		(best_hebians_delta+c)->hd = 0;
		(best_hebians_delta+c)->hr = 0;
	}
	
	float min = 1000.0;
	float max = -1000.0;
	float biggest = -1000.0;
	for (int p = 0; p<population_count; p++){// find min and max cummulative rewards
		if ((population+p)->cummulative_reward > max) max = (population+p)->cummulative_reward;
		if ((population+p)->cummulative_reward < min) min = (population+p)->cummulative_reward;
	}
	
	if (std::fabs(min) > std::fabs(max)){
		biggest = std::fabs(min);
	}else{
		biggest = std::fabs(max);
	}
	std::cout<<"max: " <<max<<"\n";
	std::cout<<"biggest: " <<biggest <<"\n";
	std::cout<<"smallest: "<<min<<"\n";
	float total_reward = 0;
	float average_reward = 0;
	
	
	average_reward = total_reward/population_count;
	ABCDNet* agent;
	sort_reward_indexes();
	
	
	float fitness_mult = -.5;
	float mult_adder = 1.0/(float)(population_count);
	
	//std::cout<<"min: "<< min<<", max: "<<max<<", biggest: "<<biggest<<", total_reward: "<<total_reward<<", average_reward: "<<average_reward<<"\n";
	for (int p = 0; p<population_count; p++){
		agent = &(population+*(index_reward_sort+p))->agent;
		
		
		
		//std::cout<<"reward: "<<(population+p)->cummulative_reward<<"\n";
		//std::cout<<"fitnes: "<<fitness_mult<<"\n";
		
		
		
		//mutate 
		for (int c = 0; c<connections_count; c++){
			
			(best_hebians_delta+c)->ha += fitness_mult * (agent->connections+c)->a_change ;//delta += fitness * change
			(best_hebians_delta+c)->hb += fitness_mult * (agent->connections+c)->b_change ; 
			(best_hebians_delta+c)->hc += fitness_mult * (agent->connections+c)->c_change ;
			(best_hebians_delta+c)->hd += fitness_mult * (agent->connections+c)->d_change ;
			(best_hebians_delta+c)->hr += fitness_mult * (agent->connections+c)->r_change ;
			
			
		}
		fitness_mult += mult_adder;
	}
	
	for (int c = 0; c<connections_count; c++){
		(best_hebians+c)->ha += (learning_rate / (sigma *population_count)) * (best_hebians_delta+c)->ha;//best hebian += lr/(sigma*pop_count) * delta
		(best_hebians+c)->hb += (learning_rate / (sigma *population_count)) * (best_hebians_delta+c)->hb;
		(best_hebians+c)->hc += (learning_rate / (sigma *population_count)) * (best_hebians_delta+c)->hc;
		(best_hebians+c)->hd += (learning_rate / (sigma *population_count)) * (best_hebians_delta+c)->hd;
		(best_hebians+c)->hr += (learning_rate / (sigma *population_count)) * (best_hebians_delta+c)->hr;
	}
	if (learning_rate > .001){
		learning_rate *= learning_decay;
	}
	if (sigma > .01){
		sigma*=.999f;
	}
	
	
	
	
}



void NaturalSelector::update_best_policy(){
	for (int c = 0; c<connections_count; c++){//reset diferances
		(best_hebians_delta+c)->ha = 0;
		(best_hebians_delta+c)->hb = 0;
		(best_hebians_delta+c)->hc = 0;
		(best_hebians_delta+c)->hd = 0;
		(best_hebians_delta+c)->hr = 0;
	}
	
	float min = 1000.0;
	float max = -1000.0;
	float biggest = -1000.0;
	for (int p = 0; p<population_count; p++){// find min and max cummulative rewards
		if ((population+p)->cummulative_reward > max) max = (population+p)->cummulative_reward;
		if ((population+p)->cummulative_reward < min) min = (population+p)->cummulative_reward;
	}
	
	if (std::fabs(min) > std::fabs(max)){
		biggest = std::fabs(min);
	}else{
		biggest = std::fabs(max);
	}
	std::cout<<"max: " <<max<<"\n";
	std::cout<<"biggest: " <<biggest <<"\n";
	std::cout<<"smallest: "<<min<<"\n";
	float total_reward = 0;
	float average_reward = 0;
	
	for (int p = 0; p<population_count; p++){//make all rewards positive and find total 
		(population+p)->cummulative_reward -= min;
		total_reward+= (population+p)->cummulative_reward;
	}
	average_reward = total_reward/population_count;
	ABCDNet* agent;
	//std::cout<<"min: "<< min<<", max: "<<max<<", biggest: "<<biggest<<", total_reward: "<<total_reward<<", average_reward: "<<average_reward<<"\n";
	for (int p = 0; p<population_count; p++){
		agent = &(population+p)->agent;
		float fitness_mult = ((((population+p)->cummulative_reward) / (max-min)) - .5f);
		if (std::isnan(fitness_mult)){
			std::cout<<"FITNESSMULT IS NAN\n";
		}
		//std::cout<<"reward: "<<(population+p)->cummulative_reward<<"\n";
		//std::cout<<"fitnes: "<<fitness_mult<<"\n";
		
		
		
		//mutate 
		for (int c = 0; c<connections_count; c++){
			
			(best_hebians_delta+c)->ha += fitness_mult * (agent->connections+c)->a_change ;//delta += fitness * change
			(best_hebians_delta+c)->hb += fitness_mult * (agent->connections+c)->b_change ;
			(best_hebians_delta+c)->hc += fitness_mult * (agent->connections+c)->c_change ;
			(best_hebians_delta+c)->hd += fitness_mult * (agent->connections+c)->d_change ;
			(best_hebians_delta+c)->hr += fitness_mult * (agent->connections+c)->r_change ;
			
			
		}
		
	}
	
	for (int c = 0; c<connections_count; c++){
		(best_hebians+c)->ha += (learning_rate / (sigma * population_count)) * (best_hebians_delta+c)->ha;//best hebian += lr/(sigma*pop_count) * delta
		(best_hebians+c)->hb += (learning_rate / (sigma * population_count)) * (best_hebians_delta+c)->hb;
		(best_hebians+c)->hc += (learning_rate / (sigma * population_count)) * (best_hebians_delta+c)->hc;
		(best_hebians+c)->hd += (learning_rate / (sigma * population_count)) * (best_hebians_delta+c)->hd;
		(best_hebians+c)->hr += (learning_rate / (sigma * population_count)) * (best_hebians_delta+c)->hr;
	}
	if (learning_rate > .001){
		learning_rate *= learning_decay;
	}
	if (sigma > .01){
		sigma*=.9995f;
	}
	
	
	
	
}
	

void NaturalSelector::populate(){
	
	
	set_random_weights();
	
	ABCDNet* agent;
	for (int p = 0; p<population_count; p++){
		agent = &(population+p)->agent;
		(population+p)->world.reset();
		//(population+p)->world.hit_count = 0; for PATH
		(population+p)->cummulative_reward = 0;
		
		
		
		//mutate 
		for (int c = 0; c<connections_count; c++){
			(agent->connections+c)->a_change = get_rand_hebian_change();//change is equal to a sample from NormalDist
			(agent->connections+c)->b_change = get_rand_hebian_change();
			(agent->connections+c)->c_change = get_rand_hebian_change();
			(agent->connections+c)->d_change = get_rand_hebian_change();
			(agent->connections+c)->r_change = get_rand_hebian_change();
			
			(agent->connections+c)->ha = (best_hebians+c)->ha + sigma* (agent->connections+c)->a_change;//new hebian is equal to best hebian+ (learningRate*change)
			(agent->connections+c)->hb = (best_hebians+c)->hb + sigma* (agent->connections+c)->b_change;
			(agent->connections+c)->hc = (best_hebians+c)->hc + sigma* (agent->connections+c)->c_change;
			(agent->connections+c)->hd = (best_hebians+c)->hd + sigma* (agent->connections+c)->d_change;
			(agent->connections+c)->hr = (best_hebians+c)->hr + sigma* (agent->connections+c)->r_change;
			
		}
	}
	
	
	
	
	
}

void NaturalSelector::ready_new_gen(){
	
}

float NaturalSelector::get_average_cummulative_reward(){
	float total = 0.0;
	for (int p = 0; p<population_count; p++){
		
		total += (population+p)->cummulative_reward;
	}
	//std::cout<<"Average "<<total/static_cast<float>(population_count)<<"\n";
	return total/static_cast<float>(population_count);
}

float NaturalSelector::get_average_hits(){
	float total = 0.0;
	for (int p = 0; p<population_count; p++){
		
		//total += (population+p)->world.hit_count;//FOR PATH
	}
	//std::cout<<"Average "<<total/static_cast<float>(population_count)<<"\n";
	return total/static_cast<float>(population_count);
}

float NaturalSelector::sampleDist(){
	float u1 = (*dis)(*gen);
	float u2 = (*dis)(*gen);
	if ((u1 == 0) || (u2 == 0)){
		std::cout<<u1<<", "<<u2<<"\n";
		std::cout<<"ZEERROOO"<<"\n";
		
		
		abort();
	}
	float s = std::log(u1/u2);
	
	
	if (std::fabs(s)>2.0){
		
		s = sampleDist();
	}
	
	
	return s;
	//std::random_device rd{};
	//std::mt19937 gen{rd()};
	//std::normal_distribution d{0.0, 1.0};
	//return d(gen);
}

float NaturalSelector::get_rand_hebian_change(){
	//return ((2.0f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 1.0f)*mutation_multiplier;
	return sampleDist();
}
Connection* NaturalSelector::get_best_hebians(){
	return best_hebians;
}
int NaturalSelector::get_connection_count(){
	return connections_count;
}
int NaturalSelector::get_layer_count(){
	return population->agent.net.layer_count;
}
bool NaturalSelector::get_bais(int target_layer){
	return (population->agent.net.layers + target_layer)->has_bais;
}
ActivationFunc NaturalSelector::get_activation(int target_layer){
	return (population->agent.net.layers + target_layer)->function;
}
int NaturalSelector::get_neuron_count(int target_layer){
	return (population->agent.net.layers + target_layer)->neuron_count;
}
void NaturalSelector::sort_reward_indexes(){
	auto comp = [this](int i, int j){
		return (population+i)->cummulative_reward < (population+j)->cummulative_reward;
	};
	std::sort(index_reward_sort, index_reward_sort+population_count, comp);
}
	


void cout_all_reward(NaturalSelector* target){
	
	for (int i =0; i<target->get_population_count(); i++){
		std::cout<<(target->population+i)->cummulative_reward<<", ";
	}
	std::cout<<"\n";
}
void cout_last_values(NaturalSelector* target){
	
	for (int i =0; i<target->get_population_count(); i++){
		cout_last_values(&(target->population+i)->agent.net);
		
	}
}


