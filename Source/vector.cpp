/* Copyright 2016 Michael MacFarlane
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <math.h>

#include "vector.h"


Vector::Vector()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vector::Vector(double xval, double yval, double zval)
{
    x = xval;
    y = yval;
    z = zval;
}

void Vector::Normalize()
{
    if (x == 0.0 && y == 0.0 && z == 0.0)
        x = 1.0;

    double length = sqrt(x*x + y*y + z*z);

    x = x / length;
    y = y / length;
    z = z / length;
}

const bool Vector::Equals(const Vector & other)
{
    double xdif = x - other.x;
    double ydif = y - other.y;
    double zdif = z - other.z;
    if (xdif > -0.000001 && xdif < 0.000001 &&
        ydif > -0.000001 && ydif < 0.000001 &&
        zdif > -0.000001 && zdif < 0.000001)
        return true;
    else
        return false;
}

const double Vector::Length()
{
    return sqrt(x*x + y*y + z*z);
}

Vector & Vector::operator=(const Vector & rhs)
{
    if (this != &rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
        this->z = rhs.z;
    }

    return *this;
}

Vector & Vector::operator-=(const Vector & rhs)
{
    if (this == &rhs)
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    else
    {
        this->x = this->x - rhs.x;
        this->y = this->y - rhs.y;
        this->z = this->z - rhs.z;
    }

    return *this;
}

const Vector Vector::operator-(const Vector & rhs) const
{
    Vector result = *this;
    result -= rhs;
    return result;
}
