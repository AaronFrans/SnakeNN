# SnakeNN

## Demo
My snake training for +- an hour:

![Snake-NN-Timelapse-croppedgif (1)](https://user-images.githubusercontent.com/61042747/212101458-128a8fe1-7519-4fda-bbf6-589c762b4da5.gif)


## Implementation  

### How does it work?

With the help of a neural network and reinforcement training I train the snake to go into the right direction.

### What is a neural network?  
![image](https://user-images.githubusercontent.com/61042747/212089038-2191f0c7-07c4-4985-b106-b37ce94919a1.png)

A neural network is a collection of layers that each have a collection of neurons. These neurons each have a connection to every neuron from the next layer. The connections are weighted and the neuron will transform the input using a mathematical function (I use the tanh function) to pass on the data to the next neurons. The last layers are the output neurons, and these will be used to further train the network.


### What is Reinforcement Learning?  

Reinforcement learning is a way to train an ai by giving it an input, an expected result and then comparing the calculated result with the expected result. It will then redo the calculations with new inputs and compare those. This way the ai will eventually learn to do a given task.

### How does my neural network work?
My neural network takes the current frames gamestate and transforms it in following inputs:
 - The distance from the left wall
 - The distance from the top wall
 - The distance from the bottom wall
 - The distance from the right wall
 - The distance from the food column
 - The distance from the food row
 - Whether the cell above the head is a body part
 - Whether the cell bellow the head is a body part
 - Whether the cell to left of the head is a body part
 - Whether the cell to right of the head is a body part

 these will then be transformed into 4 outputs:
 - Go up
 - Go down
 - Go left
 - Go right

 The ai will then change direction into the output with the highest percentage. 

 ## Is a neural network a good approach?
 Personally I think that a neural network is not the best way to solve snake. A better way to do this is to save all cells into an array and then make a graph from that grid. On this grid you can then apply the A* algorithm to find the best pa


## Resources Used
Original Snake Code from: https://codereview.stackexchange.com/questions/263298/win32-snake-game  
How to make a neural network: https://www.youtube.com/watch?v=sK9AbJ4P8ao  
What is a neural network: https://www.youtube.com/watch?v=aircAruvnKk  
Making an neural network with snake: https://towardsdatascience.com/today-im-going-to-talk-about-a-small-practical-example-of-using-neural-networks-training-one-to-6b2cbd6efdb3

