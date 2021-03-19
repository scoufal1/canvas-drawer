#include "canvas.h"
#include <cassert>
#include <math.h> 

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

void canvas::noFill()
{
   fill = false;
}

void canvas::fillShape() 
{
   fill = true;
}

void canvas::noBlend()
{
   blend = NONE;
}

void canvas::setBlend(BlendType blendType)
{
   blend = blendType;
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
         if(fill) {
            barycentricFill(a,b,c);
         } else {
            drawLine(a,b);
            drawLine(b,c);
            drawLine(c,a);
         }
      }
   } else if(currentType == POINTS) {
      for(int i = 0; i < vertices.size(); i++) {
         a = vertices[i];
         _canvas.set(a.y, a.x, a.color);
      } 
   } else if(currentType == UNDEFINED) {
         cout << "Need to define type" << endl;
   }
   vertices.clear();
   currentType = UNDEFINED;
}

void canvas::vertex(int x, int y)
{
   if(x < 0)
      x = 0;
   if(x >= _canvas.width())
      x = _canvas.width()-1;
   if(y < 0)
      y = 0;
   if(y >= _canvas.height())
      y = _canvas.height()-1;
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

void canvas::backgroundGradient(unsigned char r1, unsigned char g1, unsigned char b1,
         unsigned char r2, unsigned char g2, unsigned char b2) 
{
   float t;
   unsigned char r, g, b;
   
   for(int row = 0; row < _canvas.height(); row++) {
      for(int col = 0; col < _canvas.width(); col++) {
         t = (row)/(float)(_canvas.height());
         r = (unsigned char)(r1*(1-t)+r2*t);
         g = (unsigned char)(g1*(1-t)+g2*t);
         b = (unsigned char)(b1*(1-t)+b2*t);
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
            color = interpolateColorTriangle(a.color, b.color, c.color, alpha, beta, gamma);
            if(blend != NONE) {
               color = blendLayers(color, row, col);
            }
            _canvas.set(row,col,color);
            
            //handles adjacent edges, but doesn't work correctly so for now just double draw edges
            /*
            float offscreen1 = (c.y - b.y) * (-1.5f - b.x) - (c.x - b.x) * (-1.1f - b.y);
            float offscreen2 = (c.y - a.y) * (-1.5f - a.x) - (c.x - a.x) * (-1.1f - a.y);
            float offscreen3 = (b.y - a.y) * (-1.5f - a.x) - (b.x - a.x) * (-1.1f - a.y);
            
            bool drawAlpha = alpha > 0 || f_alpha * offscreen1 > 0;
            bool drawBeta = beta > 0 || f_beta * offscreen2 > 0;
            bool drawGamma = gamma > 0 || f_gamma * offscreen3 > 0;
            
            if(drawAlpha && drawBeta && drawGamma) {
               color = interpolateColorTriangle(a.color, b.color, c.color, alpha, beta, gamma);
               _canvas.set(row,col,color);
            }
            */
         }
      }
   }
}

ppm_pixel canvas::blendLayers(ppm_pixel color, int row, int col)
{
   ppm_pixel color2 = _canvas.get(row,col);
   int r = color.r;
   int g = color.g;
   int b = color.b;

   if(blend == MULTIPLY) {
      r = 255*((color.r/255.0)*(color2.r/255.0));       
      g = 255*((color.g/255.0)*(color2.g/255.0));
      b = 255*((color.b/255.0)*(color2.b/255.0));
   } else if(blend == ADD) {
      r = 255*((color.r/255.0)+(color2.r/255.0));  
      if(r > 255) r = 255;     
      g = 255*((color.g/255.0)+(color2.g/255.0));
      if(g > 255) g = 255;
      b = 255*((color.b/255.0)+(color2.b/255.0));
      if(b > 255) b = 255;
   } else if(blend == DIFFERENCE) {
      r = 255*((color.r/255.0)-(color2.r/255.0));  
      if(r < 0) r = -r;     
      g = 255*((color.g/255.0)-(color2.g/255.0));
      if(g < 0) g = -g;
      b = 255*((color.b/255.0)-(color2.b/255.0));
      if(b < 0) b = -b;
      if(b > 255) {
         cout << b << endl;
      }
   }
   return {(unsigned char)r, (unsigned char)g, (unsigned char)b};
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

void canvas::drawCircle(int x, int y, int rad)
{
   int numSlices = 200;
   float deltaTheta = 2*M_PI/(float)numSlices;
   float theta1, theta2;

   if(fill) {
      begin(TRIANGLES);
   } else {
      begin(LINES);
   }
   for(int i = 0; i < numSlices; i++) {
      theta1 = i*deltaTheta;
      theta2 = (i+1)*deltaTheta;
      vertex(x + (int)(rad*cos(theta1)), y + (int)(rad*sin(theta1)));
      vertex(x + (int)(rad*cos(theta2)), y + (int)(rad*sin(theta2)));
      if(fill) {
         vertex(x,y);  
      }                                   
   }
   end();
}

void canvas::drawRectangle(int x, int y, int wid, int hgt) 
{
   int leftX = x-(wid/2);
   int rightX = x+(wid/2);
   int upY = y-(hgt/2);
   int downY = y+(hgt/2);
   if(fill) {
      begin(TRIANGLES);
      vertex(leftX, upY);
      vertex(rightX, upY);
      vertex(rightX, downY);
      vertex(leftX, upY);
      vertex(leftX, downY);
      vertex(rightX, downY);
   } else {
      begin(LINES);
      vertex(leftX, upY);
      vertex(rightX, upY);
      vertex(rightX, upY);
      vertex(rightX, downY);
      vertex(rightX, downY);
      vertex(leftX, downY);
      vertex(leftX, downY);
      vertex(leftX, upY);
   }
   end();
}
