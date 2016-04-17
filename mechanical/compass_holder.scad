/*
  HMC5883L Compass holder for Dollbot (http://splashbots.blogspot.com/)

  Small plastic piece to hold the 3-axis compass in place in the dollbot hull.
  
  Recommended 20% in-fill with no supports

*/

compass_length = 18.60;
compass_width = 18;
compass_height= 1.65;
compass_bottom_clearance = 3;
compass_top_clearance = 2;

base_height = 3;

holder_wall_thickness = 2;
holder_length = compass_length + holder_wall_thickness + holder_wall_thickness;
holder_width = compass_width + holder_wall_thickness + holder_wall_thickness;
holder_height = compass_height + base_height + compass_bottom_clearance + compass_top_clearance;

difference() {
  cube([holder_length, holder_width, holder_height]);

  translate([2 * holder_wall_thickness, 0, base_height]) {
    cube([compass_length - 2 - 1, compass_width + holder_wall_thickness + holder_wall_thickness, compass_height + compass_bottom_clearance + compass_top_clearance]);
  }
  
  translate([holder_wall_thickness, holder_wall_thickness, base_height + compass_bottom_clearance]) {
      cube([compass_length, compass_width, compass_height]);
  }
}
