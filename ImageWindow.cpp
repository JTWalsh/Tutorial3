#include "ImageWindow.h"
//
// Constructor
//
ImageWindow::ImageWindow() {
    MAX_IMAGES = 32;
    firstTime = true;

    imageList = new OpenGLImage*[ MAX_IMAGES ];
    imageTextures = new GLuint[ MAX_IMAGES ];
    nextTexID = 0;
}

// Destructor
ImageWindow::~ImageWindow() {
    for ( int i = 0; i < nextTexID; i++ )
        delete imageList[i];
    delete [] imageList;
    delete [] imageTextures;
}

//
// Adds an image to the image list.
//
void ImageWindow::addImage(char *filename) {
    imageList[ nextTexID ] = new OpenGLImage(filename, imageTextures[nextTexID]);
    nextTexID += 1;
}

//
// GL initialization
// called at start of program and also on resize.
//
void ImageWindow::Init( int w, int h, Extent world) {
    width_ = w;
    height_ = h;

    glViewport(0, 0, w, h);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world.l, world.r, world.b, world.t);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, viewport);

    if (firstTime) {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(MAX_IMAGES, imageTextures);
        firstTime = false;
    }
}

//
// Draws all the images in the image list, starting with first, the last image will be on top.
//
void ImageWindow::Draw() {
    glClearColor(0, 0, 0, 1);
    glClear( GL_COLOR_BUFFER_BIT );
    for ( int i = 0; i < nextTexID; i++ )  {
        imageList[i]->draw( );
    }
  //  glFlush();
  //  glutSwapBuffers();
}

//
// This doesn't work after the image is scaled, rotated, or sheared!
// You must fix this as part of the assignment.
//
OpenGLImage *ImageWindow::findImageAt(int x, int y) {
    OpenGLImage *rval = NULL;
    for ( int i = 0; i < nextTexID; i++ )  {
        if ((x >= imageList[i]->tx()) && (x < imageList[i]->tx() + imageList[i]->width()) &&
            (y >= imageList[i]->ty()) && (y < imageList[i]->ty() + imageList[i]->height()))
            rval = imageList[i];
    }
    return rval;
}

//
// Finds the image under the mouse and moves it to the end of the imageList
//
void ImageWindow::moveToTopImageAt(int x, int y) {
    OpenGLImage *target = findImageAt(x,y);
    if (target != NULL) {
        for ( int i = 0; i < nextTexID; i++ ) {
            if ( imageList[i] == target ) {
                for ( int j = i; j < nextTexID - 1; j++ ) {
                    imageList[j] = imageList[j+1];
                }
                imageList[nextTexID-1] = target;
                break;
            }
        }
    }
}

//
// Returns the top image -- the image at the end of the imageList
//
OpenGLImage *ImageWindow::getTopImage() {
    OpenGLImage *rval = NULL;
    return imageList[ nextTexID - 1];
}

