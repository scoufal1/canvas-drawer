#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "ppm_image.h"

using namespace std;

namespace agl
{
   struct point {
      int x;
      int y;
      ppm_pixel color;
   };
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
   class canvas
   {
   public:
      canvas(int w, int h);
      virtual ~canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      void drawLine(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor);
      void drawLineLow(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor);
      void drawLineHigh(int ax, int ay, ppm_pixel aColor, int bx, int by, ppm_pixel bColor);
      ppm_pixel interpolateColor(ppm_pixel aColor, ppm_pixel bColor, float t);

   private:
      ppm_image _canvas;
      PrimitiveType currentType;
      ppm_pixel currentColor;
      vector<point> vertices;
   };
}

#endif


