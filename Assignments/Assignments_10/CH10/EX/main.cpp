#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <QApplication>
#include <QWidget>
#include <random>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>

const double HL = 10;
const double XMAX = HL;
const double XMIN = -XMAX;
const double YMAX = HL;
const double YMIN = -YMAX;
const double ZMAX = HL;
const double ZMIN = -ZMAX;
const double v = 18;
const double omega = 0.2;
const double dt = 0.01;
SoCoordinate3 *coordinates = NULL;
SoPointSet *pointSet = NULL;

class Point
{
	public:
		double x, y, z, vx, vy, vz;
		size_t id;
		Point(double _x, double _y, double _z, double _vx, double _vy, double _vz, size_t _id)
		:x(_x), y(_y), z(_z), vx(_vx), vy(_vy), vz(_vz), id(_id){}

		void updateCoordinates(double& r, double& vr, const double rMin, const double rMax)
		{
			r += vr * dt;

			if (r < rMin)
			{
				r = 2 * rMin - r;
				vr = -vr;
			}else if (r > rMax)
			{
				r = 2 * rMax - r;
				vr = -vr;
			}
		}

		void move()
		{
			updateCoordinates(x, vx, XMIN, XMAX);
			updateCoordinates(y, vy, YMIN, YMAX);
			updateCoordinates(z, vz, ZMIN, ZMAX);
			coordinates->point.set1Value(id, x, y, z);
		}
};


std::vector<Point> pointVec;
SbRotation rotationGen1;
SoRotation *rotation1 = NULL;
double Omega = 0;

void callback(void *, SoSensor *)
{
	for (size_t i = 0; i < pointVec.size(); i++)
	{
		pointVec[i].move();
	}

	Omega += omega * dt;
	rotationGen1.setValue(SbVec3f(1,1,1), Omega);
	rotation1->rotation.setValue(rotationGen1.getValue());
}

void drawLine(SoSeparator *const aSep, const Point& p1, const Point& p2)
{
		// coordinates for the starting and ending points of the axes
		SoCoordinate3 *arrow = new SoCoordinate3();
		arrow->point.set1Value(0, p1.x, p1.y, p1.z);
		arrow->point.set1Value(1, p2.x, p2.y, p2.z); 
		aSep->addChild(arrow);

		// drawing style
		SoDrawStyle *ds= new SoDrawStyle();
		ds->style=SoDrawStyle::LINES;
		ds->lineWidth=3;
		aSep->addChild(ds);

		// material/color for the axes
		SoMaterial *aMaterial = new SoMaterial();
		aMaterial->diffuseColor.setValue(0.5,0.5,0.5); 
		aSep->addChild(aMaterial);

		// a LineSet, which uses the Coordinates to draw the axes lines
		SoLineSet *line = new SoLineSet();
		line->numVertices=2;
		aSep->addChild(line);
}

int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	QWidget mainwin;
	SoQt::init(&mainwin);

	// The root of a scene graph
	SoSeparator *root = new SoSeparator;
	root->ref();

	SoQtExaminerViewer *eviewer = new SoQtExaminerViewer(&mainwin);
	eviewer->setSceneGraph(root);
	eviewer->show();

	rotation1 = new SoRotation;
	root->addChild(rotation1);

	SbRotation rotationGen0 = SbRotation(SbVec3f(1,1,1), SbVec3f(0,1,0));
	SoRotation *rotation0 = new SoRotation;
	rotation0->rotation.setValue(rotationGen0.getValue());
	root->addChild(rotation0);



	// Build frame
	SoSeparator *aSep=new SoSeparator();
	root->addChild(aSep);
	drawLine(aSep, Point(XMIN,YMIN,ZMIN,0,0,0,0), Point(XMAX,YMIN,ZMIN,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMIN,ZMIN,0,0,0,0), Point(XMAX,YMAX,ZMIN,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMAX,ZMIN,0,0,0,0), Point(XMIN,YMAX,ZMIN,0,0,0,0));
	drawLine(aSep, Point(XMIN,YMAX,ZMIN,0,0,0,0), Point(XMIN,YMIN,ZMIN,0,0,0,0));

	drawLine(aSep, Point(XMIN,YMIN,ZMAX,0,0,0,0), Point(XMAX,YMIN,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMIN,ZMAX,0,0,0,0), Point(XMAX,YMAX,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMAX,ZMAX,0,0,0,0), Point(XMIN,YMAX,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMIN,YMAX,ZMAX,0,0,0,0), Point(XMIN,YMIN,ZMAX,0,0,0,0));

	drawLine(aSep, Point(XMIN,YMIN,ZMIN,0,0,0,0), Point(XMIN,YMIN,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMIN,ZMIN,0,0,0,0), Point(XMAX,YMIN,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMAX,YMAX,ZMIN,0,0,0,0), Point(XMAX,YMAX,ZMAX,0,0,0,0));
	drawLine(aSep, Point(XMIN,YMAX,ZMIN,0,0,0,0), Point(XMIN,YMAX,ZMAX,0,0,0,0));
	

	// Adding the Coordinates and the PointSet to the root separator
	coordinates = new SoCoordinate3();
	root->addChild(coordinates);
	pointSet =new SoPointSet;
	root->addChild(pointSet);

	// Instantiate a random number generator identical to the deficient RANDU
	std::linear_congruential_engine<uint_fast32_t,     65539,    0,0X80000000> randu(1);
	double n_points = 1e4;
	std::random_device rndDev;
	std::mt19937_64 rEng(rndDev());
	std::mt19937_64 vEng(rndDev());
	std::uniform_real_distribution<double> rRnd(-2,2);
	std::uniform_real_distribution<double> vRnd(-3,3);
	double v1 = vRnd(vEng), v2 = vRnd(vEng), v3 = vRnd(vEng);
	double vx = v * v1 / std::sqrt(v1*v1 + v2*v2 + v3*v3);
	double vy = v * v2 / std::sqrt(v1*v1 + v2*v2 + v3*v3);
	double vz = v * v3 / std::sqrt(v1*v1 + v2*v2 + v3*v3);
	for (int i=0;i<n_points;i++) {
	// generate new random coordinates
		double xRnd = rRnd(rEng);
		double yRnd = rRnd(rEng);
		double zRnd = rRnd(rEng);
		double X = xRnd + zRnd;
		double Z = yRnd - 0.5 * xRnd;
		double Y = zRnd + xRnd + 5 * yRnd;
		coordinates->point.set1Value(i,X,Y,Z);
		pointVec.push_back(Point(X, Y, Z, vx, vy, vz, i));
	}
	pointSet->numPoints=n_points;

	// Schedule the update right here:
	SoTimerSensor *timerSensor = new SoTimerSensor;
	timerSensor->setFunction(callback);
	timerSensor->setInterval(0.01);
	timerSensor->schedule();

	// Pop up the main window.
	SoQt::show(&mainwin);

	eviewer->viewAll();

	// Loop until exit.
	SoQt::mainLoop();

	// Clean up resources.
	delete eviewer;
	root->unref();

	return 0;
}
