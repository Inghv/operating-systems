#include "Polynom.h"

polynom::polynom(int deg_, double* coefficients_, double x_)
	:deg{ deg_ }, coefficients{ coefficients_ }, x{ x_ }
{}

polynom::polynom(polynom* t)
	: deg{ t->deg }, coefficients{ t->coefficients }, x{ t->x }
{}