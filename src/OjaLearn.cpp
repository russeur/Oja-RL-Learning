#include "OjaLearn.hpp"
#include "math.h"



ABCDNet::ABCDNet(){}
ABCDNet::ABCDNet(int layer_count):net(layer_count){
	
}
ABCDNet::~ABCDNet(){
	delete [] connections;
}
void ABCDNet::reset_and_init(int layer_count){
	connections_count = 0;
	if (connections != nullptr) delete [] connections;
	
	net.reset_and_init(layer_count);
}

void ABCDNet::add_layer(ActivationFunc func, int n_neurons, bool has_bais){
	net.add_layer(func, n_neurons, has_bais);
	
	
	if (net.ready_layer_count>=net.layer_count){
		
		connections_count = 0;
		for (int l = 0; l<net.layer_count-1; l++){
			
			connections_count += (net.layers+l)->weight_count;
			
			
		}
		
		
		connections = new Connection[connections_count];
		//std::cout<<"count: "<<connections_count<<"\n";
		random_hebians();
	}
}
void ABCDNet::random_hebians(){
	
	for (int c = 0; c<connections_count; c++){
		(connections+c)->ha = (.05f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 0.025f;//set rand value between -1 and 1
		(connections+c)->hb = (.05f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 0.025f;//set rand value between -1 and 1
		(connections+c)->hc = (.05f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 0.025f;//set rand value between -1 and 1
		(connections+c)->hd = (.05f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 0.025f;//set rand value between -1 and 1
		(connections+c)->hr = (.05f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - 0.025f;//set rand value between -1 and 1
	}
}
void ABCDNet::set_ones_hebians(){
	for (int c = 0; c<connections_count; c++){
		(connections+c) ->ha = 1;
		(connections+c) ->hb = 1;
		(connections+c) ->hc = 1;
		(connections+c) ->hd = 1;
		(connections+c) ->hr = 1;
	}
}
void ABCDNet::predict_and_train(float* input){
	predict(input);
	
	train();
	
}
void ABCDNet::predict(float* input){
	net.foward(input);
}
void ABCDNet::train(){
	Connection* current_connection = connections;
	float* current_post;
	float* current_pre;
	float* current_weight;
	
	for (int l = 0; l<net.layer_count-1; l++){//only do inner layers(exclude input and output weights
		
		
		
		//update weights with ABCDR paramenters
		current_post = (net.layers + l+1)->values;
		
		current_weight = (net.layers + l)->weights;
		//loop through post neurons
		for (int i = 0; i<(net.layers+l+1)->neuron_count; i++){
			current_pre = (net.layers + l)->values;
			
			
			
			float* fake_weight = current_weight;
			float total_weight = 0.0f;
			int weight_count = 0;
			//loop through pre neurons
			for (int j = 0; j<(net.layers +l)->neuron_count+static_cast<int>((net.layers+l+1)->has_bais); j++){
				
				if (std::isnan(*current_weight)){
					std::cout<<"IS NAN WITHOUT HEBIAN UPDATE\n";
				}
				
				
				if (j < (net.layers +l)->neuron_count){
					
					
					*current_weight += 	((current_connection->ha * *current_pre * *current_post)+
										(current_connection->hb * *current_pre)+
										(current_connection->hc * *current_post)+
										(current_connection->hd)) * current_connection->hr;
										
				}else{//bais neuron
					
									
					
					*current_weight += 	((current_connection->ha * 1.0f * *current_post)+
										(current_connection->hb * 1.0f)+
										(current_connection->hc * *current_post)+
										(current_connection->hd)) * current_connection->hr;
				}
				if (std::isnan(*current_weight)){
					std::cout<<"WEIGHT NAN: hebs = ("<<current_connection->ha<<", "<<current_connection->hb<<", "<<current_connection->hc <<", "<<current_connection->hd<<", "<<current_connection->hd<<"), pre: "<<*current_pre<<", post: "<<*current_post<<"\n";
				}
				total_weight+= std::fabs(*current_weight);
				if (std::isnan(total_weight) || total_weight == 0.0f){
					std::cout<<weight_count<<" TOTAL WEIGHT = "<<total_weight<<"\n";
					std::cout<<"ADDED: "<<std::fabs(*current_weight)<<" = abs("<<*current_weight<<")\n";
				}
				if (std::isnan(*current_weight)){
					std::cout<<"IS NAN AFTER HEBIAN UPDATE\n";
				}
				current_weight++;
				weight_count++;
				current_connection++;
				current_pre++;
				
			}
			//loop through pre neurons again
			for (int j = 0; j<(net.layers +l)->neuron_count+static_cast<int>((net.layers+l+1)->has_bais); j++){
				if (std::isnan(*fake_weight)){
					std::cout<<"IS NAN WITHOUT WEIGHT NORMILIZATION\n";
				}
				
				*fake_weight /= total_weight;
				if (std::isnan(*fake_weight)){
					std::cout<<"IS NAN AFTER WEIGHT NORMILIZATION: "<<total_weight<<"\n";
					cout_all_values(&net);
					abort();
				}
				fake_weight++;
				
			}
			
			current_post++;
			
		}
		
		//normilize weights, so their absolute values add to 1.0f
		
		
		
		
	}
	
}

void ABCDNet::set_hebians(Connection* new_vals){
	for (int i = 0; i<connections_count;i++){
		*(connections+i) = *(new_vals+i);
	}
}


void cout_all_values(ABCDNet* target){
	cout_all_values(&target->net);
}
void cout_last_values(ABCDNet* target){
	cout_last_values(&target->net);
}


void cout_all_hebians(ABCDNet* target){
	std::cout<<"Hebians: ";
	for (int i = 0; i<target->connections_count;i+=50){
		
		Connection* current_connection = target->connections+i;
		std::cout<<"["<<current_connection->ha<<", "<<current_connection->hb<<", "<<current_connection->hc<<", "<< current_connection->hd<<", "<< current_connection->hr<<"], ";
	}
	std::cout<<"\n";
}
