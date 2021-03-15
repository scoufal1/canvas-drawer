#include "canvas.h"
#include <cassert>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h)
{
}

canvas::~canvas()
{
}

void canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void canvas::begin(PrimitiveType type)
{
   currentType = type;
}

void canvas::end()
{
   int ax, ay, bx, by;
   ppm_pixel aColor, bColor;

   for (int i = 0; i < vertices.size()-1; i+=2) 
   {
      ax = vertices[i].x;
      ay = vertices[i].y;
      aColor = vertices[i].color;
      bx = vertices[i+1].x;
      by = vertices[i+1].y;
      bColor = vertices[i+1].color;
      drawLine(ax, ay, aColor, bx, by, bColor);
   }
    vertices.clear();
}

void canvas::vertex(int x, int y)
{
   vertices.push_back(point{x,y,currentColor});
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   currentColor = ppm_pixel{r,g,b};
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   for(int row = 0; row < _canvas.height(); row++) {
      for(int col = 0; col < _canvas.width(); col++) {
      _canvas.set(row, col, ppm_pixel{r,g,b});
      }
   }
}

void canvas::drawLine(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor) {
   int w = bx-ax;
   int h = by-ay;
   if(abs(w)>abs(h)) {
      if(ax < bx) {
         drawLineLow(ax, ay, aColor, bx, by, bColor);
      } else {
         drawLineLow(bx, by, bColor, ax, ay, aColor);
      }
   }
   else {
      if(ay < by) {
         drawLineHigh(ax, ay, aColor, bx, by, bColor);
      } else {
         drawLineHigh(bx, by, bColor, ax, ay, aColor);
      }
   }
}

void canvas::drawLineLow(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor) {
   int y = ay;
   int w = bx-ax;
   int h = by-ay;
   int inc = 1;
   float t;
   ppm_pixel color;
   if(h < 0) {
      inc = -1;
      h = -h;
   }
   int f = 2*h - w;
   for(int x = ax; x <= bx; x++) {
      t = (x-ax)/(float)(bx-ax);
      color = interpolateColor(aColor, bColor, t);
      _canvas.set(y,x,color);
      if(f > 0) {
         y += inc;
         f += 2*(h-w);
      } else {
         f += 2*h;
      }
   }
}

void canvas::drawLineHigh(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor) {
   int x = ax;
   int w = bx - ax;
   int h = by - ay;
   int inc = 1;
   float t;
   ppm_pixel color;
   if(w < 0) {
      inc = -1;
      w = -w;
   }
   int f = 2*w - h;
   for (int y = ay; y <= by; y++){
      t = (y-ay)/(float)(by-ay);
      color = interpolateColor(aColor, bColor, t);
      _canvas.set(y,x,color);
      if(f > 0) { 
         x += inc;
         f += 2*(w-h);
      } else {
         f += 2*w;
      }
   }
}

ppm_pixel canvas::interpolateColor(ppm_pixel aColor, ppm_pixel bColor, float t)
{
   unsigned char r = (unsigned char)(aColor.r*(1-t)+bColor.r*t);
   unsigned char g = (unsigned char)(aColor.g*(1-t)+bColor.g*t);
   unsigned char b = (unsigned char)(aColor.b*(1-t)+bColor.b*t);
   return ppm_pixel{r,g,b};
}
