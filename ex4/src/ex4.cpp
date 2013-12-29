//
//  ex1.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "ex4.h"
#include "Model.h"
#include "ShaderIO.h"

/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ANIMATE         ('a') // Key used to start animation              //
#define KEY_ESC            ('\e') // Key used to terminate the program - ESC  //
#define KEY_QUIT            ('q') // Key used to terminate the program        //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME       ('w') // Key used to toggle wireframe rendering   //
#define KEY_PERSPECTIVE     ('p') // Key used to toggle ortho/perspect view   //
#define KEY_DECREASE_SPEC	('-')
#define KEY_INCREASE_SPEC	('=')
#define KEY_NORMAL_EST_MODE	('n')



/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** timer callback */
void timer(int value) ;
void shadersTimer(int value);

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    isAnimating = false;
bool    g_duringAnimation = false;
bool 	shadersAutoReloadEnabled = false;

// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model _model(WINDOW_SIZE, WINDOW_SIZE);

/** main function */
int main(int argc, char* argv[])
{
	std::cout << "Starting ex4..." << std::endl;

	// Initialize GLUT
	glutInit(&argc, argv) ;
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow("CG Ex4");

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	int glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(1);
	}

#ifdef __APPLE__
	GLint sync = 1;
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

	// Set callback functions:
	glutDisplayFunc(display) ;
	glutReshapeFunc(windowResize) ;
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(3000, shadersTimer, 0);   // uint millis int value
	glutTimerFunc(100, timer, 0);   // uint millis int value

	// Init anything that can be done once and for all:
	_model.init(argv[1]);

	// Set clear color to black:
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//start animation
	isAnimating = true;

	// Start events/drawing loop
	glutMainLoop();

	return 0;
}

void display(void)
{
	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	// Let the model to draw itself...
	_model.draw();

	//glDisable(GL_DEPTH_TEST);
	_model.draw2D();


	// Swap those buffers so someone will actually see the results... //
	glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{
	// Update model to fit the new resolution
	_model.resize(w, h);

	// set the new viewport //
	glViewport(0, 0, w, h);

	// Refresh the display //
	glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
	unsigned int lower_key = tolower(key);

	switch(lower_key)
	{
	case KEY_RESET:
		// reset to initial view of the object
		_model.resetMatrices();
		break;

	case 'k':
		shadersAutoReloadEnabled = !shadersAutoReloadEnabled;
		break;

	case KEY_RELOAD:
		// Reload the shading programs of the object
		_model.loadShaders();
		std::cout << "loading shaders" << std::endl;
		break;

	case KEY_WIREFRAME:
		// Toggle wireframe mode
		_model.changePolygonMode();
		break;
	case KEY_ANIMATE:
		isAnimating = !isAnimating;
		break;
	case KEY_PERSPECTIVE:
		_model.toggleProjectionMode();
		break;
	case KEY_QUIT:
	case KEY_ESC:
		// Terminate the program:
		exit(RC_OK);
		break;

	case KEY_DECREASE_SPEC:
		_model.decreaseSpec();
		break;
	case KEY_INCREASE_SPEC:
		_model.increaseSpec();
		break;


	case '1':
		_model.setShadingMode(Model::SHADING_PHONG);
		break;
	case '2':
		_model.setShadingMode(Model::SHADING_GOURAUD);
		break;
	case '3':
		_model.setShadingMode(Model::SHADING_RGB);
		break;

	case KEY_NORMAL_EST_MODE:
		_model.toggleNormalMode();
		break;


		// New in Ex4 !!!
	case 't':
		//	T/t: Switch between texturing modes (see above).
		_model.nextTextureMode();
		break;

	case 's':
		//	S/s: Decrease the scaling of the texture coordinates (or the scale of the bumps)
		_model.decreaseTextureScale();
		break;

	case 'd':
		//	D/d: Increase the scaling of the texture coordinates (or the scale of the bumps)
		_model.increaseTextureScale();
		break;

	case 'f':
		//	F/f: Decrease turbulence magnitude
		_model.decreaseTurbulenceMagnitude();
		break;

	case 'g':
		//	G/g: Increase turbulence magnitude
		_model.increaseTurbulenceMagnitude();
		break;






	default:
		std::cerr << "Key " << lower_key << " undefined\n";
		break;
	}

	return;
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		_model.setMouseFlag(button, x, y);
	}
	else if (state == GLUT_UP)
	{
		_model.resetMouseFlag(button);
	}
}


	/********************************************************************
	 * Function  :   motion
	 * Arguments :   x   : x value of the current mouse location
	 *               y   : y value of the current mouse location
	 * Returns   :   n/a
	 * Throws    :   n/a
	 *
	 * Purpose   :   This function handles mouse dragging events.
	 *
 \******************************************************************/
	void motion(int x, int y)
	{
		_model.motion(x,y);
		return;
	}


	void timer(int value) {

		glutTimerFunc(20, timer, 0);   // uint millis int value
		glutPostRedisplay();

	}

	void shadersTimer(int value) {
		if (shadersAutoReloadEnabled)
		{
			_model.loadShaders();
		}
		glutTimerFunc(1000, shadersTimer, 0);   // uint millis int value
	}

