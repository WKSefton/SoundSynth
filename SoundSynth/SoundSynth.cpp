#include <iostream>
#include "olcNoiseMaker.h"
#include "SoundSynth.h"
using namespace std;

double Pi = 3.14159;
atomic<double> FreqHz = 0.0;
atomic<double> Amplitude = 0.5; // volume control

string KeyboardInputs = "ZSXCFVGBNJMK\xbcL\xbe"; // escape chars create asci math
int KeyDown = 0x8000;

double* Freqs = new double[16]; // holds western common notes
void CreateFreqRange()
{
	double octaveBaseFreq = 110.0; // A2 note
	double TwelthRootOfTwo = pow(2.0, 1.0 / 12.0); // twelve notes

	for (int i = 0; i < 15; i++) { // calculate all freqs
		Freqs[i] = octaveBaseFreq * pow(TwelthRootOfTwo, i);
	}
}

void LoopKeyAction()
{
	while (true) {

		for (int i = 0; i < 15; i++) { // check for key down and change freq
			while (GetAsyncKeyState(KeyboardInputs[i]) & KeyDown) {
				FreqHz = Freqs[i]; // apply freq range
			}
			FreqHz = 0.0; // stop sound
		}
	}
}

double GetAngularVelocity(double freqHz) {
	return freqHz * 2 * Pi;
}

double MakeNoiseAnalogHarmonic(double dTime) {
	//double dOutput = Amplitude * (sin(GetAngularVelocity(FreqHz) * dTime) +  sin((GetAngularVelocity(FreqHz) + 20.0) * dTime));
	double dOutput =  sin(GetAngularVelocity(FreqHz) * dTime);
	for (int i = 0; i < 2; i++)
	{
		dOutput += sin((GetAngularVelocity(FreqHz) + (i * 20.0)) * dTime);
	}
	return Amplitude * dOutput;
}

double MakeNoiseAnalog(double dTime) {
	return Amplitude * sin(GetAngularVelocity(FreqHz) * dTime);
}

double MakeNoiseDigital(double dTime) {
	double dOutput = Amplitude * sin(GetAngularVelocity(FreqHz) * dTime);
	if (dOutput > 0.0)
		return 0.2;
	else
		return -0.2;
}

int main()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	//for (auto d : devices)
	//	wcout << "Found Output Device:" << d << endl;

	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	wcout << "Found Output Device:" << devices[0] << endl;

	sound.SetUserFunction(MakeNoiseAnalogHarmonic); // delegate sound function

	CreateFreqRange();

	LoopKeyAction();

	return 0;
}





