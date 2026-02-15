#include "hebianSelection.hpp"
#include <fstream>
#include <string>

void load_nature(const char* fileName, NaturalSelector** nature, int agent_count){
	std::ifstream ifs(fileName, std::fstream::in | std::fstream::binary);
	if (ifs.is_open()){
		int layer_count;
		ifs.read(reinterpret_cast<char*>(&layer_count), sizeof(layer_count));
		*nature = new NaturalSelector(agent_count, layer_count);
		for (int l = 0; l<layer_count; l++){
			bool has_bais;
			ActivationFunc activation_func;
			int neuron_count;
			ifs.read(reinterpret_cast<char*>(&has_bais), sizeof(has_bais));
			ifs.read(reinterpret_cast<char*>(&activation_func), sizeof(activation_func));
			ifs.read(reinterpret_cast<char*>(&neuron_count), sizeof(neuron_count));
			(*nature)->add_layer(activation_func, neuron_count, has_bais);
			std::cout<<"l: "<<l<<", b: "<<has_bais<<", a: "<<activation_func<<", n: "<<neuron_count<<"\n";
		}
		ifs.read(reinterpret_cast<char*>((*nature)->get_best_hebians()), (*nature)->get_connection_count() * sizeof(Connection));
		std::cout<<"loaded model\n";
		
		
	}else{
		std::cout<<"unable to open file name: "<<fileName<<"\n";
	}
	ifs.close();
}
void save_net(const char* fileName, NaturalSelector* nature){
	std::ofstream ofs(fileName, std::fstream::out | std::fstream::binary);
	if (ofs.is_open()){
		int layer_count = nature->get_layer_count();
		ofs.write(reinterpret_cast<const char*>(&layer_count), sizeof(layer_count));
		for (int l = 0; l<nature->get_layer_count(); l++){
			bool has_bais = nature->get_bais(l);
			ActivationFunc activation_func = nature->get_activation(l);
			int neuron_count = nature->get_neuron_count(l);
			ofs.write(reinterpret_cast<const char*>(&has_bais), sizeof(has_bais));
			ofs.write(reinterpret_cast<const char*>(&activation_func), sizeof(activation_func));
			ofs.write(reinterpret_cast<const char*>(&neuron_count), sizeof(neuron_count));
			std::cout<<"l: "<<l<<", b: "<<has_bais<<", a: "<<activation_func<<", n: "<<neuron_count<<"\n";
		}
		ofs.write(reinterpret_cast<const char*>(nature->get_best_hebians()), nature->get_connection_count() * sizeof(Connection));
		std::cout<<"saved\n";
	}
	else{
		std::cout<<"failed to open file name: "<<fileName<<"\n";
	}
	ofs.close();
}




int main(int argc, char*argv[]){
	if ((argc > 3)||(argc <2)) {
		std::cout<<"incorrect number of arguments of "<< argc<<"\n";
		return 1;
	}
	
	srand(time(0));
	for (int i = 0; i<100; i++){
		rand();
	}
	
	
	
	NaturalSelector* nature;//population of 3, one survivor, 6 layers to each neural net
	int agent_count = 200;
	
	
	//add layers to the nets
	
	
	
	if (argc <3){
		nature = new NaturalSelector(agent_count, 4);
		nature->add_layer(ActivationFunc::RELU, 8, true);//input
	
		nature->add_layer(ActivationFunc::RELU, 8, true);
		
		nature->add_layer(ActivationFunc::LINEAR, 4, true);
		
		
	
		nature->add_layer(ActivationFunc::LINEAR, 1, true);//output
		//init weights and hebian constants
		nature->set_random_weights();
		
		
	}else{// set the initial hebians, layers, neurons, baises, and activation funcs
		std::string load_file_location = "../saved_models/"+std::string(argv[2]);
		load_nature(load_file_location.c_str(), &nature, agent_count);
	}
	
	nature->max_steps = 500;//max number of steps in a generation
	
	
	const int num_steps = 2500;
	float averages[num_steps];
	
	//run one generation
	for (int i = 0; i<num_steps; i++){
		
		nature->populate();
		nature->run_generation();
		averages[i] = nature->get_average_cummulative_reward() / 200.0;
		
		std::cout<<"average reward: "<<nature->get_average_cummulative_reward()<<"\n";
		
		//cout_last_values(&nature);
		
		nature->update_best_policy_sort();
		
	}
	std::cout<<"Averages: ";
	for (int i = 0; i<num_steps; i++){
		std::cout<<averages[i]<<", ";
	}
	std::cout<<"\n";
	
	
	std::cout<<"\n";
	std::cout<<"lr: "<<nature->learning_rate<<"\n";
	std::cout<<"sigma: "<<nature->sigma<<"\n";
	
	std::string save_file_location = "../saved_models/"+std::string(argv[1]);
	
	save_net(save_file_location.c_str(), nature);
	
	delete nature;
	return 0;
	
}


