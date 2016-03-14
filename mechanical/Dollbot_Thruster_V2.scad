include <nutsnbolts/cyl_head_bolt.scad>;

$fn = 60;

piece_separation = 50;

base_side = 50;
base_height = 5;

pillar_side = 8;
pillar_height = 10; // Should be 30
pillar_base_offset = 1;
pillar_base_offset_side = 3;

pillar_screw_offset = 10;
pillar_screw_name="M3x30";
pillar_screw_details = _get_screw(pillar_screw_name);
pillar_screw_fam = _get_screw_fam(pillar_screw_name);

latch_depth = 2.20;
latch_side = 12 + latch_depth;
latch_fin_length = latch_side;
latch_fin_width = 2;

motor_width = 15.05;
motor_length = 20;
motor_height = 25;

motor_offset = 4;

motor_top_height = 4.5;
motor_top_overhang = 1.55;

motor_holder_radius = 9.85 / 2;
motor_holder_cutoff = 1;
motor_shaft_radius = 1;
motor_shaft_length = 8.4;
motor_shaft_cover_radius = motor_shaft_radius * 3;

thruster_jet_side = 10;
thruster_top_height = 2;
thruster_bottom_height = 5;
thruster_height = thruster_top_height + thruster_jet_side + thruster_bottom_height;
thruster_paddle_radius = 15;
thruster_hull_radius = 15;
thruster_pin_depth = 2;
thruster_pin_radius = 1.5;

rubber_rim_width = 0.6;
drilling_stencil_height = base_height;

module pillar(x,y) {
  translate([x-(pillar_side/2), y-(pillar_side/2), base_height]) {
    cube([pillar_side, pillar_side, pillar_height]);
  }
}

module pillar_screw_holes(x, y) {
  ds = _get_screw(pillar_screw_name);
  df = _get_screw_fam(pillar_screw_name);

  union() {
    //translate([x, y, 15.0001 + df[_NB_F_HEAD_HEIGHT]]) {
    translate([x, y, pillar_height + base_height/2 + df[_NB_F_HEAD_HEIGHT] - 0.4999]) {
      nut(df[_NB_F_DESC]);
      hole_through(df[_NB_F_DESC], l=ds[_NB_S_LENGTH] + 100, h=df[_NB_F_HEAD_HEIGHT]);
      translate([0,0,-piece_separation - thruster_height + df[_NB_F_HEAD_HEIGHT] - 1.1]) {
        rotate(a=180, v=[1,0,0]) {
        hole_through(df[_NB_F_DESC], l=ds[_NB_S_LENGTH] + 100, h=df[_NB_F_HEAD_HEIGHT]);
        }
      }
    };
  }
}

// Inside part of the thruster

// Base
module thruster(show_motor_gap) {
  difference() {
    union() {
      translate([0,0,base_height/2]) {
        intersection() {
          union() {
            cube([base_side, base_side, base_height], center=true);

            // Sides

            translate([-base_side/2, -base_side/2, -latch_depth - base_height/2]) {
              cube([latch_side - latch_depth, base_side, latch_depth]);
              translate([0, latch_side-latch_depth, 0]) {
                rotate(a=270, v=[0,0,1]) {
                  cube([latch_side - latch_depth, base_side, latch_depth]);
                }
              }
            }
          }
        }
      };

      // Improve the connection with the bottom of the Dollbot hull

      translate([-base_side/2 - latch_depth, -base_side/2 - latch_depth, -latch_depth]) {

        // Corner

        intersection() {
          translate([latch_side/2,latch_side/2, 0]) {
            cylinder(r=latch_side/2 + (latch_side/6), h = base_height + latch_depth);
          }

          cube([latch_side, latch_side, base_height + latch_depth]);
        };
      }



      // Pillars for screws

      pillar(base_side/2 - pillar_side/2 - pillar_base_offset, base_side/2 - pillar_side/2 - pillar_base_offset);
      pillar(-base_side/2 + pillar_base_offset + pillar_side/2, -pillar_base_offset_side * 2);
      pillar(-pillar_base_offset_side * 2, -base_side/2 + pillar_base_offset + pillar_side/2);

      // DC130 motor holder
      translate([motor_offset, motor_offset, 0]) {
        rotate(a=135, v=[0, 0, 1]) {
          difference() {
            translate([0, 0, (motor_height+base_height)/2]) {
              cube([motor_length*1.2, motor_width/1.10, motor_height + motor_top_height - base_height], center=true);
            };

          // Top of the motor holder indentation to keep motor from moving around

            translate([0,0,motor_height-motor_top_height/2]) {
              difference() {
                cylinder(r=motor_holder_radius, h=motor_top_height);

                translate([-motor_holder_radius, -motor_holder_radius, 0]) {
                  cube([motor_holder_radius*2, motor_holder_cutoff, motor_top_height]);
                }
              };
            }
          };
        }
      }

      translate([base_side/2,-base_side/2,-piece_separation]) {
        rotate(a=90, v=[0,0,1]) {
          external_thruster();
        }
      }
  }

    // Screw holes

    pillar_screw_holes(base_side/2 - pillar_side/2 - pillar_base_offset, base_side/2 - pillar_side/2 - pillar_base_offset);
    pillar_screw_holes(-base_side/2 + pillar_base_offset + pillar_side/2, -pillar_base_offset_side * 2);
    pillar_screw_holes(-pillar_base_offset_side * 2, -base_side/2 + pillar_base_offset + pillar_side/2);

    translate([-base_side/2 - latch_depth, -base_side/2 - latch_depth, 0]) {
      translate([latch_fin_width/2, -latch_fin_width/2, -latch_depth - 1]) {
        rotate(a=45, v = [0,0,1]) {
          cube([latch_fin_length, latch_fin_width, base_height + latch_depth + 2]);
        }
      }
    }

    // Motor holder block
    if(show_motor_gap) {
      translate([motor_offset, motor_offset, 0]) {

        rotate(a=135, v=[0, 0, 1]) {

          translate([-motor_length/2, -motor_width/2, -latch_depth]) {

            union() {
              intersection() {
                cube([motor_length, motor_width, motor_height+latch_depth]);
                translate([motor_length/2, motor_width/2,0]) {
                  cylinder(r=motor_length/2, h=motor_height+latch_depth);
                }
              };

              translate([(motor_length - motor_width)/2, motor_width, 0]) {
                cube([motor_width, motor_top_overhang, motor_height+latch_depth]);
              }
            }
          };
        }
      }

    } else {
      // Hole for drilling template instead of the motor holder block
      translate([motor_offset, motor_offset, -base_height/2]) {
        // cylinder(r = motor_shaft_cover_radius, h = 100);
        cylinder(r = motor_shaft_radius, h = 100);
      }

    }

  }

}


// Module: External thruster
module external_thruster() {

  difference() {

    union() {
      cube([base_side, base_side, thruster_height]);

      translate([latch_side,0,thruster_height]) {
        difference() {
          cube([base_side - latch_side, base_side - latch_side, latch_depth]);
          translate([0,base_side - (2*latch_side), 0]) {
            cube([latch_side, latch_side, latch_depth]);
          }
        }
      }
    }

    rubber_rim(pillar_base_offset + pillar_side/2, base_side/2 + pillar_side/2 + pillar_base_offset * 2, thruster_height);
    rubber_rim(base_side/2 - pillar_base_offset * 2 - pillar_side/2 , base_side - pillar_side/2 - pillar_base_offset, thruster_height);
    rubber_rim(base_side - pillar_side/2 - pillar_base_offset, pillar_side/2 + pillar_base_offset, thruster_height + latch_depth);

    translate([thruster_hull_radius-0.0001, base_side - thruster_hull_radius, 0]) {
      rotate(a=90, v=[0,0,1]) {
        difference() {
          cube([thruster_hull_radius, thruster_hull_radius, thruster_height]);
          cylinder(r=thruster_hull_radius, h=thruster_height);
        }
      }
    }

    translate([0,0,thruster_bottom_height]) {
      rotate(a=45, v=[0,0,1]) {
        cube([sqrt(2 * pow(base_side, 2)), thruster_jet_side, thruster_jet_side]);
      }
    }

    translate([base_side/2 + motor_offset, base_side/2 - motor_offset, thruster_bottom_height]) {
      cylinder(r=thruster_paddle_radius, h = thruster_top_height + thruster_jet_side + latch_depth);
      translate([0,0,-thruster_pin_depth]) {
        cylinder(r=thruster_pin_radius, h=thruster_pin_depth);
      }
    }
  }
}

module rubber_rim(x, y, z) {
  df = _get_screw_fam(pillar_screw_name);

  translate([x, y, z]) {
    rotate_extrude(convexity = 10)
    translate([df[_NB_F_OUTER_DIA] + rubber_rim_width * 2, 0, 0])
    circle(r = rubber_rim_width);
  }
}

module drilling_stencil() {
  intersection() {
    thruster(false);
    translate([-base_side, -base_side, -base_height/2]) {
      cube([base_side * 2, base_side * 2, drilling_stencil_height]);
    }
  }
}

thruster_pin_depth = 2;
thruster_pin_radius = 1.5;

turbine_radius = 3;
turbine_height = 13;
turbine_top_radius = thruster_pin_radius * 0.9;
turbine_wings = 2;
turbine_wing_height = turbine_height - 3.50;
turbine_wing_length = 14;
turbine_wing_width = 4;
turbine_bottom_offset = 0;

translate([40,40,40]) {
//  turbine();
}

module turbine() {
  // Center circle
  difference() {
    union() {
      cylinder(r=turbine_radius, h=turbine_height - 2 * thruster_pin_depth);
      // Top protuberance
      translate([0,0,turbine_height - (2 * thruster_pin_depth)]) {
        cylinder(r=turbine_top_radius, h = 2 * thruster_pin_depth);
        cylinder(r1=turbine_radius, r2=turbine_top_radius, h=thruster_pin_depth * 0.9);
      }

      // Rotate 6 times and do the sides
      for(angle =  [360/turbine_wings: 360/turbine_wings : 360]) {
          rotate(angle, [0, 0, 1]) {
            translate([0, -turbine_wing_width/2, turbine_bottom_offset]) {
              cube([turbine_wing_length - turbine_wing_width/2, turbine_wing_width, turbine_wing_height]);
              translate([turbine_wing_length - turbine_wing_width/2, turbine_wing_width/2,0]) {
                cylinder(r=turbine_wing_width/2, h=turbine_wing_height);
              }
          }
        }
      }
    }
  // holes

  cylinder(r=motor_shaft_radius * 1.1, h=motor_shaft_length - 2);
}
}

/* Actual code, create the thruster (which includes both internal and external components),
   the turbine and the drilling stencil */

thruster(true);

translate([0, 0, piece_separation * 1.5]) {
  turbine();
}

translate([0, 0, piece_separation]) {
  drilling_stencil();
}
