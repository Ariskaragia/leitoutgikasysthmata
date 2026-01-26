#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define Ntel 4
#define Ncook 2
#define Noven 10
#define Ndeliverer 10
#define Torderlow 1 
#define Torderhigh 5 
#define Norderlow 1 
#define Norderhigh 5
#define Pm 35
#define Pp 25
#define Ps 40
#define Tpaymentlow 1
#define Tpaymenthigh 3
#define Pfail 5
#define Cm 10
#define Cp 11
#define Cs 12
#define Tprep 1
#define Tbake 10
#define Tpack 1
#define Tdellow 5
#define Tdelhigh 15

typedef struct customer {
	int ID;
	int numberOfPizzas;
	int timeFromFirstOrder;
} CUSTOMER;
