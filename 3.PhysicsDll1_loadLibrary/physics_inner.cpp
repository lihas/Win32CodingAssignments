constexpr long double Gas_Constant_R = 0.08205; //L atm / (mol·K) - from http://www.science.uwaterloo.ca/~cchieh/cact/c120/idealgas.html

float PtoV_inner(float P, unsigned int moles, unsigned int temp_kelvin)
{
    float V = (moles * Gas_Constant_R*temp_kelvin)/P;
    return V;
}


float VtoP_inner(float V, unsigned int moles, unsigned int temp_kelvin)
{
    float P = (moles * Gas_Constant_R*temp_kelvin) / V;
    return P;
}
