#include"DFT.h"
#include <cmath>

constexpr long double PI = 3.141592653589793238462643383279502884197169399375105820974; //http://www.wolframalpha.com/input/?i=pi
/*
Before writing C code, I wrote matlab code for quick prototyping. Pasting it here for reference.
Links:-
http://in.mathworks.com/help/matlab/ref/fft.html
https://en.wikipedia.org/wiki/Discrete_Fourier_transform

Code -
## Copyright (C) 2018 sahils
##
## This program is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see
## <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{retval} =} fft_try2 (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: sahils <sahils@SAHILS-LT>
## Created: 2018-10-13
clc
clear

Fs = 1000; %sampling frequency
T = 1/Fs; %sampling period
L = 1000; %length of singnal -> no. of sampling periods
t = (0:L-1)*T;
num_samples = length(t);

signal = sin(2*pi*30*t);

for p=1:num_samples
  XK = 0;
  for q=1:num_samples
    w=(2*pi*p*q)/num_samples;
    XK = XK + signal(q)*(cos(w) - j*sin(w));
  end
  X(end + 1) = XK;
end

Y = abs(X);
Y2 = Y(1:L/2+1);

f=Fs*(0:L/2)/L;%http://in.mathworks.com/help/matlab/ref/fft.html
plot(f, Y2);
set(gca,'XTick',0:5:f(end));

figure(2);
stem(f,Y2);
set(gca,'XTick',0:5:f(end));

*/
using namespace std::complex_literals;

std::vector<std::complex<long double>> GetDFT(const std::vector<std::complex<long double>>& signal, const unsigned int sampling_frequency, const unsigned int signal_length)
{
    std::vector<std::complex<long double>> DFT;
    long double sampling_period = ((long double)1.0) / sampling_frequency;
    std::vector<long double> time_epochs; //discrete time intervals when signal is sampled

    for (int i = 0; i < signal_length; i++)
    {
        time_epochs.push_back(i*sampling_period);
    }

    const unsigned int num_samples = time_epochs.size();

    for (int p = 0; p < num_samples; p++)
    {
        std::complex<long double> XK(0.0,0.0);
        for (int q = 0; q < num_samples; q++)
        {
            long double w = (2.0 * PI*p*q)/ num_samples;
            std::complex<long double> cs(std::cos(w), std::sin(w));
            XK += cs*signal[q];
        }
        DFT.push_back(XK);
    }
    return DFT;
}

std::tuple<std::vector<long double>, std::vector<std::complex<long double>>> GetSineWave(unsigned int signal_frequency)
{
    constexpr unsigned int sampling_frequency = 1000;
    const unsigned int signal_length = 1000;

    long double sampling_period = ((long double)1.0) / sampling_frequency;

    std::vector<long double> time_epochs; //discrete time intervals when signal is sampled

    for (int i = 0; i < signal_length; i++)
    {
        time_epochs.push_back(i*sampling_period);
    }

    std::vector<std::complex<long double>> signal;
    for (int i = 0; i < signal_length; i++)
    {
        auto value_at_epoch_i = sin(2 * PI*signal_frequency*time_epochs[i]);
        signal.push_back(value_at_epoch_i);
    }

    return std::tuple<std::vector<long double>, std::vector<std::complex<long double>>>(time_epochs, signal);
}

std::vector<long double> TimeEpochsToFrequencySpectrum(const std::vector<long double>& time_epochs, const unsigned int sampling_frequency, const unsigned int signal_length)
{
    std::vector<long double> freqSpectrum;
    for (auto& t : time_epochs)
    {
        freqSpectrum.push_back(((long double)(t*sampling_frequency * sampling_frequency)) / signal_length);
    }

    return freqSpectrum;
}
