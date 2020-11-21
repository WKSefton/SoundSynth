// SoundSynth.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"
double Pi = 3.14159;
atomic<double> FreqHz = 0.0;
atomic<double> Amplitude = 1.0;

double GetAngularVelocity(double freqHz) {
	return freqHz * 2 * Pi;
}
double MakeNoise(double dTime) {
	double dOutput = Amplitude * sin(GetAngularVelocity(FreqHz) * dTime);

	if (dOutput > 0.0)
		return 0.2;
	else
		return -0.2;
}

string KeyboardInputs = "ZSXCFVGBNJMK\xbcL\xbe"; // escape chars create asci math
int KeyDown = 0x8000;

double* Freqs = new double[16];
int main()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices)
		wcout << "Found Output Device:" << d << endl;

	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	sound.SetUserFunction(MakeNoise);

	double octaveBaseFreq = 110.0; // A2 note
	double TwelthRootOfTwo = pow(2.0, 1.0 / 12.0); // twelve notes

	for (int i = 0; i < 15; i++) { // calculate all freqs
		Freqs[i] = octaveBaseFreq * pow(TwelthRootOfTwo, i);
	}

	while (true) {

		for (int i = 0; i < 15; i++) { // check for key down and change freq
			while (GetAsyncKeyState(KeyboardInputs[i]) & KeyDown) {
				FreqHz = Freqs[i];
			}
			FreqHz = 0.0;
		}
	}

	return 0;
}