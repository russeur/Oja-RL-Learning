#  Oja RL Learning

A PANN using Oja Learning, more specifically ABCD plasticity, to solve RL problems.

The PANN and ABCD plasticity is based of this paper: https://arxiv.org/pdf/2111.04113

Currently, the PANN model is being used to solve the Pong Game.

## Build

You must have SDL2 installed

Use Cmake in a build directory then make

use 'main save_model_name' in order to train a newly generated model and save it to the save name.
use 'main save_model_name load_model_name' in order to train a loaded model from save, then save it to the save name

use 'vis agent1_name agent2_name' in order to see two different agents play against eachother.
Agent_name can either be a saved model name, 'self' (user plays), or 'simple' (very simple AI).


