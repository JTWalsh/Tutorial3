#include "OpenGLImage.h"
typedef struct {
	double l, r, b, t;
} Extent;

extern GLint viewport[4];
class ImageWindow {
public:
    ImageWindow();
    ~ImageWindow();
    void addImage(char *filename);
    void Init( int w, int h, Extent world);
    void Draw();
    OpenGLImage *findImageAt(int x, int y);
    void moveToTopImageAt(int x, int y);
    OpenGLImage *getTopImage();
    inline int width() { return width_; };
    inline int height() { return height_; };
private:
    OpenGLImage **imageList;
    int MAX_IMAGES;
    GLuint *imageTextures;
    int nextTexID;
    bool firstTime;
    int width_;
    int height_;
};