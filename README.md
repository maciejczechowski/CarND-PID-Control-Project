# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program - PID Controller

##Introduction
This is a project implementing a PID controller for Term2 simulator of Udacity Self-Driving Car Engineer Program.
It also contains auto-tune algorithm using Twiddle. 

## The controller
The controller logic is contained in `pid.h/pid.cpp` - it provides a direct implementation of PID algorithm.
Additionally the `Twiddle` class in `twiddle.h` contains an algorithm for auto-tuning the P, I, D parameters.
It was used to fine-tune the parameters which were first selected manually to allow car get through the track. 
Using only twiddle was problematic, as the car quickly felt off the track.

The initial parameter selection and meaning each of each parameter is discussed below:

### P(roportional)   
The P parameter has the most direct impact on the controller. It steers contrary to cte (left when car is going right and otherwise) therefore creating oscillating movements.
Too high P parameter lead to high oscillations and car quickly falling of the track. Making P too low caused the car not to follow the path as it could not react quickly enough to the change.
When setting parameter manually, it was set to the way that the car was driving with stable oscillations (I and D were set to 0). Input for fine tune was set to 0.12

### D(ifferential) 
The D parameter damps the oscillations when error is low, meaning that our car drives close to the path without weaving left and right. Setting D to high meant that the car could not react to changes quickly enough.
Manual selection meant finding the lowest value that caused oscillations to disappear (with P value set as above and I set to 0). This was set to 2.757.

### I(ntegral)
Integral parameter works on accumulated error, therefore removing the bias that goes out from P-D controller. Example reason of bias could be a steering drift.
I parameter was a most difficult to work with. Slight increase of I caused the car to go like crazy. In the end, this parameter was set to 0.00001, allowing Twiddle to find a better value using very small steps.

## Twiddle
The manual parameters of (0.12, 0, 2.757) were then passed to Twiddle algorithm. It was run over 2000, then 6000 frames. PID was not including first 500 measurements when calculating Total Error for Twiddle - this was introduced to allow controller to settle down.
Twiddle was tweaked to use slow initial learning rate, with 0.1 of initial value of each parameter.

After a dozens of drives, the final parameters were calculated as (0.132104, 9.68619e-06, 3.77153).
The effect of those can be seen on [this video](PID_control.m4v), which also uses pretty aggressive throttle.

  

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

