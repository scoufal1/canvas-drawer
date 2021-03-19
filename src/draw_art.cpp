#include <iostream>
#include <random>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   int wid = 100;
   int hgt = 100;

   ppm_pixel green = {130,190,127};
   ppm_pixel lightBlue = {57,146,177};
   ppm_pixel lightPurple = {159,127,177};
   ppm_pixel darkBlue = {15,100,137};
   ppm_pixel darkPurple = {80,66,132};
   ppm_pixel* palette = new ppm_pixel[5];
   palette[0] = green;
   palette[1] = lightBlue;
   palette[2] = lightPurple;
   palette[3] = darkBlue;
   palette[4] = darkPurple;

   canvas drawer(wid, hgt);

   //point primitive
   drawer.background(0,0,0);
   drawer.begin(POINTS);
   //to generate random colors
   random_device rd; 
   mt19937 gen(rd()); // seed t
   uniform_int_distribution<> distr(0, 255); // define range
   int r,g,b;
   int c = 0;
   for(int j = 10; j < wid; j+=10) {
      for(int i = 10; i < hgt; i+=10) {
         r = distr(gen);
         g = distr(gen);
         b = distr(gen);
         //drawer.color(r, g, b);
         drawer.color(palette[c].r, palette[c].g, palette[c].b);
         drawer.vertex(j,i);
         c++;
         if(c > 4) c = 0;
      }
   }
   drawer.end();
   drawer.save("points.png");

   //filled circle
   drawer.background(0,0,0);
   drawer.color(green.r, green.g, green.b);
   drawer.drawCircle(wid/2, hgt/2, hgt/3);
   drawer.save("filled-circle.png");

   //outlined circle
   drawer.background(0,0,0);
   drawer.noFill();
   drawer.color(lightBlue.r, lightBlue.g, lightBlue.b);
   drawer.drawCircle(wid/2, hgt/2, hgt/3);
   drawer.save("outlined-circle.png");

   //filled rectangle
   drawer.background(0,0,0);
   drawer.fillShape();
   drawer.color(lightPurple.r, lightPurple.g, lightPurple.b);
   drawer.drawRectangle(wid/2, hgt/2, wid/2, hgt/3);
   drawer.save("filled-rect.png");

   //filled rectangle
   drawer.background(0,0,0);
   drawer.noFill();
   drawer.color(darkBlue.r, darkBlue.g, darkBlue.b);
   drawer.drawRectangle(wid/2, hgt/2, wid/2, hgt/3);
   drawer.save("outline-rect.png");

   //blend

   //multiply
   drawer.background(green.r, green.g, green.b);
   drawer.fillShape();
   drawer.color(lightBlue.r, lightBlue.g, lightBlue.b);
   drawer.setBlend(MULTIPLY);
   //drawer.color(lightPurple.r, lightPurple.g, lightPurple.b);
   drawer.drawRectangle(wid/2, hgt/2, hgt/3, wid/2);
   drawer.drawRectangle(wid/2, hgt/2, wid/2, hgt/3);
   drawer.save("blend-multiply.png");

   //add
   drawer.background(darkPurple.r, darkPurple.g, darkPurple.b);
   drawer.color(lightBlue.r, lightBlue.g, lightBlue.b);
   drawer.setBlend(ADD);
   drawer.color(lightPurple.r, lightPurple.g, lightPurple.b);
   drawer.drawCircle(wid/2, hgt/3, hgt/4);
   drawer.drawCircle(wid/2, 2*hgt/3, hgt/4);
   drawer.save("blend-add.png");
   
   //difference
   //drawer.background(darkBlue.r, darkBlue.g, darkBlue.b);
   drawer.background(lightBlue.r, lightBlue.g, lightBlue.b);
   drawer.color(darkPurple.r, darkPurple.g, darkPurple.b);
   drawer.setBlend(DIFFERENCE);
   //drawer.color(green.r, green.g, green.b);
   drawer.drawCircle(wid/2, hgt/3, hgt/4);
   drawer.drawRectangle(wid/2, 2*hgt/3, wid/2, hgt/3);
   drawer.save("blend-difference.png");

   //background gradient
   drawer.backgroundGradient(green.r, green.g, green.b,
   lightBlue.r, lightBlue.g, lightBlue.b);
   drawer.save("background-gradient.png");

   wid = 640;
   hgt = 380;

   canvas drawer2(wid, hgt);

   drawer2.backgroundGradient(darkBlue.r, darkBlue.g, darkBlue.b,
   lightPurple.r, lightPurple.g, lightPurple.b);

   c = 0; //color index
   int numCircles = 20;

   uniform_int_distribution<> width(0, wid); // define range
   uniform_int_distribution<> height(0, hgt); // define range
   uniform_int_distribution<> radius(30, 70); // define range
   
   int x, y, rad;

   for(int i = 0; i < numCircles; i++) {

      if(i % 2 == 0) drawer2.noBlend();
      if(i % 3 == 0) drawer2.setBlend(MULTIPLY);

      drawer2.color(palette[c].r, palette[c].g, palette[c].b);

      x = width(gen);
      y = height(gen);
      rad = radius(gen);
      drawer2.drawCircle(x, y, rad);
      c++;
      if(c > 4) c = 0;
   }

   drawer2.save("art.png");
   delete[] palette;
}

