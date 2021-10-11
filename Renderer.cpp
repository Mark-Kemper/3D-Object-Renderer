#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace renderNameSpace;
using namespace std;


RenderEngine::RenderEngine(int xReso, int yReso, Screen pixelScreen, vector<double>  cameraPosition, vector<double>  cameraOrientation) :
	maximumLightBounces(2), lightIntensity(.1), PI(3.14159265358979323846), spheres({}), planes({}),
	lightEmitters({}), xResolution(xReso), yResolution(yReso), userScreen(pixelScreen), perspective(true) {

	//constructing camera plane with 4 points
	int div = 500;
	vector<double> cameraP1 = { -14.5, 2.5 + (double)yResolution / div, 2 - (double)xResolution / div };
	vector<double>  cameraP2 = { -14.5, 2.5 + (double)yResolution / div, 2 + (double)xResolution / div };
	vector<double>  cameraP3 = { -14.5, 2.5 - (double)yResolution / div, 2 - (double)xResolution / div };
	vector<double>  cameraP4 = { -14.5, 2.5 - (double)yResolution / div, 2 + (double)xResolution / div };
	camera = { cameraP1, cameraP2, cameraP3, cameraP4, 0, 0, 0 };
}

void RenderEngine::addSphere(vector<double> center, double radius, Uint8 r, Uint8 g, Uint8 b) {
	Sphere addedSphere = { center, radius, r, g, b };
	spheres.push_front(addedSphere);
}

void RenderEngine::addPlane(vector<double> point1, vector<double> point2, vector<double> point3, vector<double> point4, Uint8 r, Uint8 g, Uint8 b) {
	Plane addedPlane = { point1, point2, point3, point4, r, g, b };
	planes.push_front(addedPlane);
}

void RenderEngine::addLightEmitter(vector<double> position) {
	Light addedLight = { position };
	lightEmitters.push_front(addedLight);
}

double RenderEngine::quadraticFormula(double a, double b, double c)
{
	double discriminant = pow(b, 2) - 4 * a * c;
	if (discriminant >= 0)
	{
		double plus = (-b + sqrt(discriminant)) / (2 * a);
		double minus = (-b - sqrt(discriminant)) / (2 * a);
		return std::min(plus, minus); //since a vector will collide twice with a sphere (front vs back), the one that is closer is chosen
	}

	return 0;
}

vector<double> RenderEngine::crossProduct(vector<double> firstV, vector<double> secondV) {
	double i = firstV[1] * secondV[2] - firstV[2] * secondV[1];
	double j = -firstV[0] * secondV[2] + firstV[2] * secondV[0];
	double k = firstV[0] * secondV[1] - firstV[1] * secondV[0];
	vector<double> returnVector = { i, j, k };
	return returnVector;
}

double RenderEngine::dot(vector<double> firstV, vector<double> secondV) {
	return (firstV[0] * secondV[0]) + (firstV[1] * secondV[1]) + (firstV[2] * secondV[2]);
}

double RenderEngine::checkMagnitude(vector<double> vectorV) {
	return sqrt(pow(vectorV[0], 2) + pow(vectorV[1], 2) + pow(vectorV[2], 2));
}

vector<double> RenderEngine::convertToUnitVector(vector<double> vectorV) {
	vector<double> newV = multiplyByScalar(vectorV, 1 / checkMagnitude(vectorV));
	return newV;
}

vector<double> RenderEngine::multiplyByScalar(vector<double> vectorV, double scalar) {
	vector<double> newV = { vectorV[0] * scalar, vectorV[1] * scalar, vectorV[2] * scalar };
	return newV;
}

vector<double> RenderEngine::subtractVectors(vector<double> firstV, vector<double> secondV) {
	vector<double> newV = { firstV[0] - secondV[0], firstV[1] - secondV[1], firstV[2] - secondV[2] };
	return newV;
}

vector<double> RenderEngine::addVectors(vector<double> firstV, vector<double> secondV) {
	vector<double> newV = { firstV[0] + secondV[0] , firstV[1] + secondV[1] , firstV[2] + secondV[2] };
	return newV;
}

vector<double> RenderEngine::calculateBounce(vector<double> firstV, vector<double> normalV, vector<double> center) {//vector, vector, point
	//Ap = Bp + 2 * ((C + (N dot B) * N) - Bp)
	//need to be unit vectors
	vector<double> firstVPoint = subtractVectors(center, firstV);
	vector<double> bounceV = multiplyByScalar(normalV, dot(normalV, firstV));
	bounceV = subtractVectors(addVectors(center, bounceV), firstVPoint);
	bounceV = addVectors(firstVPoint, multiplyByScalar(bounceV, 2));
	return bounceV;
}

auto RenderEngine::calculateSphereCollision(vector<double> uV, vector<double> o, RenderEngine::Sphere sphere)//unit vector, origin, sphere center, radius
{
	vector<double> sC = sphere.center;
	double R = sphere.radius;
	double a = pow(uV[0], 2) + pow(uV[1], 2) + pow(uV[2], 2);
	double b = 2 * o[0] * uV[0] - 2 * sC[0] * uV[0] + 2 * o[1] * uV[1] - 2 * sC[1] * uV[1] + 2 * o[2] * uV[2] - 2 * sC[2] * uV[2];
	double c = pow(o[0], 2) - 2 * o[0] * sC[0] + pow(sC[0], 2) + pow(o[1], 2) - 2 * o[1] * sC[1] + pow(sC[1], 2) +
		pow(o[2], 2) - 2 * o[2] * sC[2] + pow(sC[2], 2) - pow(R, 2);
	double collisionTime = quadraticFormula(a, b, c);

	if (collisionTime != NULL) {
		//if (collisionTime > 0) {
		vector<double> collisionPoint = { o[0] + uV[0] * collisionTime, o[1] + uV[1] * collisionTime, o[2] + uV[2] * collisionTime };
		vector<double> guideVector = { collisionPoint[0] - sC[0], collisionPoint[1] - sC[1], collisionPoint[2] - sC[2] };

		double collisionDistance = checkMagnitude(subtractVectors(o, collisionPoint));
		guideVector = convertToUnitVector(guideVector);
		return collisionData{ collisionPoint, guideVector, collisionDistance, sphere.R, sphere.G, sphere.B };
		//}
	}

	return collisionData{ vector<double>(), vector<double>(), 0, 0, 0, 0 };
}

auto RenderEngine::calculatePlaneCollision(vector<double> uV, vector<double> o, Plane plane)// unit vector and origin position
{
	vector<double> edgeVector1 = subtractVectors(plane.point2, plane.point1);
	vector<double> edgeVector2 = subtractVectors(plane.point3, plane.point1);
	vector<double> planeNormal = crossProduct(edgeVector1, edgeVector2);
	planeNormal = convertToUnitVector(planeNormal);

	/*double rightSide = planeNormal[0] * (plane.point1[0] - o[0]) + planeNormal[1] * (plane.point1[1] - o[1]) +
		planeNormal[2] * (plane.point1[2] - o[2]);
	double leftSide = uV[0] + uV[1] + uV[2];*/

	//numerator is same as rightSide its just factored, but denominator is different than left side

	double numerator = planeNormal[0] * plane.point1[0] + planeNormal[1] * plane.point1[1]
		+ planeNormal[2] * plane.point1[2] - planeNormal[0] * o[0] - planeNormal[1] * o[1] - planeNormal[2] * o[2];

	double denominator = planeNormal[0] * uV[0] + planeNormal[1] * uV[1] + planeNormal[2] * uV[2];
	double timeForCollision = numerator / denominator;

	//if (timeForCollision >= 0) {
		//if timeForCollision is negative dont continue, but need a way to know normal vector is pointing the correct way.
	vector<double> collisionPoint = addVectors(o, multiplyByScalar(uV, timeForCollision));
	vector<double> topLeftDown = subtractVectors(plane.point3, plane.point1);
	vector<double> topLeftRight = subtractVectors(plane.point2, plane.point1);
	vector<double> topRightDown = subtractVectors(plane.point4, plane.point2);
	vector<double> topRightLeft = subtractVectors(plane.point1, plane.point2);
	vector<double> bottomLeftUp = subtractVectors(plane.point1, plane.point3);
	vector<double> bottomLeftRight = subtractVectors(plane.point4, plane.point3);
	vector<double> bottomRightUp = subtractVectors(plane.point2, plane.point4);
	vector<double> bottomRightLeft = subtractVectors(plane.point3, plane.point4);
	vector<double> checkVector1 = subtractVectors(collisionPoint, plane.point1);
	vector<double> checkVector2 = subtractVectors(collisionPoint, plane.point2);
	vector<double> checkVector3 = subtractVectors(collisionPoint, plane.point3);
	vector<double> checkVector4 = subtractVectors(collisionPoint, plane.point4);
	bool contained = true;

	if (dot(checkVector1, topLeftDown) < 0 || dot(checkVector1, topLeftRight) < 0) {
		contained = false;
	}
	else if (dot(checkVector2, topRightDown) < 0 || dot(checkVector2, topRightLeft) < 0) {
		contained = false;
	}
	else if (dot(checkVector3, bottomLeftUp) < 0 || dot(checkVector3, bottomLeftRight) < 0) {
		contained = false;
	}
	else if (dot(checkVector4, bottomRightUp) < 0 || dot(checkVector4, bottomRightLeft) < 0) {
		contained = false;
	}

	if (contained) {
		double collisionDist = checkMagnitude(subtractVectors(collisionPoint, o));
		return planeCollisionData{ collisionPoint, planeNormal, collisionDist, plane.R, plane.G, plane.B };
	}
	//}
	return planeCollisionData{ vector<double>(), vector<double>(), 0, 0, 0, 0 };
}

auto RenderEngine::findCollision(vector<double> originPosition, vector<double> unitVector) {
	double shortestDistance = -1;
	vector<double> actualCollisionPoint = vector<double>();
	vector<double> actualGuideVector = vector<double>();
	Uint8 colorR = 0;
	Uint8 colorG = 0;
	Uint8 colorB = 0;

	for (Sphere sphere : spheres) {
		auto [collisionPoint, guideVector, collisionDistance, r, g, b] = calculateSphereCollision(unitVector, originPosition, sphere);
		if (collisionPoint.size() > 0) {
			if (collisionDistance < shortestDistance || shortestDistance < 0) {
				shortestDistance = collisionDistance;
				actualCollisionPoint = collisionPoint;
				actualGuideVector = guideVector;
				colorR = r;
				colorG = g;
				colorB = b;
			}
		}
	}

	for (Plane plane : planes) {
		//unit vector, origin, sphere center, radius
		auto [collisionPoint, guideVector, collisionDistance, r, g, b] = calculatePlaneCollision(unitVector, originPosition, plane);
		if (collisionPoint.size() > 0) {
			if (collisionDistance < shortestDistance || shortestDistance < 0) {
				shortestDistance = collisionDistance;
				actualCollisionPoint = collisionPoint;
				actualGuideVector = guideVector;
				colorR = r;
				colorG = g;
				colorB = b;
			}
		}
	}
	return planeCollisionData{ actualCollisionPoint, actualGuideVector, shortestDistance, colorR, colorG, colorB };
}

void RenderEngine::emitAndRenderLight() {

	for (Light lightEmitter : lightEmitters) {
		vector<double> light = { (double)(rand()), (double)(-rand() + rand()), (double)(-rand() + rand()) };
		light = convertToUnitVector(light);

		//for (int x = 0; x < maximumLightBounces; x++) {
		auto [actualCollisionPoint, actualGuideVector, shortestDistance, colorR, colorG, colorB] =
			findCollision(lightEmitter.position, light);

		/*if (actualCollisionPoint != NULL) {
			double* bounceVector = calculateBounce(light, actualGuideVector, actualCollisionPoint);
			auto [bounceCollisionPoint, bounceGuideVector, shortestBounce, colourR, colourG, colourB] =
				findCollision(actualCollisionPoint, bounceVector);
			if (bounceCollisionPoint != NULL) {
				shortestDistance = -1;
			}
		}*/

		if (shortestDistance > 0) {
			if (perspective) {
				vector<double> cameraCenter = { (camera.point1[0] + camera.point3[0]) / 2, (camera.point1[1] + camera.point3[1]) / 2,
					(camera.point1[2] + camera.point3[2]) / 2 };

				vector<double> pointToCenterVector = subtractVectors(actualCollisionPoint, cameraCenter);
				vector<double> cameraNormal = crossProduct(subtractVectors(camera.point2, camera.point1),
					subtractVectors(camera.point3, camera.point1));
				cameraNormal = convertToUnitVector(cameraNormal);

				double multiplyFactor = dot(pointToCenterVector, cameraNormal);

				cameraNormal = multiplyByScalar(cameraNormal, multiplyFactor);

				vector<double> newCenter = addVectors(cameraCenter, cameraNormal);

				double focalGrowthRate = .3;

				//double horizontalGrowth = checkMagnitude(subtractVectors(camera.point2, camera.point1)) * 
				//double horizontalGrowth =	checkMagnitude(cameraNormal) * focalGrowthRate;
				//double verticalGrowth = checkMagnitude(subtractVectors(camera.point3, camera.point1)) *
				//double verticalGrowth =	checkMagnitude(cameraNormal) * focalGrowthRate;

				double growthMultiplier = checkMagnitude(cameraNormal) * focalGrowthRate;

				vector<double> newPoint1 = addVectors(camera.point1, multiplyByScalar(subtractVectors(camera.point1, camera.point2),
					growthMultiplier / 2));

				newPoint1 = addVectors(newPoint1, multiplyByScalar(subtractVectors(camera.point1, camera.point3),
					growthMultiplier / 2));

				newPoint1 = addVectors(newPoint1, cameraNormal);

				vector<double> newPoint2 = addVectors(camera.point2, multiplyByScalar(subtractVectors(camera.point2, camera.point1),
					growthMultiplier / 2));

				newPoint2 = addVectors(newPoint2, multiplyByScalar(subtractVectors(camera.point2, camera.point4),
					growthMultiplier / 2));

				newPoint2 = addVectors(newPoint2, cameraNormal);

				vector<double> newPoint3 = addVectors(camera.point3, multiplyByScalar(subtractVectors(camera.point3, camera.point4),
					growthMultiplier / 2));

				newPoint3 = addVectors(newPoint3, multiplyByScalar(subtractVectors(camera.point3, camera.point1),
					growthMultiplier / 2));

				newPoint3 = addVectors(newPoint3, cameraNormal);

				vector<double> newPoint4 = addVectors(camera.point4, multiplyByScalar(subtractVectors(camera.point4, camera.point3),
					growthMultiplier / 2));

				newPoint4 = addVectors(newPoint4, multiplyByScalar(subtractVectors(camera.point4, camera.point2),
					growthMultiplier / 2));

				newPoint4 = addVectors(newPoint4, cameraNormal);


				Plane newPerspectivePanel = { newPoint1, newPoint2, newPoint3, newPoint4 };

				double horizontalDistance = checkMagnitude(subtractVectors(actualCollisionPoint, newPerspectivePanel.point1)) *
					dot(convertToUnitVector(subtractVectors(newPerspectivePanel.point2, newPerspectivePanel.point1)),
						convertToUnitVector(subtractVectors(actualCollisionPoint, newPerspectivePanel.point1)));

				double verticalDistance = sqrt(pow(checkMagnitude(subtractVectors(actualCollisionPoint, newPerspectivePanel.point1)), 2) - pow(horizontalDistance, 2)); //pytharogran thereom
				int xPixel = (int)((horizontalDistance / checkMagnitude(subtractVectors(newPerspectivePanel.point2, newPerspectivePanel.point1))) * xResolution);
				int yPixel = (int)((verticalDistance / checkMagnitude(subtractVectors(newPerspectivePanel.point3, newPerspectivePanel.point1))) * yResolution);

				vector<double> cameraNormalP = crossProduct(subtractVectors(camera.point2, camera.point1),
					subtractVectors(camera.point3, camera.point1));

				cameraNormal = convertToUnitVector(cameraNormalP);
				actualGuideVector = convertToUnitVector(actualGuideVector);

				double intensity = .8 * abs(dot(actualGuideVector, cameraNormal));
				double rColor = colorR * intensity;
				double gColor = colorG * intensity;
				double bColor = colorB * intensity;

				vector<double> pointOnCamera = addVectors(camera.point1,
					multiplyByScalar(subtractVectors(camera.point2, camera.point1), (double)xPixel / xResolution));
				pointOnCamera = addVectors(pointOnCamera,
					multiplyByScalar(subtractVectors(camera.point3, camera.point1), (double)yPixel / yResolution));

				/*
				vector<double> checkObstructionVector = subtractVectors(actualCollisionPoint, pointOnCamera);

				auto [obstrCollisionPoint, obstrGuideVector, obstrShortestDistance, uselessR, uselessG, uselessB] =
					findCollision(actualCollisionPoint, checkObstructionVector);
					*/
					//bool obstructed = obstrCollisionPoint.size() == 0 || abs(obstrShortestDistance) > .03;
				bool obstructed = false;

				if (xPixel < xResolution && xPixel >= 0 && yPixel < yResolution && yPixel >= 0 && !obstructed) {
					userScreen.incrementPixel(xPixel, yPixel, rColor, gColor, bColor);
					//userScreen.setEntireScreen(rand(), rand(), rand());
				}
			}


			//Orthogonal is just a perspective with a focalGrowthRate of 0, so no need for separate branch.
			if (!perspective) { //Orthogonal
				vector<double> cameraNormal = crossProduct(subtractVectors(camera.point2, camera.point1),
					subtractVectors(camera.point3, camera.point1));
				cameraNormal = convertToUnitVector(cameraNormal);
				vector<double> reverseCameraNormal = multiplyByScalar(cameraNormal, -1);
				auto [collisionPoint, planeNormal, collisionDistance, r, g, b] = calculatePlaneCollision(reverseCameraNormal, actualCollisionPoint, camera);
				if (collisionPoint.size() > 0) {
					actualGuideVector = convertToUnitVector(actualGuideVector);
					double intensity = .8 * abs(dot(actualGuideVector, reverseCameraNormal));
					//assuming rectangular:
					//assuming point1 and point2 are the top ones. ( 1 and 3 is vertical)
					//and point 3 and point4 are the bottom ones. (3 and 4 (or 1 and 2) is horizontal)
					//horizontalDistance = p1 between | collisionPoint - p1 | *2 * ((collisionPoint - p1) dot(p3 - p1));
					double horizontalDistance = checkMagnitude(subtractVectors(collisionPoint, camera.point1)) *
						dot(convertToUnitVector(subtractVectors(camera.point2, camera.point1)),
							convertToUnitVector(subtractVectors(collisionPoint, camera.point1)));
					double verticalDistance = sqrt(pow(checkMagnitude(subtractVectors(collisionPoint, camera.point1)), 2) - pow(horizontalDistance, 2)); //pytharogran thereom
					int xPixel = (int)((horizontalDistance / checkMagnitude(subtractVectors(camera.point2, camera.point1))) * xResolution);
					int yPixel = (int)((verticalDistance / checkMagnitude(subtractVectors(camera.point3, camera.point1))) * yResolution);
					double rColor = (colorR * intensity);
					double gColor = (colorG * intensity);
					double bColor = (colorB * intensity);
					if (xPixel < xResolution && xPixel >= 0 && yPixel < yResolution && yPixel >= 0) {
						userScreen.incrementPixel(xPixel, yPixel, rColor, gColor, bColor);
						//userScreen.setEntireScreen(rand(), rand(), rand());
					}
				}
			}
		}
		/* bounce vector here not implemented yet
		  */
	}
}