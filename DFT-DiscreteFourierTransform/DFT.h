#pragma once
#include <vector>
#include <complex>
#include <tuple>
#include<Windows.h>

enum class PLOT_TYPE
{
    PLOT_TYPE_STEM,
    PLOT_TYPE_PLOT
};

std::vector<std::complex<long double>> GetDFT(const std::vector<std::complex<long double>>& signal, const unsigned int sampling_frequency, const unsigned int signal_length /*In terms of number of sampling periods*/);
std::tuple<std::vector<long double>, std::vector<std::complex<long double>>> GetSineWave(unsigned int signal_frequency);
HRESULT Figure(HWND hParent, int figNum);
HRESULT Stem(const std::vector<long double> X, const std::vector<long double> Y, const unsigned int tick_step_size, const PLOT_TYPE plotType = PLOT_TYPE::PLOT_TYPE_STEM);
HRESULT Plot(const std::vector<long double> X, const std::vector<long double> Y, const unsigned int tick_step_size);
std::vector<long double> TimeEpochsToFrequencySpectrum(const std::vector<long double>& time_epochs, const unsigned int sampling_frequency, const unsigned int signal_length);