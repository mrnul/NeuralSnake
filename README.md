# NeuralSnake

Not proud for the quality of code but it was fun!

# What is going on

Creating a population of 1000 networks ([check the repo](https://github.com/mrnul/GNeural-Nets/tree/master/src)), 100 out of those 1000 are the elite networks (the networks that achieved the best score).

Those elite networks get to live for one more generation and give birth to 900 new networks with some mutations, and those new networks replace the previous non-elite networks.

After training in a 10x10 grid for ~700 generations the best network is able to play for a decent amount of time in a 40x25 grid.

# Input and output of the networks

Topology of the networks is {5, 4, 3}

Input
1. Head to Food direction (2 nodes)
2. Danger of hitting something in front, right, left of the head (3 nodes)

Output
1. Don't turn, Turn right, Turn left (3 nodes)

# Run

Use Visual Studio Community 2019 to compile and run preferably in release mode.

I am using windows API for some console functions and [GetAsyncKeyState](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate) for user input so this can run only on windows.

So... run the .exe let it train for as long as you like and then hit ESCAPE key to see how the best network performs.

