// ImageViewer.cpp : Defines the entry point for the console application.
//
// OpenGL2.cpp : Defines the entry point for the console application.
//
//#include "OpenGLImage.h"
#include "Globals.h";
#include "ImageWindow.h"
GLubyte *file_data = NULL;
GLint viewport[4];
Extent world = {-1, 1, -1, 1};

double xScreenToWorld(int x) {
	return ((world.r - world.l) * (x - viewport[0]) / viewport[2]) + world.l;
}

double yScreenToWorld(int y) {
	return ((world.t - world.b) * ((viewport[3] - y) - viewport[1]) / viewport[3]) + world.b;
}
void initialize()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world.l, world.r, world.b, world.t);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, viewport);
}


// Global variables:
ImageWindow *img_win;
int left_mouse_down = 0;
float startxTopRight;
float startyTopRight;
float startxBotLeft;
float startyBotLeft;

float endxTopRight;
float endyTopRight;
float endxBotLeft;
float endyBotLeft;

double redValue = 0;
double greenValue = 0;
double blueValue = 0;


//
// GLUT display function
// 

void drawEnd(){
	glDisable(GL_TEXTURE_2D);

	//Starting selection
	glColor3d(0, 1.0, 0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(startxBotLeft, startyBotLeft);
		glVertex2d(startxBotLeft, startyTopRight);
		glVertex2d(startxTopRight, startyTopRight);
		glVertex2d(startxTopRight, startyBotLeft);
	glEnd();

	//Ending Selection
	glColor3d(1.0, 0, 0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(endxBotLeft, endyBotLeft);
		glVertex2d(endxBotLeft, endyTopRight);
		glVertex2d(endxTopRight, endyTopRight);
		glVertex2d(endxTopRight, endyBotLeft);
	glEnd();
	//cout << xStart << " " << yStart;
	glFlush();
	glutSwapBuffers();
	glEnable(GL_TEXTURE_2D);
}


void display() {
    img_win->Draw();
	drawEnd();
	

    glutPostRedisplay();
}



//
// GLUT mouse button function
// 
void onMouseButton(int button, int state, int x, int y) {
   
	double mouseX, mouseY;
	double dx, dy;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseX = xScreenToWorld(x);
			mouseY = yScreenToWorld(y);
			dx = (world.r - world.l)/4;
			dy = (world.t - world.b)/4;
			world.l = mouseX - dx;
			world.r = mouseX + dx;
			world.b = mouseY - dy;
			world.t = mouseY + dy;
			initialize();
			display();
		}
	} 
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseX = xScreenToWorld(x);
			mouseY = yScreenToWorld(y);
			dx = (world.r - world.l);
			dy = (world.t - world.b);
			world.l = mouseX - dx;
			world.r = mouseX + dx;
			world.b = mouseY - dy;
			world.t = mouseY + dy;
			initialize();
			display();
		}
	}
}

//
// GLUT keypress function
// 
void onKeyPress(unsigned char key, int x, int y) {
	
    if ( key == 't' ) {      // request transformation from user
        img_win->getTopImage()->inputXform();
    }
	else if (key == 'a') {
		greenValue = 1.0;
		startxBotLeft = xScreenToWorld(x);
		startyBotLeft = yScreenToWorld(y);
		glutPostRedisplay();
	}
	else if (key == 'b') {
		greenValue = 1.0;
		startxTopRight = xScreenToWorld(x);
		startyTopRight = yScreenToWorld(y);
		glutPostRedisplay();
	}
	else if (key == 'c') {
		redValue = 1.0;
		endxBotLeft = xScreenToWorld(x);
		endyBotLeft = yScreenToWorld(y);
		glutPostRedisplay();
	}
	else if (key == 'd') {
		redValue = 1.0;
		endxTopRight = xScreenToWorld(x);
		endyTopRight = yScreenToWorld(y);
		glutPostRedisplay();
	}
    else if ( key == 'q' ) { // quit the program.
        delete img_win;
        if ( file_data )
            delete file_data;
        exit( 0 );
    }
	
}

//
// GLUT mouse motion function
// 
void mouse_motion( int x, int y ) {
    if ( left_mouse_down ) {
        img_win->getTopImage()->translate( x, (img_win->height() - 1 - y) );
    }
    glutPostRedisplay();
}


//
// GLUT resize function
// 
void resize(int w, int h) {
    img_win->Init( w, h, world);
}



int main(int argc, char *argv[])
{
	// TODO: code your application's behavior here.
	// create instance of an ImageWindow (global variable)
	img_win = new ImageWindow;

	// window size parameters
	int w = 256; int h = 256;

	// glut initialization functions:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(w,h);
	glutInitWindowPosition(100,100);
	glutCreateWindow("ImageViewer");

	// display, onMouseButton, mouse_motion, onKeyPress, and resize are functions defined above
	glutDisplayFunc(display);       
	glutMouseFunc(onMouseButton);
	glutPassiveMotionFunc( mouse_motion );
	glutKeyboardFunc(onKeyPress);
	glutReshapeFunc(resize);

	// Initializes window, includes opengl calls
	img_win->Init(w, h, world);
	for ( int i = 1; i < argc; i++ ) {
		// add images to the ImageWindow
		img_win->addImage( argv[i] );
	}

	// start glutMainLoop -- infinite loop
	glutMainLoop();
}

