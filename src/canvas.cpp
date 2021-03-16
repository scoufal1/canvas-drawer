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
   point a,b,c;
   if(currentType == LINES) {
      for (int i = 0; i < vertices.size()-1; i+=2) 
      {
         a = vertices[i];
         b = vertices[i+1];
         drawLine(a,b);
      }
   } else if(currentType == TRIANGLES) {
      for (int i = 0; i < vertices.size()-2; i+=3) 
      {
         a = vertices[i];
         b = vertices[i+1];
         c = vertices[i+2];
         barycentricFill(a,b,c);
      }
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

void canvas::drawLine(point a, point b) 
{
   int w = b.x-a.x;
   int h = b.y-a.y;
   //if width is greater than height
   if(abs(w)>abs(h)) {
      if(a.x < b.x) {
         drawLineLow(a,b);
      } else {
         drawLineLow(b,a);
      }
   }
   //if height is greater than width
   else {
      if(a.y < b.y) {
         drawLineHigh(a,b);
      } else {
         drawLineHigh(b,a);
      }
   }
}

void canvas::drawLineLow(point a, point b) 
{
   int y = a.y;
   int w = b.x-a.x;
   int h = b.y-a.y;
   int inc = 1;
   float t;
   ppm_pixel color;

   if(h < 0) {
      inc = -1;
      h = -h;
   }
   int f = 2*h - w;

   for(int x = a.x; x <= b.x; x++) {

      t = (x-a.x)/(float)(b.x-a.x);
      color = interpolateColorLine(a.color, b.color, t);
      _canvas.set(y,x,color);

      if(f > 0) {
         y += inc;
         f += 2*(h-w);
      } else {
         f += 2*h;
      }
   }
}

void canvas::drawLineHigh(point a, point b) 
{
   int x = a.x;
   int w = b.x - a.x;
   int h = b.y - a.y;
   int inc = 1;
   float t;
   ppm_pixel color;

   if(w < 0) {
      inc = -1;
      w = -w;
   }
   int f = 2*w - h;

   for (int y = a.y; y <= b.y; y++){

      t = (y-a.y)/(float)(b.y-a.y);
      color = interpolateColorLine(a.color, b.color, t);
      _canvas.set(y,x,color);
      
      if(f > 0) { 
         x += inc;
         f += 2*(w-h);
      } else {
         f += 2*w;
      }
   }
}

ppm_pixel canvas::interpolateColorLine(ppm_pixel aColor, ppm_pixel bColor, float t)
{
   unsigned char r = (unsigned char)(aColor.r*(1-t)+bColor.r*t);
   unsigned char g = (unsigned char)(aColor.g*(1-t)+bColor.g*t);
   unsigned char b = (unsigned char)(aColor.b*(1-t)+bColor.b*t);
   return ppm_pixel{r,g,b};
}

ppm_pixel canvas::interpolateColorTriangle(ppm_pixel aColor, ppm_pixel bColor, ppm_pixel cColor, 
   float alpha, float beta, float gamma)
{
   unsigned char r = (unsigned char)(aColor.r*alpha + bColor.r*beta + cColor.r*gamma);
   unsigned char g = (unsigned char)(aColor.g*alpha + bColor.g*beta + cColor.g*gamma);
   unsigned char b = (unsigned char)(aColor.b*alpha + bColor.b*beta + cColor.b*gamma);

   return ppm_pixel{r,g,b};
}

void canvas::barycentricFill(point a, point b, point c) 
{
   int xmin = minimum(a.x, b.x, c.x);
   int xmax = maximum(a.x, b.x, c.x);
   int ymin = minimum(a.y, b.y, c.y);
   int ymax = maximum(a.y, b.y, c.y);

   int f_alpha = implicitLine(b,c,a);
   int f_beta = implicitLine(a,c,b);
   int f_gamma = implicitLine(a,b,c);
   point p;
   ppm_pixel color;

   float alpha, beta, gamma;
   for(int row = ymin; row < ymax; row++) {
      for(int col = xmin; col < xmax; col++) {
         
         p = {col,row,ppm_pixel{0,0,0}};
         alpha = implicitLine(b,c,p)/(float)f_alpha;
         beta = implicitLine(a,c,p)/(float)f_beta;
         gamma = implicitLine(a,b,p)/(float)f_gamma;

         //if pixel is inside triangle
         if(alpha >= 0 && beta >= 0 && gamma >= 0) {
            point offscreenPoint = {-1,-1,ppm_pixel{0,0,0}};
            //handles adjacent edges
            if(alpha > 0 || f_alpha*implicitLine(b,c,offscreenPoint) > 0 
            || f_alpha*implicitLine(a,c,offscreenPoint) > 0 || f_alpha*implicitLine(a,b,offscreenPoint) > 0) {
               color = interpolateColorTriangle(a.color, b.color, c.color, alpha, beta, gamma);
               _canvas.set(row,col,color);
            }
         }
      }
   }
}

int canvas::minimum(int a, int b, int c) 
{
   int min = ( a > b )? b : a;
   return ( ( min > c ) ? c : min );
}

int canvas::maximum(int a, int b, int c) 
{
   int max = ( a < b )? b : a;
   return ( ( max < c ) ? c : max );
}

int canvas::implicitLine(point p1, point p2, point p) 
{
   return((p2.y-p1.y)*(p.x-p1.x)-(p2.x-p1.x)*(p.y-p1.y));
}