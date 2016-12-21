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

#include <fstream>
#include <iostream>
#include <string>

#include "raytracer.h"

using namespace std;

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);

    if (argc != 2)
    {
        cerr << "Error: must pass 1 scene file as argument" << endl;
        return 1;
    }

    InitializeVariables();

    int ret = ReadSceneFile(argv[1]);
    if (ret > 0) return 1;

    InitializeScene();

    InitializeWindow();

    // specify callback functions
    glutDisplayFunc(DisplayEvent);
    glutReshapeFunc(ReshapeEvent);

    // run program
    glutMainLoop();

    return 0;
}

void InitializeVariables()
{
    eyepos = Vector(0, 0, 0);
    viewdir = Vector(1, 0, 0);
    updir = Vector(0, 1, 0);
    viewdist = 1;
    fovh = 90;
    fovv = 90;
    pixwidth = 500;
    currentMat.diffColor.red = 0.0;
    currentMat.diffColor.green = 0.0;
    currentMat.diffColor.blue = 0.0;
    currentMat.specColor.red = 0.0;
    currentMat.specColor.green = 0.0;
    currentMat.specColor.blue = 0.0;
    currentMat.ka = 0.5;
    currentMat.kd = 0.5;
    currentMat.ks = 0.5;
    currentMat.specExp = 10;
    currentMat.kt = 0.0;
    currentMat.insideRefraction = 1.0;
    currentMat.outsideRefraction = 1.0;

    image = nullptr;
    width = 100;
    height = 100;
    blackColor = Color(0, 0, 0);
}

void InitializeScene()
{
    double fovhrad = fovh * PI / 180.0;
    double fovvrad = fovv * PI / 180.0;

    width = 2 * viewdist * tan(fovhrad / 2.0);
    height = 2 * viewdist * tan(fovvrad / 2.0);

    pixheight = (height / width) * pixwidth;

    int imageSize = pixwidth*pixheight * 3;
    image = new GLubyte[imageSize];
    for (int i = 0; i < imageSize; i++)
        image[i] = (GLubyte)0;

    uVect = CrossProd(viewdir, updir);
    uVect.Normalize();

    vVect = CrossProd(uVect, viewdir);

    ulVect.x = eyepos.x + viewdist*viewdir.x + height*0.5*vVect.x - width*0.5*uVect.x;
    ulVect.y = eyepos.y + viewdist*viewdir.y + height*0.5*vVect.y - width*0.5*uVect.y;
    ulVect.z = eyepos.z + viewdist*viewdir.z + height*0.5*vVect.z - width*0.5*uVect.z;

    urVect.x = eyepos.x + viewdist*viewdir.x + height*0.5*vVect.x + width*0.5*uVect.x;
    urVect.y = eyepos.y + viewdist*viewdir.y + height*0.5*vVect.y + width*0.5*uVect.y;
    urVect.z = eyepos.z + viewdist*viewdir.z + height*0.5*vVect.z + width*0.5*uVect.z;

    llVect.x = eyepos.x + viewdist*viewdir.x - height*0.5*vVect.x - width*0.5*uVect.x;
    llVect.y = eyepos.y + viewdist*viewdir.y - height*0.5*vVect.y - width*0.5*uVect.y;
    llVect.z = eyepos.z + viewdist*viewdir.z - height*0.5*vVect.z - width*0.5*uVect.z;

    lrVect.x = eyepos.x + viewdist*viewdir.x - height*0.5*vVect.x + width*0.5*uVect.x;
    lrVect.y = eyepos.y + viewdist*viewdir.y - height*0.5*vVect.y + width*0.5*uVect.y;
    lrVect.z = eyepos.z + viewdist*viewdir.z - height*0.5*vVect.z + width*0.5*uVect.z;

    deltaH.x = (lrVect.x - llVect.x) / (pixwidth - 1);
    deltaH.y = (lrVect.y - llVect.y) / (pixwidth - 1);
    deltaH.z = (lrVect.z - llVect.z) / (pixwidth - 1);

    deltaV.x = (ulVect.x - llVect.x) / (pixheight - 1);
    deltaV.y = (ulVect.y - llVect.y) / (pixheight - 1);
    deltaV.z = (ulVect.z - llVect.z) / (pixheight - 1);

    for (int i = pixheight - 1; i >= 0; i--)
    {
        for (int j = 0; j < pixwidth; j++)
        {
            Color tempColor = blackColor;

            Vector pixpoint;
            pixpoint.x = llVect.x + deltaH.x*j + deltaV.x*i;
            pixpoint.y = llVect.y + deltaH.y*j + deltaV.y*i;
            pixpoint.z = llVect.z + deltaH.z*j + deltaV.z*i;

            Ray rayToPixel;
            rayToPixel.pos = eyepos;
            rayToPixel.dir.x = pixpoint.x - rayToPixel.pos.x;
            rayToPixel.dir.y = pixpoint.y - rayToPixel.pos.y;
            rayToPixel.dir.z = pixpoint.z - rayToPixel.pos.z;
            rayToPixel.dir.Normalize();

            RTObject * intersectObj = nullptr;
            Vector intersectPoint;
            intersectObj = TraceRay(rayToPixel, intersectPoint);
            if (intersectObj != nullptr)
            { // intersection present
                Ray rayToEye;
                rayToEye.pos = intersectPoint;
                rayToEye.dir = eyepos - intersectPoint;
                rayToEye.dir.Normalize();
                tempColor = ShadeRay(rayToEye, intersectObj, false, 0);
            }

            image[i*pixwidth * 3 + j * 3 + 0] = (GLubyte)(tempColor.red * 255.0);
            image[i*pixwidth * 3 + j * 3 + 1] = (GLubyte)(tempColor.green * 255.0);
            image[i*pixwidth * 3 + j * 3 + 2] = (GLubyte)(tempColor.blue * 255.0);
        }
    }
}

void InitializeWindow()
{
    glutInitWindowSize(pixwidth, pixheight);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Raytracer");
}

void DisplayEvent()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)pixwidth, 0.0, (double)pixheight);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glRasterPos2i(0, 0);
    glDrawPixels(pixwidth, pixheight, GL_RGB, GL_UNSIGNED_BYTE, image);

    glFlush();
}

void ReshapeEvent(int width, int height)
{
    glutReshapeWindow(pixwidth, pixheight);
}

int ReadSceneFile(char * filename)
{
    // check filename
    if (filename == nullptr)
    {
        cerr << "Error: nullptr filename" << endl;
        return 1;
    }

    ifstream filein;

    // open file for reading
    filein.open(filename);
    if (!filein)
    {
        cerr << "Error: failed to open file" << endl;
        return 1;
    }

    // read file
    string str = "";
    while (true)
    {
        filein >> str;

        if (filein.eof())
        { // end of file
            break;
        }
        else if (!filein)
        { // error
            cerr << "Error: failed to read file" << endl;
            filein.close();
            return 1;
        }
        else if (str[0] == '#')
        { // comment
            filein.ignore(256, '\n');
        }
        else if (str.compare("eye") == 0)
        {
            filein >> str >> eyepos.x >> eyepos.y >> eyepos.z >> str;
        }
        else if (str.compare("viewdir") == 0)
        {
            filein >> str >> viewdir.x >> viewdir.y >> viewdir.z >> str;
            viewdir.Normalize();
        }
        else if (str.compare("updir") == 0)
        {
            filein >> str >> updir.x >> updir.y >> updir.z >> str;
            updir.Normalize();
        }
        else if (str.compare("viewdist") == 0)
        {
            filein >> str >> viewdist >> str;
            if (viewdist < 0)
                viewdist = -viewdist;
            if (viewdist == 0)
                viewdist = 1;
        }
        else if (str.compare("fovh") == 0)
        {
            filein >> str >> fovh >> str;
            Clamp(fovh, 1, 179);
        }
        else if (str.compare("fovv") == 0)
        {
            filein >> str >> fovv >> str;
            Clamp(fovv, 1, 179);
        }
        else if (str.compare("pixwidth") == 0)
        {
            filein >> str >> pixwidth >> str;
            Clamp(pixwidth, 10, 1200);
        }
        else if (str.compare("light") == 0)
        {
            Vector vect;
            double src;
            filein >> str >> vect.x >> vect.y >> vect.z >> src >> str;
            Light * tempLight = new Light(vect, src);
            lightList.Prepend(tempLight);
        }
        else if (str.compare("material") == 0)
        {
            filein >> str >> str;
            filein >> currentMat.diffColor.red >> currentMat.diffColor.green >> currentMat.diffColor.blue;
            filein >> str >> str;
            filein >> currentMat.specColor.red >> currentMat.specColor.green >> currentMat.specColor.blue;
            filein >> str >> str;
            filein >> currentMat.ka >> currentMat.kd >> currentMat.ks >> currentMat.specExp;
            filein >> str >> currentMat.kt >> str;
            filein >> currentMat.insideRefraction >> currentMat.outsideRefraction;
            filein >> str >> str;

            Clamp(currentMat.diffColor.red, 0.0, 1.0);
            Clamp(currentMat.diffColor.green, 0.0, 1.0);
            Clamp(currentMat.diffColor.blue, 0.0, 1.0);
            Clamp(currentMat.specColor.red, 0.0, 1.0);
            Clamp(currentMat.specColor.green, 0.0, 1.0);
            Clamp(currentMat.specColor.blue, 0.0, 1.0);
            Clamp(currentMat.ka, 0.0, 1.0);
            Clamp(currentMat.kd, 0.0, 1.0);
            Clamp(currentMat.ks, 0.0, 1.0);
            if (currentMat.specExp < 1)
                currentMat.specExp = 1;
            Clamp(currentMat.kt, 0.0, 1.0);
            if (currentMat.insideRefraction <= 0.0)
                currentMat.insideRefraction = 1.0;
            if (currentMat.outsideRefraction <= 0.0)
                currentMat.outsideRefraction = 1.0;
        }
        else if (str.compare("sphere") == 0)
        {
            Vector center;
            double rad;
            filein >> str >> str >> center.x >> center.y >> center.z;
            filein >> str >> rad >> str;
            Sphere * tempSphere = new Sphere(center, rad, currentMat);
            sphereList.Prepend(tempSphere);
        }
        else if (str.compare("triangle") == 0)
        {
            Vector c1;
            Vector c2;
            Vector c3;
            filein >> str >> str >> c1.x >> c1.y >> c1.z;
            filein >> str >> str >> c2.x >> c2.y >> c2.z;
            filein >> str >> str >> c3.x >> c3.y >> c3.z;
            filein >> str >> str;
            Triangle * tempTri = new Triangle(c1, c2, c3, currentMat);
            triList.Prepend(tempTri);
        }
        else
        {
            cerr << "Error: invalid string to begin line: " << str << endl;
            filein.close();
            return 1;
        }
    }

    filein.close();

    if (viewdir.Equals(updir))
    { // vectors are parallel
        viewdir.x += 1.0;
        viewdir.Normalize();
    }

    return 0;
}

// returns RTObject if intersection, nullptr otherwise
RTObject * TraceRay(const Ray & ray, Vector & point)
{
    RTObject * retObj = nullptr;
    int numIntersect = 0;
    double dist = -1;

    sphereList.ResetIterator();
    for (Sphere * sphere = sphereList.Next(); sphere; sphere = sphereList.Next())
    {
        double a = 1.0;
        double b = 2.0*(ray.dir.x*(ray.pos.x - sphere->center.x) +
                        ray.dir.y*(ray.pos.y - sphere->center.y) +
                        ray.dir.z*(ray.pos.z - sphere->center.z));
        double c = (ray.pos.x - sphere->center.x)*(ray.pos.x - sphere->center.x) +
            (ray.pos.y - sphere->center.y)*(ray.pos.y - sphere->center.y) +
            (ray.pos.z - sphere->center.z)*(ray.pos.z - sphere->center.z) -
            sphere->radius*sphere->radius;

        double disc = b*b - 4 * a*c;

        if (disc >= 0.0)
        {
            double t;

            if (disc > 0.0)
            {// 2 intersection points
                double t1 = (-b + sqrt(disc)) / (2 * a);
                double t2 = (-b - sqrt(disc)) / (2 * a);

                t = t1;
                if (t <= EPSILON || (t > t2 && t2 > EPSILON))
                    t = t2;
            }
            else if (disc == 0.0)
            {// 1 intersection point
                t = -b / (2 * a);
            }

            if (t > EPSILON)
            {
                if (numIntersect == 0 || (numIntersect > 0 && t < dist))
                {
                    dist = t;
                    retObj = sphere;
                }
                numIntersect++;
            }
        }
    } // end sphereList

    triList.ResetIterator();
    for (Triangle * tri = triList.Next(); tri; tri = triList.Next())
    {
        Vector vectorAB = tri->cornerB - tri->cornerA;
        Vector vectorAC = tri->cornerC - tri->cornerA;

        Vector triNormal = CrossProd(vectorAB, vectorAC);

        double A = triNormal.x;
        double B = triNormal.y;
        double C = triNormal.z;
        double D = -A*tri->cornerA.x - B*tri->cornerA.y - C*tri->cornerA.z;

        double denom = A*ray.dir.x + B*ray.dir.y + C*ray.dir.z;

        if (denom > EPSILON || denom < -EPSILON) // denom != 0
        {
            double t = -(A*ray.pos.x + B*ray.pos.y + C*ray.pos.z + D) / denom;

            if (t > EPSILON)
            {
                Vector tempPoint;
                tempPoint.x = ray.pos.x + t*ray.dir.x;
                tempPoint.y = ray.pos.y + t*ray.dir.y;
                tempPoint.z = ray.pos.z + t*ray.dir.z;

                double areaABC = 0.5 * triNormal.Length();

                double areaQBC = 0.5 * CrossProd((tri->cornerC - tri->cornerB),
                    (tempPoint - tri->cornerB)).Length();

                double areaAQC = 0.5 * CrossProd((tri->cornerA - tri->cornerC),
                    (tempPoint - tri->cornerC)).Length();

                double areaABQ = 0.5 * CrossProd((tri->cornerB - tri->cornerA),
                    (tempPoint - tri->cornerA)).Length();

                double alpha = areaQBC / areaABC;
                double beta = areaAQC / areaABC;
                double gamma = areaABQ / areaABC;

                if (0 < alpha && alpha < 1 &&
                    0 < beta  && beta < 1 &&
                    0 < gamma && gamma < 1 &&
                    ((alpha + beta + gamma) - 1) < EPSILON &&
                    ((alpha + beta + gamma) - 1) > -EPSILON)
                {
                    if (numIntersect == 0 || (numIntersect > 0 && t < dist))
                    {
                        dist = t;
                        retObj = tri;
                    }
                    numIntersect++;
                }
            }
        }
    } // end triList

    if (numIntersect == 0)
        return nullptr;

    point.x = ray.pos.x + dist*ray.dir.x;
    point.y = ray.pos.y + dist*ray.dir.y;
    point.z = ray.pos.z + dist*ray.dir.z;
    return retObj;
}

Color ShadeRay(const Ray & incidentRay, const RTObject * object,
               const bool inside, const int depth)
{
    if (object == nullptr)
        return blackColor;

    Color retColor;
    retColor.red = object->material.ka * object->material.diffColor.red;
    retColor.green = object->material.ka * object->material.diffColor.green;
    retColor.blue = object->material.ka * object->material.diffColor.blue;

    if (depth > MAX_DEPTH)
        return retColor;

    Vector dirSurfNorm;
    if (object->shape == SPHERE_OBJECT)
    {
        Sphere * sphere = (Sphere *)object;
        dirSurfNorm = incidentRay.pos - sphere->center;
        dirSurfNorm.Normalize();

        if (inside)
        {
            dirSurfNorm.x = -dirSurfNorm.x;
            dirSurfNorm.y = -dirSurfNorm.y;
            dirSurfNorm.z = -dirSurfNorm.z;
        }
    }
    else
    {
        Triangle * tri = (Triangle *)object;
        dirSurfNorm = CrossProd((tri->cornerB - tri->cornerA),
            (tri->cornerC - tri->cornerA));
        dirSurfNorm.Normalize();

        if (DotProd(dirSurfNorm, incidentRay.dir) < 0.0)
        {
            dirSurfNorm.x = -dirSurfNorm.x;
            dirSurfNorm.y = -dirSurfNorm.y;
            dirSurfNorm.z = -dirSurfNorm.z;
        }

    }

    Light * light = nullptr;
    lightList.ResetIterator();
    for (light = lightList.Next(); light; light = lightList.Next())
    {
        // determine diffuse and specular lighting
        Vector dirToLight;
        if (light->source == POINT_SOURCE)
        {
            dirToLight = light->vector - incidentRay.pos;
        }
        else // DIR_SOURCE
        {
            dirToLight.x = -light->vector.x;
            dirToLight.y = -light->vector.y;
            dirToLight.z = -light->vector.z;
        }
        dirToLight.Normalize();

        Vector lv;
        lv.x = dirToLight.x + incidentRay.dir.x;
        lv.y = dirToLight.y + incidentRay.dir.y;
        lv.z = dirToLight.z + incidentRay.dir.z;
        double lvLength = lv.Length();

        Vector h;
        h.x = lv.x / lvLength;
        h.y = lv.y / lvLength;
        h.z = lv.z / lvLength;
        h.Normalize();

        double ndotl = DotProd(dirSurfNorm, dirToLight);
        if (ndotl < 0.0)
            ndotl = 0.0;

        double ndoth = DotProd(dirSurfNorm, h);
        if (ndoth < 0.0)
            ndoth = 0.0;

        double dCoef = object->material.kd * ndotl;
        double sCoef = object->material.ks * pow(ndoth, object->material.specExp);

        // determine shadow
        double shadow = 1.0;

        Ray shadowRay;
        shadowRay.pos = incidentRay.pos;
        shadowRay.dir = dirToLight;

        Vector shadowPoint;
        RTObject * prevObj = nullptr;
        RTObject * shadowObj = TraceRay(shadowRay, shadowPoint);
        while (shadowObj != nullptr)
        {
            if (shadowObj != prevObj)
                shadow *= shadowObj->material.kt;

            prevObj = shadowObj;
            shadowRay.pos = shadowPoint;
            shadowObj = TraceRay(shadowRay, shadowPoint);
        }

        // modify color for this light
        retColor.red += shadow*(dCoef*object->material.diffColor.red
                                + sCoef*object->material.specColor.red);
        retColor.green += shadow*(dCoef*object->material.diffColor.green
                                  + sCoef*object->material.specColor.green);
        retColor.blue += shadow*(dCoef*object->material.diffColor.blue
                                 + sCoef*object->material.specColor.blue);
    }

    // determine refraction
    Color refractedColor = blackColor;

    double iorI = object->material.outsideRefraction;
    double iorT = object->material.insideRefraction;
    if (object->shape == SPHERE_OBJECT && inside)
    {
        iorI = object->material.insideRefraction;
        iorT = object->material.outsideRefraction;
    }

    double cosThetaI = DotProd(incidentRay.dir, dirSurfNorm);

    double cosThetaT = 1 - pow(iorI / iorT, 2)*(1 - pow(cosThetaI, 2));
    if (cosThetaT > 0.0)
    {
        cosThetaT = sqrt(cosThetaT);

        Ray refractedRay;
        refractedRay.pos = incidentRay.pos;
        refractedRay.dir.x = cosThetaT*(-dirSurfNorm.x)
            + (iorI / iorT)*(cosThetaI*dirSurfNorm.x - incidentRay.dir.x);
        refractedRay.dir.y = cosThetaT*(-dirSurfNorm.y)
            + (iorI / iorT)*(cosThetaI*dirSurfNorm.y - incidentRay.dir.y);
        refractedRay.dir.z = cosThetaT*(-dirSurfNorm.z)
            + (iorI / iorT)*(cosThetaI*dirSurfNorm.z - incidentRay.dir.z);

        Vector refractedPoint;
        RTObject * refractedObj = TraceRay(refractedRay, refractedPoint);
        if (refractedObj != nullptr)
        {
            Ray newIncidentRay;
            newIncidentRay.pos = refractedPoint;
            newIncidentRay.dir = incidentRay.pos - refractedPoint;
            newIncidentRay.dir.Normalize();

            bool stillInside = inside;
            if (object->shape == SPHERE_OBJECT)
                stillInside = !inside;

            refractedColor = ShadeRay(newIncidentRay, refractedObj,
                                      stillInside, depth + 1);
        }
    }

    retColor.red = (1 - object->material.kt) * retColor.red
        + object->material.kt * refractedColor.red;
    retColor.green = (1 - object->material.kt) * retColor.green
        + object->material.kt * refractedColor.green;
    retColor.blue = (1 - object->material.kt) * retColor.blue
        + object->material.kt * refractedColor.blue;

    // determine relfection
    Color reflectedColor = blackColor;

    Vector vectorA;
    double lengthA = DotProd(incidentRay.dir, dirSurfNorm);
    vectorA.x = dirSurfNorm.x * lengthA;
    vectorA.y = dirSurfNorm.y * lengthA;
    vectorA.z = dirSurfNorm.z * lengthA;

    Ray reflectedRay;
    reflectedRay.pos = incidentRay.pos;
    reflectedRay.dir.x = 2 * vectorA.x - incidentRay.dir.x;
    reflectedRay.dir.y = 2 * vectorA.y - incidentRay.dir.y;
    reflectedRay.dir.z = 2 * vectorA.z - incidentRay.dir.z;
    reflectedRay.dir.Normalize();

    RTObject * reflectedObj = nullptr;
    Vector reflectedPoint;
    reflectedObj = TraceRay(reflectedRay, reflectedPoint);
    if (reflectedObj != nullptr && reflectedObj != object)
    {
        Ray newIncidentRay;
        newIncidentRay.pos = reflectedPoint;
        newIncidentRay.dir = incidentRay.pos - reflectedPoint;
        newIncidentRay.dir.Normalize();
        reflectedColor = ShadeRay(newIncidentRay, reflectedObj,
                                  inside, depth + 1);
    }

    retColor.red += object->material.ks * reflectedColor.red;
    retColor.green += object->material.ks * reflectedColor.green;
    retColor.blue += object->material.ks * reflectedColor.blue;

    // return color of incidentRay.pos
    Clamp(retColor.red, 0.0, 1.0);
    Clamp(retColor.green, 0.0, 1.0);
    Clamp(retColor.blue, 0.0, 1.0);
    return retColor;
}

void Clamp(double & value, double low, double high)
{
    if (value < low) value = low;
    if (value > high) value = high;
}

void Clamp(int & value, int low, int high)
{
    if (value < low) value = low;
    if (value > high) value = high;
}

Vector CrossProd(Vector v1, Vector v2)
{
    Vector result;
    result.x = v1.y*v2.z - v1.z*v2.y;
    result.y = v1.z*v2.x - v1.x*v2.z;
    result.z = v1.x*v2.y - v1.y*v2.x;
    return result;
}

double DotProd(Vector v1, Vector v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
