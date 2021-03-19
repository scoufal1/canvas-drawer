# canvas-drawer

Implements a simple drawing api

TODO: Add a representative image for your project here

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives

```
PrimitiveType POINTS
```

![points](https://user-images.githubusercontent.com/48161551/111716999-9040be00-882d-11eb-9a37-af3eff1d3959.png)

```
void drawCircle(int x, int y, int rad);
```
![filled-circle](https://user-images.githubusercontent.com/48161551/111717363-486e6680-882e-11eb-9e83-44030f7528d6.png)

```
void drawRectangle(int x, int y, int wid, int hgt);
```

![filled-rect](https://user-images.githubusercontent.com/48161551/111717451-6d62d980-882e-11eb-9c34-1a22254d587c.png)

```
void noFill();
```
![outline-rect](https://user-images.githubusercontent.com/48161551/111717544-9daa7800-882e-11eb-9bb2-665a4452df93.png)

![outlined-circle](https://user-images.githubusercontent.com/48161551/111717553-a00cd200-882e-11eb-8a3a-609de09c0c02.png)

```
void setBlend(BlendType blendType);
```
Multiply:

![blend-multiply](https://user-images.githubusercontent.com/48161551/111717634-d2b6ca80-882e-11eb-9d46-86f768af38cf.png)

Add:

![blend-add](https://user-images.githubusercontent.com/48161551/111717643-d9ddd880-882e-11eb-82cb-3b222a9ab1fb.png)

Difference:

![blend-difference](https://user-images.githubusercontent.com/48161551/111717653-e19d7d00-882e-11eb-931f-f296cb9fed50.png)

```
void backgroundGradient(unsigned char r1, unsigned char g1, unsigned char b1, 
                        unsigned char r2, unsigned char g2, unsigned char b2);
```
![background-gradient](https://user-images.githubusercontent.com/48161551/111717822-3ccf6f80-882f-11eb-80fa-40a76fdc0ae0.png)


## Results

Unique image created using drawCircle, multiply blend mode, and background gradient.

![unique-image](https://user-images.githubusercontent.com/48161551/111717901-5d97c500-882f-11eb-8456-ee12dfb03823.png)
