#include <iostream>
enum ActivationFunc{
    LINEAR,
    RELU,
    SIGMIOD,
	LRELU,
	TANH
};

static const float E = 2.71828f;
float linear(float);
float relu(float);
float sigmiod(float);
float leaky_relu(float);
float my_tanh(float);



class BPNNLayer{
    public:
    int weight_count = 0;
    float* weights = nullptr;//i is each neuron in next layer, j is each neuron in this layer, if its empty then this is output layer
    float* values = nullptr;//the value of the neurons in this layer
    int neuron_count;
    bool has_bais;//doesn't effect neuron count, only effects the number of weghts going into it, you can add bais to any layer exept first
    BPNNLayer* next_layer = nullptr;
    int is_first = false;
    
    ActivationFunc function;
    
    void ready_layer(int neuron_count, ActivationFunc func, bool is_first, bool has_bais);
    void connect(BPNNLayer*);
    void compute();//computes the values of output, places those values in output, input and output can be the same
    void set_ones();//sets all weights to one, usually a init
    void set_rand();//sets all weights to random value between -1 and 1
    void set_glorot();//sets all weights using uniform xavier initialization
    
    ~BPNNLayer();

};

class BPNeuralNet{
    protected:
    
    public:
    int total_number_paramaters = 0;
    int ready_layer_count = 0;
    int input_count;
    BPNNLayer* layers = nullptr;
	
    int layer_count;//first and last layer count

    int real_output_count = 0;
    

    void foward(float* input);//sets the output of the nn to the result of foward passing the input
    void add_layer(ActivationFunc func, int neuron_count, bool has_bais);
    void set_ones();//sets all layers' weights to one, usually a init
    void set_random();//sets all wegihts to random value between -1 and 1
    void set_glorot();//sets all layers' weights using the glorot/xavier initialization
    void reset_and_init(int layer_count);//used to reset and int this object
    float* get_output();//gets the values of the last layer
    
    BPNeuralNet(int layer_count);
    BPNeuralNet();
    ~BPNeuralNet();
};

void cout_all_values(BPNeuralNet*);

void cout_last_values(BPNeuralNet*);

void cout_all_weights(BPNeuralNet*);
