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
#include "rtobject.h"

RTObject::RTObject()
{
    shape = UNKNOWN_OBJECT;
    UseDefaultMaterial();
}

RTObject::RTObject(Shape objShape)
{
    shape = objShape;
    UseDefaultMaterial();
}

RTObject::RTObject(Shape objShape, Material mat)
{
    shape = objShape;
    material = mat;
}

void RTObject::UseDefaultMaterial()
{
    material.diffColor.red = 0.0;
    material.diffColor.green = 0.0;
    material.diffColor.blue = 0.0;
    material.specColor.red = 0.0;
    material.specColor.green = 0.0;
    material.specColor.blue = 0.0;
    material.ka = 0.5;
    material.kd = 0.5;
    material.ks = 0.5;
    material.specExp = 10;
    material.kt = 0.0;
    material.insideRefraction = 1.0;
    material.outsideRefraction = 1.0;
}
