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
#include "triangle.h"

Triangle::Triangle() :
    RTObject(TRIANGLE_OBJECT)
{
    cornerA = Vector(0.0, 0.0, 0.0);
    cornerB = Vector(0.0, 0.0, 0.0);
    cornerC = Vector(0.0, 0.0, 0.0);
}

Triangle::Triangle(Vector a, Vector b, Vector c, Material mat) :
    RTObject(TRIANGLE_OBJECT, mat)
{
    cornerA = a;
    cornerB = b;
    cornerC = c;
}
