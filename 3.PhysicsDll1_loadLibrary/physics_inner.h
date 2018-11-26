#pragma once
//convert pressure to volume
float PtoV_inner(float P, unsigned int moles, unsigned int temp_kelvin);

//convert volume to pressure
float VtoP_inner(float V, unsigned int moles, unsigned int temp_kelvin);
