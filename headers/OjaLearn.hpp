#include "NN.hpp"

struct Connection{
	float ha, hb, hc, hd, hr, a_change, b_change, c_change, d_change, r_change;
};

class ABCDNet{
	protected:
	void train_layer();
	
	public:
	int connections_count;
	Connection* connections = nullptr;// i is layer, j is post neuron, i is pre neuron, skip connections from both last and first layers
	BPNeuralNet net;
	
	void predict_and_train(float* inputs);
	void predict(float* inputs);
	void train();//trains entire net
	void add_layer(ActivationFunc func, int n_neurons, bool has_bais);
	void random_hebians();//sets the hebian values in every connecition to a float between -1 and 1
	void set_ones_hebians();//sets all the hebians as 1
	void set_hebians(Connection* new_vals);//must be same dims as connections var
	void reset_and_init(int layer_count);
	
	ABCDNet(int layer_count);//safe, must add layers for each layer
	ABCDNet();
	~ABCDNet();//delete connections
	
};


void cout_all_values(ABCDNet*);

void cout_last_values(ABCDNet*);

void cout_all_hebians(ABCDNet*);








