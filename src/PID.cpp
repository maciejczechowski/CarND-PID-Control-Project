#include "PID.h"
#include <iostream>
/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {

}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
    this->Kp = Kp_;
    this->Ki = Ki_;
    this->Kd = Kd_;
    this->p_error = 0;
    this->d_error = 0;
    this->i_error = 0;
    this->total_error = 0;
    this->iteration = 0;

}

void PID::UpdateError(double cte) {

    if (iteration == 0){
        p_error = cte;
    }
    d_error = cte - p_error;
    i_error += cte;
    p_error = cte;

    //iteration++;
    if (iteration++ > 500) {
        total_error += cte * cte;
    }


}

double PID::GetDError(){
    return d_error;
}

double PID::TotalError() {
    return total_error / iteration;  // TODO: Add your total error calc here!
}

double PID::Run() {
    return -Kp * p_error - Kd * d_error - Ki * i_error;
}

vector<double> PID::GetCoeffs() {
    return {Kp, Ki, Kd};
}
