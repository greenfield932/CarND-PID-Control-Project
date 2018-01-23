#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID():m_cte(0.),
           m_totalCte(0.)
{
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void PID::UpdateError(double cte) {
    m_cte = cte;
    m_totalCte += cte;
}

double PID::TotalError() {
    return m_totalCte;
}

double PID::CTE(){
    return m_cte;
}
