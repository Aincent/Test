/*
 * EffectVector.h
 *
 *  Created on: 22/10/2015
 *      Author: root
 */

#ifndef EFFECTMATHCOMMON_H_
#define EFFECTMATHCOMMON_H_

class EffectVector
{
public:
	EffectVector() : x(0),y(0),z(0){}

	EffectVector(float x,float y,float z)
	{
		 this->x = x;
		 this->y = y;
		 this->z = z;
	}

	EffectVector operator - ( const EffectVector& wzParam) const;

	EffectVector operator * ( const float& fParam) const;

	EffectVector& operator += ( const EffectVector&);

	EffectVector operator + ( const EffectVector& wzParam) const;

	EffectVector operator - () const;

	float getLength() const;

	float getLength2() const;

	float x,y,z;
};

#endif /* EFFECTMATHCOMMON_H_ */
