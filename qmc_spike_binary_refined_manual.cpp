#include <iostream>
#include <math.h>
#include <string.h>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <new>
#include <fstream>

using namespace std;

int 
n = 100,
L = 1000, iMinusOne, iPlusOne, numSweep, numSweepHigh, 
numSweepLow, weightChange, averageNumber, c;

double
beta = 32,
modifier = 0.7,
Gamma, k, weightTotal, costChange, J, checkingExp, 
r, alpha = 1, error, aggregateError, aggregateNumSweep;

float targetError = 0.1;

bool bit, good;

bool ** panel;
int * panelWeight;

int main()
{
    srand(time(NULL));
    
    cout << "L=" << L << " alpha=" << alpha << 
    " target_error="	<< targetError << " n=" << n << endl;
    
    k = beta / double(L);
    panel = new bool *[L];
    for (int i = 0; i < L; i++) panel[i] = new bool[n];
    panelWeight = new int[L];
    
    aggregateNumSweep = 0, aggregateError = 0;
    
    for (c = 0; c < 5; c++) //To find out one instance for the numSweep that 
                            //makes error targetError
    {  
        numSweep = rand() % 50;
        while (numSweep < 4) numSweep = rand() % 50;
        cout << "Initial numSweep=" << numSweep << endl;
        numSweepHigh = numSweep;
        numSweepLow = 0;
        good = false;
         
        while (numSweepHigh - numSweepLow > 3)
        {
            for (int i = 0; i < L; i++)
            { //initialize the panel
                panelWeight[i] = 0;
                for (int j = 0; j < n; j++)
                {
                    panel[i][j] = rand() % 2;
                    panelWeight[i] += panel[i][j];
                }
            }
            Gamma = 1;
            
            while (Gamma > pow(10, -12))
            {
                J = 0.5 * log(1 / tanh(k * Gamma));
                for (int a = 0; a < numSweep; a++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        for (int i = 0; i < L; i++)
                        {
                            iMinusOne = (L + i - 1) % L, 
                            iPlusOne = (L + i + 1) % L;
                            costChange = 0;
                            
                            bit = !panel[i][j];
                            weightChange = 2 * bit - 1;
                            costChange += (panelWeight[i] + weightChange == n/4?
                            double(pow(n, alpha) - panelWeight[i]) :
                            double(weightChange)) * k;
                            
                            costChange += J * (bit == panel[iMinusOne][j] ? 
                            -2 : 2);
                            costChange += J * (bit == panel[iPlusOne][j] ? 
                            -2 : 2);
                            
                            checkingExp = exp(-costChange);
                            r = double(rand() + 1)/double(INT_MAX);
                            
                            if (checkingExp > r)
                            {
                                panel[i][j] = bit;
                                panelWeight[i] += weightChange;
                            }
                        }
                    }
                }
                Gamma *= modifier;
            }
            
            error = 0;
            for (int i = 0; i < L; i++) 
            {
				error += panelWeight[i];
            }
            error /= n * L;
            
            if (!good && error > targetError)
            {
                numSweepLow = numSweep;
                numSweep *= 2;
                numSweepHigh = numSweep;
                cout << 1 << endl;
            }
            else if (!good && error <= targetError)
            {
                numSweep = (numSweepLow + numSweepHigh) / 2;
                good = true;
                cout << 2 << endl;
            }
            else if (good && error > targetError)
            {
                numSweepLow = numSweep;
                numSweep = (numSweepLow + numSweepHigh) / 2;
                cout << 3 << endl;
            }
            else if (good && error <= targetError)
            {
                numSweepHigh = numSweep;
                numSweep = (numSweepLow + numSweepHigh) / 2;
                cout << 4 << endl;
            }
        }
        aggregateNumSweep += numSweep;
        aggregateError += error;
    }
    cout << "To Obtain Target Error " << targetError << " for n=" << n << 
    " L=" << L << " alpha=" << alpha << " The Mean Error is " << 
    aggregateError / c << " The Mean Number of Sweeps is " << 
    aggregateNumSweep / c << endl;
    
    for (int i = 0; i < L; i++) delete panel[i];
    delete panel;
    delete panelWeight;
}
