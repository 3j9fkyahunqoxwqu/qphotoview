/*
 * QPhotoView core classes
 *
 * License: GPL V2. See file COPYING for details.
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 */

#include "Fraction.h"


bool Fraction::isInt() const
{
    return ( _denominator == 0 || _numerator % _denominator == 0 );
}


QString Fraction::toString() const
{
    if ( _denominator == 0 )
	return "0";

    QString result;

    if ( isInt() )
    {
	result.setNum( _numerator / _denominator );
    }
    else
    {
	result = QString( "%1/%2" ).arg( _numerator ).arg( _denominator );
    }

    return result;
}


double Fraction::toDouble() const
{
    if ( _denominator == 0 )
	return 0.0;

    else
	return _numerator / (double) _denominator;
}



int Fraction::greatestCommonDivisor( int a, int b )
{
    // Euklid's algorithm, stolen from Wikipedia
    // http://en.wikipedia.org/wiki/Euclidean_algorithm

    if ( a == 0 )
	return b;

    while ( b != 0 )
    {
	if ( a > b )
	    a -= b;
	else
	    b -= a;
    }

    return a;
}


void Fraction::simplify()
{
    int gcd = greatestCommonDivisor( _numerator, _denominator );

    if ( gcd == 0 )
	return;

    _numerator	 /= gcd;
    _denominator /= gcd;
}


Fraction Fraction::simplified() const
{
    int gcd = greatestCommonDivisor( _numerator, _denominator );

    if ( gcd == 0 )
	return Fraction();

    return Fraction( _numerator	  / gcd,
		     _denominator / gcd );
}


bool Fraction::operator>( double num )
{
    if ( _denominator == 0 )
	return false;

    return (_numerator / (double) _denominator) > num;
}


bool Fraction::operator<( double num )
{
    if ( _denominator == 0 )
	return false;

    return (_numerator / (double) _denominator) < num;
}
