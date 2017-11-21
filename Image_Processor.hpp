// Image_processer.hpp

#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <vector>
#include "wx/image.h"


// Definitions of the routines
void wxImage2grayBuffer(const wxImage* img, int* buffer);
wxImage* grayBuffer2wxImage(const int* buffer, int width, int height);
void wxImage2colorBuffer(const wxImage* img, int* buffer);
wxImage* colorBuffer2wxImage(const int* buffer, int width, int height);
wxImage* LowPass(wxImage *pImage);
wxImage* HighPass(wxImage *pImage);	//lab1

wxImage* Nonlinear(wxImage *pImage, int type);	//new lab2

wxImage* Edge(wxImage *pImage);	//new lab3
wxImage* Binary(wxImage *pImage);

wxImage* binarize(wxImage *masterImage, int threshold);	//new lab4

wxImage* copy(wxImage *masterImage);

#endif
