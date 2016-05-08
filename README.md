# dollbot
Water surface autonomous vehicle with holonomic propulsion

More information on http://splashbots.blogspot.com/

Mechanical design
-----------------
The "mechanical" directory includes the CAD design files for the pieces used in dollbot (as well as the STL renderings, ready to 3D print).

The designs are parametric, so you can adjust and play with different measurements - just use OpenSCAD to visualise the results and export to STL. If you plan to work with the SCAD files you will need to install the ["nutsnbolts"](https://github.com/JohK/nutsnbolts)
OpenSCAD library. You can do that easily using git as the dollbot project already include the appropriate references to pull any supporting libraries:
```
$ git submodule init
$ git submodule update
```
