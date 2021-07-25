# NeuralSnake

Not proud for the quality of code but it was fun!

# What is going on

Creating a population of 1000 networks ([check the repo](https://github.com/mrnul/GNeural-Nets/tree/master/src)), 100 out of those 1000 are the elite networks (the networks that achieved the best score).

Those elite networks get to live for one more generation and give birth to 900 new networks that replace the previous non-elite networks.

After ~700 generations the best network is able to play for a decent amount of time 

# Run

Use Visual Studio Community 2019 to compile and run preferably in release mode.

I am using windows API for some console functions and [GetAsyncKeyState](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate) for user input so this can run only on windows.

So... run the .exe let it train for as long as you like and then hit ESCAPE key to see how the best network performs.
