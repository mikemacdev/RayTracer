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
#include "light.h"
#include "linkedlist.h"
#include "ray.h"
#include "rtobject.h"
#include "sphere.h"
#include "triangle.h"
#include "vector.h"
#include <gl/glut.h>
#include <gl/gl.h>

static const double PI = 3.141592654;

static const double EPSILON = 0.000000000001;

static const int MAX_DEPTH = 10;

// scene properties
Vector eyepos;
Vector viewdir;
Vector updir;

double viewdist;
double fovh;
double fovv;

int pixwidth;

Material currentMat;

LinkedList<Light> lightList;
LinkedList<Sphere> sphereList;
LinkedList<Triangle> triList;

// scene rendering properties
GLubyte *image;

Color blackColor;

double width;
double height;

int pixheight;

Vector uVect, vVect;
Vector ulVect, urVect, llVect, lrVect;
Vector deltaH, deltaV;

// functions
void InitializeVariables();
void InitializeScene();
void InitializeWindow();

void DisplayEvent();
void ReshapeEvent(int width, int height);

int ReadSceneFile(char *filename);

RTObject * TraceRay(const Ray & ray, Vector & point);

Color ShadeRay(const Ray & incidentRay, const RTObject * object,
               const bool inside, const int depth);

void Clamp(double & value, double low, double high);
void Clamp(int & value, int low, int high);

Vector CrossProd(Vector v1, Vector v2);
double DotProd(Vector v1, Vector v2);
