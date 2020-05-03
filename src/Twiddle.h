//
// Created by Maciej Czechowski on 27/04/2020.
//

#ifndef PID_TWIDDLE_H
#define PID_TWIDDLE_H
#include <iostream>
#include <vector>
#include "PID.h"
using std::vector;

class Twiddle {
public:
    Twiddle(PID &pid) :  stage(0), coeff(0), pid(pid) {
        p = pid.GetCoeffs();
        best_p = p;
        org_p = p;
        dp = {0.1*p[0],0.1*p[1],0.1*p[2]};
     // dp = {1,1,1};
    }

    void Reset(){
        if (stage == 0) return;
        p[coeff] += org_p[coeff];
        dp[coeff] *= 0.9;

        if (++stage == 4) stage = 1;
    }

    vector<double> GetCurrentBest(){
        return best_p;
    }

    void Execute(){
        while (true) {
            if (stage == 0) {
                best = pid.TotalError();
                stage = 1;
            } else if (stage == 1) {
                org_p = p;
              //  std::cout << "ratio " << dp[0] + dp[1] + dp[2] << std::endl;

                //for...
                p[coeff] += dp[coeff];
                pid.Init(p[0], p[1], p[2]);
               // std::cout << "trying " << p[0] << " " << p[1] << " " <<  p[2] << std::endl;
                stage = 2;
                return; //run

            } else if (stage == 2) {
                auto err = pid.TotalError();

                if (err < best) {
                    best = err;
                    best_p = p;
                    dp[coeff] *= 1.1;
                    std::cout << " New best " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
                    stage = 1;
                } else {
                    stage = 3;
                    p[coeff] -= 2*dp[coeff];
                    pid.Init(p[0], p[1], p[2]);
                  //  std::cout << "trying " << p[0] << " " << p[1] << " " <<  p[2] << std::endl;
                    return;
                }
            } else if (stage == 3){
                auto err = pid.TotalError();
                if (err < best) {
                    best = err;
                    best_p = p;
                    dp[coeff] *= 1.1;
                    std::cout << " New best " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
                    //end for
                } else {
                    std::cout << " Did not improve on "<< p[0] << ", " << p[1] << ", " << p[2] << " c/b" << err << "/" << best << std::endl;
                    std::cout << " best: " << best_p[0] << ", " << best_p[1] << ", " << best_p[2] << std::endl;
                    p[coeff] += dp[coeff];
                    dp[coeff] *= 0.9;
                }
                if (++coeff == 3) coeff = 0;
                stage = 1;
            }
        }

    }

private:
 int stage;
 int coeff;
 double best;
 PID& pid;
 vector<double> p;
 vector<double> best_p;
 vector<double> dp;
 vector<double> org_p;
};


#endif //PID_TWIDDLE_H
