/*
 * EffectVector.cpp
 *
 *  Created on: 22/10/2015
 *      Author: root
 */


#include "EffectVector.h"
#include <math.h>

EffectVector EffectVector::operator - ( const EffectVector& wzParam) const
{
	EffectVector v;
	v.x = x-wzParam.x;
	v.y = y-wzParam.y;
	v.z = z-wzParam.z;

	return v;
}

EffectVector EffectVector::operator * ( const float& fParam) const
{
	EffectVector v;
	v.x = x*fParam;
	v.y = y*fParam;
	v.z = z*fParam;

	return v;
}

EffectVector& EffectVector::operator += ( const EffectVector& wzParam)
{
	x += wzParam.x;
	y += wzParam.y;
	z += wzParam.z;
	return (*this);
}

EffectVector EffectVector::operator + ( const EffectVector& wzParam) const
{
	EffectVector v;
	v.x = x+wzParam.x;
	v.y = y+wzParam.y;
	v.z = z+wzParam.z;
	return v;
}

EffectVector EffectVector::operator - () const
{
	EffectVector v;
	v.x = -x;
	v.y = -y;
	v.z = -z;
	return v;
}

float EffectVector::getLength() const
{
	return sqrt( x * x + y * y + z * z );
}

float EffectVector::getLength2() const
{
	return x * x + y * y + z * z;
}






