#pragma once
#include "Screen.h"
#include <list>
#include <vector>

namespace renderNameSpace {

	class RenderEngine
	{
	public:
		int xResolution;
		int yResolution;
		bool perspective;
		const double PI;
		struct Plane {
			std::vector<double> point1;
			std::vector<double> point2;
			std::vector<double> point3;
			std::vector<double> point4;
			Uint8 R;
			Uint8 G;
			Uint8 B;
		};
		struct Triangle {
			std::vector<double> point1;
			std::vector<double> point2;
			std::vector<double> point3;
		};
		struct Sphere {
			std::vector<double> center;
			double radius;
			Uint8 R;
			Uint8 G;
			Uint8 B;
		};
		struct Light {
			std::vector<double> position;
		};
		struct planeCollisionData {
			std::vector<double> collisionPosition;
			std::vector<double> planeNormal;
			double collisionDistance;
			Uint8 R;
			Uint8 G;
			Uint8 B;
		};
		struct collisionData {
			std::vector<double> collisionP;
			std::vector<double> guideV;
			double collisionDistance;
			Uint8 R;
			Uint8 G;
			Uint8 B;
		};
		int maximumLightBounces;
		double lightIntensity;
		std::list <Sphere> spheres;
		std::list <Light> lightEmitters;
		std::list <Plane> planes;
		Plane camera;
		Screen userScreen;

	public:
		RenderEngine(int, int, Screen, std::vector<double>, std::vector<double>);
		void addSphere(std::vector<double>, double, Uint8, Uint8, Uint8);
		void addPlane(std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>, Uint8, Uint8, Uint8);
		void addLightEmitter(std::vector<double>);
		double quadraticFormula(double, double, double);
		std::vector<double> crossProduct(std::vector<double>, std::vector<double>);
		double dot(std::vector<double>, std::vector<double>);
		double checkMagnitude(std::vector<double>);
		std::vector<double> convertToUnitVector(std::vector<double>);
		//void radToDegree(double&);
		std::vector<double> multiplyByScalar(std::vector<double>, double);
		std::vector<double> subtractVectors(std::vector<double>, std::vector<double>);
		std::vector<double> addVectors(std::vector<double>, std::vector<double>);
		std::vector<double> calculateBounce(std::vector<double>, std::vector<double>, std::vector<double>);
		auto findCollision(std::vector<double>, std::vector<double>);
		auto calculateSphereCollision(std::vector<double>, std::vector<double>, Sphere);
		auto calculatePlaneCollision(std::vector<double>, std::vector<double>, Plane);
		void emitAndRenderLight();
	};
}
