#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "vecmath.h"
using namespace std;

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;



// You will need more global variables to implement color and position changes
int colorId = 0;
float lightX = 1.0f;
float lightY = 1.0f;
std::string filename = "";
int angle = 0;
bool spin = false;

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a)
{
	glVertex3fv(a);
}

inline void glNormal(const Vector3f &a)
{
	glNormal3fv(a);
}


//
void rotate(int value) {
	if(spin){
		if (angle > 360) {
			angle = 0;
		}
		angle++;
		glutPostRedisplay();
		glutTimerFunc(10, rotate, 0);
		cout << "Angle: " << angle << "\n";
	}
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		exit(0);
		break;

	case 'r':
		spin = !spin;
		glutTimerFunc(0, rotate, 0);
		break;
	case 'c':
		// add code to change color here
		colorId = (colorId + 1) % 4;
		cout << "Color Id = " << colorId << "." << endl;
		break;

	default:
		cout << "Unhandled key press " << key << "." << endl;
	}

	// this will refresh the screen so that the user sees the color change
	glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		// add code to change light position
		lightY += 0.5;
		break;
	case GLUT_KEY_DOWN:
		// add code to change light position
		lightY -= 0.5;
		break;
	case GLUT_KEY_LEFT:
		// add code to change light position
		lightX -= 0.5;

		break;
	case GLUT_KEY_RIGHT:
		// add code to change light position
		lightX += 0.5;
		break;
	}

	// this will refresh the screen so that the user sees the light position
	glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
	int i;

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate the image
	glMatrixMode(GL_MODELVIEW);  // Current matrix affects objects positions
	glLoadIdentity();              // Initialize to the identity

								   // Position the camera at [0,0,5], looking at [0,0,0],
								   // with [0,1,0] as the up direction.
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	// Set material properties of object

	// Here are some colors you might use - feel free to add more
	GLfloat diffColors[4][4] = { { 0.5, 0.5, 0.9, 1.0 },
	{ 0.9, 0.5, 0.5, 1.0 },
	{ 0.5, 0.9, 0.3, 1.0 },
	{ 0.3, 0.8, 0.9, 1.0 } };

	// Here we use the first color entry as the diffuse color
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[colorId]);

	// Define specular color and shininess
	GLfloat specColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 100.0 };

	// Note that the specular color and shininess can stay constant
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// Set light properties

	// Light color (RGBA)
	GLfloat Lt0diff[] = { 1.0,1.0,1.0,1.0 };
	// Light position
	GLfloat Lt0pos[] = { lightX, lightY, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	glRotatef(angle, 0, 1, 0);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	//glutSolidTeapot(1.0);
	for (std::vector<std::vector<unsigned>>::iterator it = vecf.begin(); it != vecf.end(); ++it) {
		glBegin(GL_TRIANGLES);
		glNormal3d(vecn[(*it)[2] - 1][0], vecn[(*it)[2] - 1][1], vecn[(*it)[2] - 1][2]); // c-1
		glVertex3d(vecv[(*it)[0] - 1][0], vecv[(*it)[0] - 1][1], vecv[(*it)[0] - 1][2]); // a-1
		glNormal3d(vecn[(*it)[5] - 1][0], vecn[(*it)[5] - 1][1], vecn[(*it)[5] - 1][2]); // f-1
		glVertex3d(vecv[(*it)[3] - 1][0], vecv[(*it)[3] - 1][1], vecv[(*it)[3] - 1][2]); // d-1
		glNormal3d(vecn[(*it)[8] - 1][0], vecn[(*it)[8] - 1][1], vecn[(*it)[8] - 1][2]); // i-1
		glVertex3d(vecv[(*it)[6] - 1][0], vecv[(*it)[6] - 1][1], vecv[(*it)[6] - 1][2]); // g-1
		glEnd();
	}
	// Dump the image to the screen.
	glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
	glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
	glEnable(GL_LIGHTING);     // Enable lighting calculations
	glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
	// Always use thve largest square viewport possible
	if (w > h) {
		glViewport((w - h) / 2, 0, h, h);
	}
	else {
		glViewport(0, (h - w) / 2, w, w);
	}

	// Set up a perspective view, with square aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 50 degree fov, uniform aspect ratio, near = 1, far = 100
	gluPerspective(50.0, 1.0, 1.0, 100.0);
}

// method to split strings
std::vector<std::string> splitString(std::string s, std::string delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s);
	return tokens;
}

void loadInput()
{
	// load the OBJ file here
	char buffer[256];

	while (cin.getline(buffer, 256)) {
		stringstream ss;
		std::string line;
		ss << buffer;
		line = ss.str();
		// split the string
		std::vector<std::string> values = splitString(line, " ");
		if (values.empty()) {
			continue;
		}
		char state = ' ';
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (values[0].compare("v") == 0) {
			std::stringstream(values[1]) >> x;
			std::stringstream(values[2]) >> y;
			std::stringstream(values[3]) >> z;
			vecv.push_back(Vector3f(x, y, z));
		}
		else if (values[0].compare("vn") == 0) {
			std::stringstream(values[1]) >> x;
			std::stringstream(values[2]) >> y;
			std::stringstream(values[3]) >> z;
			vecn.push_back(Vector3f(x, y, z));
		}
		else if (values[0].compare("f") == 0) {
			std::vector<unsigned> indices;

			for (int i = 1; i < 4; i++) {
				std::vector<std::string> subvalues = splitString(values[i], "/");
				for (std::vector<std::string>::iterator it = subvalues.begin(); it != subvalues.end(); ++it) {
					indices.push_back(std::stoi(*it));
				}
			}

			vecf.push_back(indices);
		}

	}
	return;

}



// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv)
{
	loadInput();

	glutInit(&argc, argv);

	// We're going to animate it, so double buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Initial parameters for window position and size
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(360, 360);
	glutCreateWindow("Assignment 0");

	// Initialize OpenGL parameters.
	initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
	glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

									// Set up the callback function for resizing windows
	glutReshapeFunc(reshapeFunc);

	// Call this whenever window needs redrawing
	glutDisplayFunc(drawScene);

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop();

	

	return 0;	// This line is never reached.
}
