#include <iostream>
#include "RunTests.h"
#include "Tuple.h"
#include "Canvas.h"
#include "Utils.h"
#include "Matrix.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Lighting.h"
#include "World.h"
#include "Camera.h"
#include "Plane.h"
#include "StripePattern.h"
#include "Chrono.h"
#include "GLWindow.h"
#include "TestPattern.h"
#include "GradientPattern.h"
#include "RingPattern.h"
#include "CheckerPattern.h"

// For 100x50 in Render::Test
// Allocation depth: Release: 30, Debug: 35
// Debug: Average duration = 1066.16 ms (10 runs)
// Release: Average duration = 28.4016 ms (10 runs)

//  after optimization in World::Intersect by passing Intersections&
// Debug: Average duration = 563.732 ms, and 33 depth
// Release: Average duration = 14.3008 ms, and 27 depth

// after fastcall & removed security checks & fast-fp & omit stack, etc.:
// Release: Average duration = 11.3007 ms

// Release 1000x500: Average duration = 788.045 ms
// Release 1000x500: Average duration = 579.733 ms (Intersection set changed to a vector)
// Release 1000x500: Average duration = 549.231 ms (Camera _localOrigin stored)
//                   Average duration = 543.831 ms (Canvas::WritePixelFast)
//                   Average duration = 550.131 ms (removing World::ColorAt if(!hit). Seems like it negates some optimisations..)
//                   Average duration = 554.432 ms (storing _smallestNonNegativeIndex in Intersections. Seems to be not good.)
//                   Average duration = 552.132 ms (storing _smallestNonNegative instead. Not better.)
//                   Average duration = 545.331 ms (Canvas Color* _pix instead of vector)
//                   Average duration = 550.732 ms (Canvas _pix is a Dynarray)
//                   Average duration = 370.821 ms (With 2 threads in Camera::Render)
//                   Average duration = 340.319 ms (With 3 threads in Camera::Render)
//                   Average duration = 303.117 ms (With 4 threads in Camera::Render)
//                   Average duration = 302.117 ms (With 8 threads in Camera::Render: Doesn't help if more than logical units)
//                   Average duration = 351.22  ms (with 4 threads)
//                   Average duration = 336.319 ms (why is it slower now?)
//                   Average duration = 299.217 ms (still 4 threads. It might be depending on status of battery level)
//                   Average duration = 363.421 ms (after matrix no default init and transpose optimization)
//                   Average duration = 344.52 ms (after changing inlining in project properties)
//                   Average duration = 264.915 ms (World::GetIntersectHit to only keep 'best first hit')
// Debug 100x50: Average duration = 461.826 ms

// Overloading Global new operator
//static int GAllocCount = 0;
//static int GTotalAllocCount = 0;
//static int GDeepest = 0;
//void* operator new(size_t sz)
//{
//	void* m = malloc(sz);
//	//std::cout << "Alloc #" << GAllocCount << " New " << m << " size:" << sz << std::endl;
//	GAllocCount++;
//	GTotalAllocCount++;
//	if (GAllocCount > GDeepest) {
//		GDeepest = GAllocCount;
//		std::cout << "New deepest: Alloc #" << GAllocCount << " New " << m << " size:" << sz << std::endl;
//	}
//	return m;
//}
//// Overloading Global delete operator
//void operator delete(void* m)
//{
//	GAllocCount--;
//	//std::cout << "Alloc #" << GAllocCount << " Free " << m << std::endl;
//	free(m);
//}
//// Overloading Global new[] operator
//void* operator new[](size_t sz)
//{
//	void* m = malloc(sz);
//	std::cout << "Alloc #" << GAllocCount << " New[] " << m << " size:" << sz << std::endl;
//	GAllocCount++;
//	return m;
//}
//// Overloading Global delete[] operator
//void operator delete[](void* m)
//{
//	GAllocCount--;
//	std::cout << "Alloc #" << GAllocCount << " Delete[] " << m << std::endl;
//	free(m);
//}

class Game {
public:
	class Projectile {
	public:
		Point position;
		Vector velocity;
	};

	class Environment {
	public:
		Vector gravity;
		Vector wind;
	};

	static Projectile Tick(Environment env, Projectile proj) {
		Point position = proj.position + proj.velocity;
		Vector velocity = proj.velocity + env.gravity + env.wind;
		return Projectile{ position, velocity };
	}

	static void GameTest() {
		Canvas canvas(20, 20);
		Projectile p{ Point(0,1,0), Vector(1,1,0).Normalize()*1.3 };
		Environment e{ Vector(0, -0.1, 0), Vector(-0.01, 0, 0) };
		int countStep = 0;
		do {
			canvas.WritePixel(p.position.x, 19 - p.position.y, Color(255, 255, 0));
			p = Tick(e, p);
			std::cout << "Step " << countStep << " pos=" << p.position.ToString() << std::endl;
			countStep++;
		} while (p.position.y > 0);
		canvas.SaveToFile("test.ppm");
	}
};

class Clock {
public:
	static void ClockTest() {
		const int kSize = 100;
		const double radius = 48;
		const int nbTick = 12;
		Canvas canvas(kSize, kSize);

		for (int i = 0; i < nbTick; i++) {
			double angle = 2 * M_PI / nbTick * i;
			Matrix<4, 4> transform = IdentityMatrix.RotationZ(angle).Scaling({ radius, radius, radius }).Translation({ kSize / 2, kSize / 2, 0 });
			Point p(0, 1, 0);
			Point r = transform * p;
			canvas.WritePixel(r.x, r.y, Color::White);
		}
		canvas.SaveToFile("test.ppm");
	}
};

class Render {
public:
	static void Test() {
		World world;

		//auto floor = std::make_unique<Sphere>();
		//floor->SetTransform(Scaling({ 10,0.01,10 }));
		auto floor = std::make_unique<Plane>();
		auto floorMat = world._materialManager.CreateMaterial();
		floorMat->color = Color(1, 0.9, 0.9);
		floorMat->specular = 0;
		//floorMat->pattern = std::make_unique<StripePattern>(Color(1, 0.9, 0.9), Color(0.2, 0.2, 0.2));
		floorMat->pattern = std::make_unique<CheckerPattern>(Color(1, 0.9, 0.9), Color(0.2, 0.2, 0.2));
		floor->SetMaterial(floorMat);

		//auto leftWall = std::make_unique<Sphere>();
		//leftWall->SetTransform(
		//	Translation({ 0,0,5 })*
		//	RotationY(-M_PI / 4)*
		//	RotationX(M_PI / 2)*
		//	Scaling({ 10, 0.01, 10 }));
		auto leftWall = std::make_unique<Plane>();
		leftWall->SetTransform(Translation({ 0,0,10 }) * RotationX(M_PI / 2));
		leftWall->SetMaterial(floorMat);

		//auto rightWall = std::make_unique<Sphere>();
		//rightWall->SetTransform(
		//	Translation({ 0,0,5 })*
		//	RotationY(M_PI / 4)*
		//	RotationX(M_PI / 2)*
		//	Scaling({ 10, 0.01, 10 }));
		//rightWall->SetMaterial(floorMat);

		auto middle = std::make_unique<Sphere>();
		middle->SetTransform(Translation({ -0.5, 1, 0.5 }));
		auto middleMat = world._materialManager.CreateMaterial();
		middleMat->color = Color(0.1, 1, 0.5);
		middleMat->diffuse = 0.7;
		middleMat->specular = 0.3;
		//middleMat->pattern = std::make_unique<StripePattern>(Color(0.1, 1, 0.5), Color(0.9, 0.8, 0.2));
		middleMat->pattern = std::make_unique<TestPattern>();
		middleMat->pattern->SetTransform(Scaling({ 0.4, 0.4, 0.4 }) * RotationY(M_PI / 4));
		middle->SetMaterial(middleMat);

		auto right = std::make_unique<Sphere>();
		right->SetTransform(Translation({ 1.5, 0.5, -0.5 }) * Scaling({ 0.5, 0.5, 0.5 }));
		auto rightMat = world._materialManager.CreateMaterial();
		rightMat->color = Color(0.5, 1, 0.1);
		rightMat->diffuse = 0.7;
		rightMat->specular = 0.3;
		//rightMat->pattern = std::make_unique<StripePattern>(Color(0.5, 1, 0.1), Color(0.5, 0.1, 0.1));
		rightMat->pattern = std::make_unique<CheckerPattern>(Color(0.5, 1, 0.1), Color(0.5, 0.1, 0.1));
		rightMat->pattern->SetTransform(Scaling({ 0.2, 0.2, 0.2 }));
		right->SetMaterial(rightMat);

		auto left = std::make_unique<Sphere>();
		left->SetTransform(Translation({ -1.5, 0.33, -0.75 }) * Scaling({ 0.33, 0.33, 0.33 }));
		auto leftMat = world._materialManager.CreateMaterial();
		leftMat->color = Color(1, 0.8, 0.1);
		leftMat->diffuse = 0.7;
		leftMat->specular = 0.3;
		//leftMat->pattern = std::make_unique<StripePattern>(Color(1, 0.8, 0.1), Color(0.1, 0.1, 0.3));
		leftMat->pattern = std::make_unique<GradientPattern>(Color(1, 0.8, 0.1), Color(0.1, 0.1, 0.3));
		leftMat->pattern->SetTransform(RotationZ(M_PI_4));
		left->SetMaterial(leftMat);

		world.SetLightSource(std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1)));

		const int kWidth = 320;
		const int kHeight = 180;

		Camera camera(kWidth, kHeight, M_PI / 3);
		world.AddObject(std::move(floor));
		world.AddObject(std::move(leftWall));
		//world.AddObject(std::move(rightWall));
		world.AddObject(std::move(middle));
		world.AddObject(std::move(left));
		world.AddObject(std::move(right));

		camera.SetTransform(ViewTransform(Point(0, 1.5, -5),
			Point(0, 1, 0),
			Vector(0, 1, 0)));

		GLWindow glWindow(kWidth, kHeight);
		glWindow.SetRenderCallback([&](const Vector& from, const Point& to, const Vector& up) {
			camera.SetTransform(ViewTransform(from,
				to,
				up));
			rightMat->pattern->SetTransform(rightMat->pattern->GetTransform() * RotationX(0.01) * RotationY(0.009) * RotationZ(0.008));
			return camera.Render(world);
		});
		glWindow.StartMainLoop();

		Canvas canvas = camera.Render(world);
		canvas.SaveToFile("test.ppm");
	}
};

int __cdecl main(int /*argc*/, char** /*argv*/) {
	std::cout << "Hello World!\n";

	RunTests();

	//Game::GameTest();
	//Clock::ClockTest();

	double average = 0;
	constexpr int NBITER = 1;
	for (int i = 0; i < NBITER; i++) {
		Chrono start;
		Render::Test();
		double duration = start.GetElapsedTime();
		average += duration;
		std::cout << "Duration = " << duration << std::endl;
	}
	average /= NBITER;
	std::cout << "Average duration = " << average << std::endl;

	//std::cout << "GTotalAllocCount=" << GTotalAllocCount << std::endl;
	//std::cout << "GAllocCount should be 0 or almost:" << GAllocCount << " deepest was: " << GDeepest << std::endl;
}
