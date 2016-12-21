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
#pragma once

#include "color.h"

enum Shape { UNKNOWN_OBJECT, SPHERE_OBJECT, TRIANGLE_OBJECT };

// material properties
struct Material
{
    Color diffColor;
    Color specColor;
    double ka, kd, ks;
    int specExp;
    double kt;
    double insideRefraction;
    double outsideRefraction;
};

// base object type
class RTObject
{
public:
    Shape shape;
    Material material;

    RTObject();
    RTObject(Shape objShape);
    RTObject(Shape objShape, Material mat);

protected:
    void UseDefaultMaterial();
};
