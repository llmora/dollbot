include <nutsnbolts/cyl_head_bolt.scad>;

// $fn = 400;

base_side = 50;
base_height = 5;

pillar_side = 8;
pillar_height = 10; // Should be 30
pillar_base_offset = 3;

pillar_screw_offset = 10;
pillar_screw_name="M3x25";
pillar_screw_details = _get_screw(pillar_screw_name);
pillar_screw_fam = _get_screw_fam(pillar_screw_name);

latch_depth = 2.20;
latch_side = 16 + latch_depth;
latch_fin_length = latch_side;
latch_fin_width = 2;

motor_width = 15.05;
motor_length = 20;
motor_height = 25;

motor_offset = 5;

motor_top_height = 4.5;
motor_top_overhang = 1.55;

motor_holder_radius = 9.85 / 2;
motor_holder_cutoff = 1;

module pillar(x,y) {
  translate([x-(pillar_side/2), y-(pillar_side/2), base_height]) {
    cube([pillar_side, pillar_side, pillar_height]);
  }
}

module pillar_screw_holes(x, y) {
  ds = _get_screw(pillar_screw_name);
  df = _get_screw_fam(pillar_screw_name);

  union() {
    translate([x, y, 15]) {
      hole_through(df[_NB_F_DESC], l=ds[_NB_S_LENGTH], h=df[_NB_F_HEAD_HEIGHT]);
    };
  }
}

// Inside part of the thruster

// Base

difference() {
  union() {
    translate([0,0,base_height/2]) {
      intersection() {
        translate([0,0,-latch_depth/2]) {
          cylinder(r=base_side/2 + (base_side/6), h=base_height + latch_depth, center=true);
        }
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
    pillar(-base_side/2 + pillar_base_offset + pillar_side/2, 0);
    pillar(0, -base_side/2 + pillar_base_offset + pillar_side/2);

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
}

  // Screw holes

  pillar_screw_holes(base_side/2 - pillar_side/2 - pillar_base_offset, base_side/2 - pillar_side/2 - pillar_base_offset);
  pillar_screw_holes(-base_side/2 + pillar_base_offset + pillar_side/2, 0);
  pillar_screw_holes(0, -base_side/2 + pillar_base_offset + pillar_side/2);

  // Fin
  // TODO: Align properly, we turn not in the center but on the side of the fin

  translate([-base_side/2 - latch_depth, -base_side/2 - latch_depth, 0]) {
    translate([latch_fin_width/2, -latch_fin_width/2, -latch_depth - 1]) {
      rotate(a=45, v = [0,0,1]) {
        cube([latch_fin_length, latch_fin_width, base_height + latch_depth + 2]); // TODO: Length of fin is inaccurate
      }
    }
  }

  // Motor holder block
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
}
