#include "lpc.h"

#define COEF_SIZE 13
#define MIC_SIZE 8000

double coefs[COEF_SIZE];

void ForwardLinearPrediction(double * x )
{
    // GET SIZE FROM INPUT VECTORS
    int N = MIC_SIZE-1;
    int m = COEF_SIZE-1;
	
    // INITIALIZE R WITH AUTOCORRELATION COEFFICIENTS
    double R[m+1];
    for ( int i = 0; i <= m; i++ )
    {
        for ( int j = 0; j <= N - i; j++ )
        {
            R[ i ] += x[ j ] * x[ j + i ];
        }
    }
    // INITIALIZE Ak
    double Ak[m+1];
    Ak[ 0 ] = 1.0;

    // INITIALIZE Ek
    double Ek = R[ 0 ];
    // LEVINSON-DURBIN RECURSION
    for ( int k = 0; k < m; k++ )
    {
        // COMPUTE LAMBDA
        double lambda = 0.0;
        for (  int j = 0; j <= k; j++ )
        {
            lambda -= Ak[ j ] * R[ k + 1 - j ];

        }
        lambda /= Ek;
        // UPDATE Ak
        for ( int n = 0; n <= ( k + 1 ) / 2; n++ )
        {
            double temp = Ak[ k + 1 - n ] + lambda * Ak[ n ];
            Ak[ n ] = Ak[ n ] + lambda * Ak[ k + 1 - n ];
            Ak[ k + 1 - n ] = temp;
        }
        // UPDATE Ek
        Ek *= 1.0 - lambda * lambda;
        //printf("Ek=%f\n",Ek);
    }
		
		for(int i=0; i<=m; i++)
			coefs[i] = Ak[i];
}
