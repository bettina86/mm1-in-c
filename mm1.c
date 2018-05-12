#include <stdio.h>              // printf()
#include <stdlib.h>             // exit() e rand()
#include <math.h>               // log()


#define SIM_TEMPO  2.0e6        // tempo di simulazione
#define ARR_TEMPO  0.201 // lambda
#define SERV_TEMPO  0.100         // mu




double rand_val(int seed)
{
	const long  a = 16807;  // Moltiplicatore
	const long  m = 2147483647;  // Modulo
	const long  q = 127773;  // m div a
	const long  r = 2836;  // m mod a
	static long x;               // Random intero
	long        x_div_q;         // x diviso per q
	long        x_mod_q;         // x modulo q
	long        x_nuovo;           // Nuovo valore di x

								   // Popola
	if (seed != 0) x = seed;

	// RNG con interi aritmetici
	x_div_q = x / q;
	x_mod_q = x % q;
	x_nuovo = (a * x_mod_q) - (r * x_div_q);
	if (x_nuovo > 0)
		x = x_nuovo;
	else
		x = x_nuovo + m;

	// Valore random tra 0.0 e 1.0
	return((double)x / m);
}

//==============================================================================
//      =         Genera esponeziale con modo inverso                         =//
//==============================================================================
double esponenziale(double x)
{
	double z;                     // Numero uniforme random tra o e 1


	do
	{
		z = rand_val(0);
	} while ((z == 0) || (z == 1));

	return(-x * log(z));
}
main()
{

	double   end_tempo = SIM_TEMPO; // Tempo totale di simulazione
	double   Ta = ARR_TEMPO;       // Tempo medio tra gli arrivi
	double   Ts = SERV_TEMPO;     // Tempo medio di servizio
	double   tempo = 0.0;          // Tempo di simulazione
	double   t1 = 0.0;            // Tempo per il prossimo evento (arrivo)
	double   t2 = SIM_TEMPO;       // Tempo per il prossimo evento #2 (partenza)
	unsigned int n = 0;           // Numero di utenti nel sistema
	unsigned int c = 0;           // Numero di servizi completati
	double   b = 0.0;             // Tempo totale di occupazione
	double   s = 0.0;             // Area del numero di utenti nel sistema
	double   tn = tempo;           // Variabile per "tempo ultimo evento"
	double   tb;                  // Variabile per "inizio ultimo tempo occupato"
	double   x;                   // Throughput
	double   u;                   // Utilizzazione
	double   l;                   // Numero medio nel sistema
	double   w;                   // Tempo medio di residenza
								 



								  // Popola la variabile random
	rand_val(1);

	// Ciclo tempo di simulazione
	while (tempo < end_tempo)
	{
		if (t1 < t2)                // *** Evento #1 (arrivo) ***
		{
			tempo = t1;
			s = s + n * (tempo - tn);  // Aggiorna s area numero di utenti nel sistema
			n++;
			tn = tempo;                // tn = "tempo ultimo evento" per il prossimo evento
			t1 = tempo + esponenziale(Ta);
			if (n == 1)
			{
				tb = tempo;              // Setta "inizio ultimo tempo occupato"
				t2 = tempo + esponenziale(Ts);
			}
		}
		else                        // *** Evento #2 (partenza) ***
		{
			tempo = t2;
			s = s + n * (tempo - tn);
			n--;
			tn = tempo;
			c++;                      // Incrementa il numero di utenti serviti
			if (n > 0)
				t2 = tempo + esponenziale(Ts);
			else
			{
				t2 = SIM_TEMPO;
				b = b + tempo - tb;      // Aggiorna tempo di occupazione se vuoto
			}
		}
	}

	// Fine della simulazione e aggiorna il tempo di occupazione
	b = b + tempo - tb;

	// Calcola l'output
	x = c / tempo;   // Throughput rate, numero servizi completati nel tempo di simulazione
	u = b / tempo;   // Utilizzazione(rho), tempo totale di occupazione
	l = s / tempo;   // Numero medio nel sistema
	w = l / x;      // Tempo medio di residenza

					// Output results
	printf("=============================================================== \n");
	printf("=                        Risultati           = \n");
	printf("=============================================================== \n");
	printf("=  Tempo totale di simulazione        = %3.4f sec   \n", end_tempo);
	printf("=============================================================== \n");
	printf("=  INPUTS:                                    \n");
	printf("=    Tempo medio tra gli arrivi = %f sec      \n", Ta);
	printf("=    Tempo medio di servizio       = %f sec      \n", Ts);
	printf("=============================================================== \n");
	printf("=  OUTPUTS:                                   \n");
	printf("=    Numero utenti serviti      = %ld utenti    \n", c);
	printf("=    Throughput rate            = %f cust/sec \n", x);
	printf("=    Utilizzazione         = %f %%       \n", 100.0 * u);
	printf("=    Numero medio utenti nel sistema      = %f utenti     \n", l);
	printf("=    Tempo medio nel sistema        = %f sec      \n", w);
	printf("=============================================================== \n");
	system("pause");

}
