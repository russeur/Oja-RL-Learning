#include "NN.hpp"
#include "math.h"


float linear(float val){
    return val;
}
float relu(float val){
    return (val<=0)? 0:val;
}
float sigmiod(float val){
    return 1.0f/(1.0f +std::pow(E, -val));
}
float leaky_relu(float val){
	return (val<0)? .01*val : val;
}
float my_tanh(float val){
	return (std::pow(E, val)-std::pow(E, -val))/(std::pow(E, val)+std::pow(E, -val));
}









void BPNNLayer::compute(){
    
    float* current_weight = weights;
    switch (function)
    {
    case ActivationFunc::LINEAR:
	//std::cout<<"LIlayer: ";
        for (int i = 0; i<next_layer->neuron_count; i++){
            //std::cout<<"\nneuron: "<<i<<": ";
            *(next_layer->values + i) = 0;

            for (int j = 0; j<neuron_count+static_cast<int>(next_layer->has_bais); j++){
                if (j == neuron_count){//if is currently targeting the bais neuron, only happens if next layer has bais
                   *(next_layer->values+i) +=  *current_weight;
                }else{
                    *(next_layer->values+i) += *(values+j) * *current_weight;
					//std::cout<<*(values+j)<<", ";
                }
                current_weight++;
                
            }
            //std::cout<<"    : " <<*(next_layer->values+i)<<" : "<<linear(*(next_layer->values+i))<<"\n";
            *(next_layer->values+i) = linear(*(next_layer->values+i));
            
        }
		//std::cout<<"\n";
        break;

    case ActivationFunc::RELU:
		//std::cout<<"Rlayer: ";
        for (int i = 0; i<next_layer->neuron_count; i++){
            //std::cout<<"\nneuron: "<<i<<": ";
            *(next_layer->values + i) = 0;

            for (int j = 0; j<neuron_count+static_cast<int>(next_layer->has_bais); j++){
                if (j == neuron_count){//if is currently targeting the bais neuron, only happens if next layer has bais
                   *(next_layer->values+i) +=  *current_weight;
                }else{
                    *(next_layer->values+i) += *(values+j) * *current_weight;
					//std::cout<<*(values+j)<<", ";
                }
                current_weight++;
                
            }
            //std::cout<<"    : " <<*(next_layer->values+i)<<" : "<<relu(*(next_layer->values+i))<<"\n";
            *(next_layer->values+i) = relu(*(next_layer->values+i));
            
        }
		//std::cout<<"\n";
        break;
    case ActivationFunc::SIGMIOD:
        for (int i = 0; i<next_layer->neuron_count; i++){
            
            *(next_layer->values + i) = 0;

            for (int j = 0; j<neuron_count+static_cast<int>(next_layer->has_bais); j++){
                if (j == neuron_count){//if is currently targeting the bais neuron, only happens if next layer has bais
                   *(next_layer->values+i) +=  *current_weight;
                }else{
                    *(next_layer->values+i) += *(values+j) * *current_weight;
                }
                current_weight++;
                
            }
            
            *(next_layer->values+i) = sigmiod(*(next_layer->values+i));
            
        }
        break;
	case ActivationFunc::LRELU:
		//std::cout<<"Rlayer: ";
        for (int i = 0; i<next_layer->neuron_count; i++){
            //std::cout<<"\nneuron: "<<i<<": ";
            *(next_layer->values + i) = 0;

            for (int j = 0; j<neuron_count+static_cast<int>(next_layer->has_bais); j++){
                if (j == neuron_count){//if is currently targeting the bais neuron, only happens if next layer has bais
                   *(next_layer->values+i) +=  *current_weight;
                }else{
                    *(next_layer->values+i) += *(values+j) * *current_weight;
					//std::cout<<*(values+j)<<", ";
                }
                current_weight++;
                
            }
            //std::cout<<"    : " <<*(next_layer->values+i)<<" : "<<relu(*(next_layer->values+i))<<"\n";
            *(next_layer->values+i) = leaky_relu(*(next_layer->values+i));
            
        }
		//std::cout<<"\n";
        break;
	case ActivationFunc::TANH:
		//std::cout<<"Rlayer: ";
        for (int i = 0; i<next_layer->neuron_count; i++){
            //std::cout<<"\nneuron: "<<i<<": ";
            *(next_layer->values + i) = 0;

            for (int j = 0; j<neuron_count+static_cast<int>(next_layer->has_bais); j++){
                if (j == neuron_count){//if is currently targeting the bais neuron, only happens if next layer has bais
                   *(next_layer->values+i) +=  *current_weight;
                }else{
                    *(next_layer->values+i) += *(values+j) * *current_weight;
					//std::cout<<*(values+j)<<", ";
                }
                current_weight++;
                
            }
            //std::cout<<"    : " <<*(next_layer->values+i)<<" : "<<relu(*(next_layer->values+i))<<"\n";
            *(next_layer->values+i) = my_tanh(*(next_layer->values+i));
            
        }
		//std::cout<<"\n";
        break;
    }
    
}
void BPNNLayer::ready_layer(int neuron_count, ActivationFunc func, bool is_first, bool has_bais){
    if (values != nullptr){
        delete [] values;
    }
    this->neuron_count = neuron_count;
    this->function = func;
    this->is_first = is_first;
    this->has_bais = has_bais;
    this->values = new float[neuron_count];
    
}
void BPNNLayer::connect(BPNNLayer* next_layer){
    if (weights != nullptr){
        delete [] weights;
    }
    this->next_layer = next_layer;
    this->weight_count = (neuron_count + static_cast<int>(next_layer->has_bais)) * next_layer->neuron_count; //(neuron_count+bais)*next_neuron_count
    weights = new float[weight_count];//set fully connected
}
void BPNNLayer::set_ones(){
    for (int i = 0; i<weight_count; i++){
        *(weights+i) = .05f;
    }
}
void BPNNLayer::set_rand(){
    for (int i = 0; i<weight_count; i++){
		
        *(weights+i) = (.2f*static_cast<float>(rand())/static_cast<float>(RAND_MAX)) - .1f;
    }
}

void BPNNLayer::set_glorot(){
    float sub_by = std::sqrt(6.0/(neuron_count + next_layer->neuron_count));
    
    float mod_by = sub_by*2;
    
    for (int w = 0; w<weight_count; w++){
        *(weights+w) = (std::fmod(rand(), mod_by))-sub_by;
        
    }
}

BPNNLayer::~BPNNLayer(){
    if (weights != nullptr){//for output layer
        delete [] weights;
    }
    if (!is_first){//for input layer
        delete [] values;
    }
    
}


void BPNeuralNet::foward(float* const input){
    
    for (int i = 0; i<layer_count-1; i++){//exclude last layer as a compute layer
        
        if (i == 0){//if its the first layer, then compute with the inputs instead
            for (int v = 0; v<(layers+0)->neuron_count; v++){
				*((layers+0)->values+v) = *(input+v);
				//std::cout<<v<<": "<<*(input+v)<<"\n";
			}
            
            (layers+0)->compute();
        } else {
            (layers+i)->compute();//compute with the output of the last layer
        }
    }

}

void BPNeuralNet::add_layer(ActivationFunc func, int neuron_count, bool has_bais){
    //make sure all layers arn't allready full
    if (ready_layer_count>=layer_count) return;
    
    

    //ready current layer
    (layers+ready_layer_count)->ready_layer(neuron_count, func, (ready_layer_count==0), has_bais);
    //connect last layer to this current layer
    if (ready_layer_count != 0) (layers+ready_layer_count-1)->connect((layers+ready_layer_count));
    total_number_paramaters += (layers+ready_layer_count-1)->weight_count;

    ready_layer_count++;

    if (ready_layer_count==layer_count){
        real_output_count = neuron_count;
    }

}
void BPNeuralNet::set_ones(){
    for (int i = 0; i<layer_count-1; i++){// -1 because last layer doesn't have weights, the one before it connects to it
        (layers + i)->set_ones();
    }
}
void BPNeuralNet::set_random(){
	for (int i = 0; i<layer_count-1; i++){// -1 because last layer doesn't have weights, the one before it connects to it
		(layers + i)->set_rand();
	}
	/*
    for (int i = 0; i<layer_count-1; i++){// -1 because last layer doesn't have weights, the one before it connects to it
		if ((i ==0)||(i==layer_count-1)){
			(layers + i)->set_ones();
		}
		else{
			
			(layers + i)->set_rand();
		}
		
		
    }
	*/
}
void BPNeuralNet::set_glorot(){
    for (int l = 0; l<layer_count-1; l++){// -1 because last layer doesn't have weights, the one before it connects to it
        (layers+l)->set_glorot();
    }
}
float* BPNeuralNet::get_output(){
    return (layers+layer_count-1)->values;
}
void BPNeuralNet::reset_and_init(int layer_count){
	
    if (layers != nullptr){
        
        delete [] layers;
    }
	
    total_number_paramaters = 0;
    input_count = 0;
    ready_layer_count = 0;
    real_output_count = 0;
    this->layer_count = layer_count;
    layers = new BPNNLayer[layer_count]{};
    ready_layer_count = 0;
	
}
BPNeuralNet::BPNeuralNet(int layer_count){
    this->layer_count = layer_count;
    layers = new BPNNLayer[layer_count]{};
    ready_layer_count = 0;
}

BPNeuralNet::BPNeuralNet(){}
BPNeuralNet::~BPNeuralNet(){
    delete [] layers;
}



void cout_all_values(BPNeuralNet* net){
	std::cout<<"Vals: "<<net->layer_count;
    for (int l = 0; l<net->layer_count; l++){
        std::cout<<"layer "<<l<<" values: ";

        for (int n = 0; n< (net->layers+l)->neuron_count; n++){
			
            std::cout<<*((net->layers+l)->values+n)<<", ";
        }
        std::cout<<"\n";
    }
    
}

void cout_last_values(BPNeuralNet* net){
    std::cout<<"Outputs: ";
    for (int n = 0; n< (net->layers + net->layer_count - 1)->neuron_count; n++){
        std::cout<<*((net->layers + net->layer_count - 1)->values+n)<<", ";
    }
    std::cout<<"\n";
}

void cout_all_weights(BPNeuralNet* net){
	for (int l = 0; l<net->layer_count-1; l++){
        std::cout<<"layer "<<l<<" weights: ";

        for (int n = 0; n< (net->layers+l)->weight_count; n+=100){
            std::cout<<*((net->layers+l)->weights+n)<<", ";
        }
	}
}