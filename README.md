# CarND-Controls-PID

This projects implements PID controller for steering control of a car over track in the simulator.

## Description

[image1]: ./formula.png
[image2]: ./tau_p_high.gif
[image3]: ./tau_p_low.gif
[image4]: ./tau_d_high.gif
[image5]: ./tau_i.gif

The PID controller implements the following formula:

![alt text][image1]

"P" component in a PID controller means proportional control, in our case - proportional response of the steering angle to the cross track error.
The greater the error the greater steering correction and vice versa.
The tau_p coefficient is responsible for the contribution of proportional correction in the total steering.
Visually the greater tau_p the faster response of the controller to the cross track erorr.
When tau_p multiplier is too high, steering may result to oscillations with increasing of amplitude.

![alt text][image2]

And too low values may result to late correction.

![alt text][image3]

"I" component in a PID controller is responsible for bias compensations which in reality may mean actuation delay time or errors in angles of the wheels setup.
Without proper multiplier tau_i for this component steering unable to compensate systematic errors, as result the correct position is shifted by a bias.

![alt text][image5]

"D" component in a PID controller is responsible for correction of the steering and proportional to the difference of cross track error over two sequential updates divided by the time interval between updates. It helps to compensate oscillations introduced by P component. Visually the greater tau_d the smoother and slow response to a cross track error.
Too high values of tau_d may result to overshooting because tau_p does not introduce enough correction.

![alt text][image4]

To find appropriate values of the PID controller coefficients I changed these values by multipling them x10 or x0.1 repeatedly starting from 1 until I got correct behaviour.
First I tuned multipliers for P and D terms. And next I tuned tau_i.
Throttle value is controlled by a cross track error propotionally, the higher error the lower the throttle.

Here's a [link to my video result for speed limited version](./drive_40_mph.mp4) and [speed unlimited version with proportional breaks](./drive_max.mp4)
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

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

