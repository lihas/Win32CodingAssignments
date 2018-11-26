#pragma once

//convert pressure to volume
float PtoV(float P, unsigned int moles, unsigned int temp_kelvin);

//convert volume to pressure
float VtoP(float V, unsigned int moles, unsigned int temp_kelvin);
