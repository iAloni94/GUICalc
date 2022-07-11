#pragma once
#include "framework.h"
#include "Resource.h"
#include <windows.h>
#include <tchar.h>
#include <winuser.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct flags {
	char mat_a;
	char mat_b;
	char matC;
	char detErr;
} FLAGS;

typedef struct mat {
	char n;
	char m;
} MAT;


typedef struct data {
	int type;
	int sub_type;
}DATA;

enum func {false, true, MAT_INPUT, MAT_IN_A, MAT_A_INPUT , MAT_IN_B, A_ROW_SIZE, A_COL_SIZE, B_ROW_SIZE, B_COL_SIZE, MAT_A_SIZE, MAT_B_SIZE, COL_SIZE, ENTER_VAL_A, ENTER_VAL_B, MAT_MULT, DET_A, DET_B, COMM_MAT};



