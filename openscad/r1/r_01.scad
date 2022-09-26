/**
 * Martin Egli
 * 2022-09-14
 */

use <parts.scad>
use <screws.scad>
use <mechanics_parts.scad>
use <electronics_parts.scad>

//origin();
//mpu650pcb();

module lego_cut(len = 10) {
    hull() {
        translate([-1.15, -1.0, 0]) 
        cube([2.3, 2.0, len]);
        translate([-1.0, -1.15, 0]) 
        cube([2.0, 2.3, len]);
    }
    hull() {
        translate([-2.5, -0.8, 0]) 
        cube([5.0, 1.6, len]);
        translate([-2.3, -1.0, 0]) 
        cube([4.6, 2.0, len]);
    }
    hull() {
        translate([-1.0, -2.3, 0]) 
        cube([2.0, 4.6, len]);
        translate([-0.8, -2.5, 0]) 
        cube([1.6, 5.0, len]);
    }
}

module lego_wheel_holder(loc_res = 32) {
    difference() {
        union() {
            hull() {
                translate([0, 0, 0]) 
                cylinder(d = 24, 2, $fn = loc_res);
                translate([0, 0, 0]) 
                cylinder(d = 10, 5.5, $fn = loc_res);
            }
            translate([0, 0, 0]) 
            cylinder(d = 7, 9, $fn = loc_res);
        }

        translate([0, 0, -4]) 
        lego_cut(20);

        translate([0, 0, 3]) 
        flycat2204_rotor_cut_m2x6mm();
    }
}
module motorA(loc_res = 32) {
    translate([0, 0, 14]) 
    lego_wheel_holder();
    flycat2204();
}

module motor_leg_v1_1(show = 1, loc_res = 32) {
    if(show) {
        translate([0, 0, 6]) 
        motorA();

        *color("DarkGray")
        translate([0, 0, 16]) 
        difference() {
            cylinder(d = 57, h = 26, $fn = loc_res);
            translate([0, 0, -1]) 
            cylinder(d = 32, h = 28, $fn = loc_res);
        }
    }
    

    difference() {
        union() {
            hull() {
                translate([32, 0, 0]) 
                cylinder(d = 28, h = 6, $fn = loc_res);
                translate([0, 0, 0]) 
                cylinder(d = 28, h = 6, $fn = loc_res);
            }
            translate([22, -14, 0])
            cube([20, 28, 36]);
        }
        translate([0, 0, -1]) 
        cylinder(d = 5, h = 8, $fn = loc_res);
        translate([0, 0, 3])
        rotate([180, 0, 0])
        flycat2204_rotor_cut_m2x6mm();

        // cable channel
        hull() {
            translate([11, -4, -1]) 
            cylinder(d = 4, h = 8, $fn = loc_res);
            translate([11, +4, -1]) 
            cylinder(d = 4, h = 8, $fn = loc_res);
        }
        hull() {
            translate([11, -3, 0]) 
            rotate([0, 90, 0])
            cylinder(d = 6, h = 40, $fn = loc_res);
            translate([11, +3, 0]) 
            rotate([0, 90, 0])
            cylinder(d = 6, h = 40, $fn = loc_res);
        }

        // M3 holes
        translate([18, -8, -1]) 
        cylinder(d = 3, h = 8, $fn = loc_res);
        translate([18, +8, -1]) 
        cylinder(d = 3, h = 8, $fn = loc_res);
        translate([36, 0, -1]) {
            cylinder(d = 3, h = 38, $fn = loc_res);
            translate([0, 0, 13]) 
            cylinder(d = 6.5, h = 38, $fn = loc_res);
            hull() {
                translate([0, 0, 8]) 
                cylinder(d = 6.5, h = 3, $fn = 6);
                translate([4, 0, 8]) 
                cylinder(d = 6.5, h = 3, $fn = 6);
            }
        }

        // M3 for upper part
        translate([50, -8, 32])
        rotate([0, -90, 0]) {
            cylinder(d = 3, h = 38, $fn = loc_res);
            translate([0, 0, 13]) 
            cylinder(d = 6.5, h = 38, $fn = loc_res);
        }
        translate([50, +8, 32])
        rotate([0, -90, 0]) {
            cylinder(d = 3, h = 38, $fn = loc_res);
            translate([0, 0, 13]) 
            cylinder(d = 6.5, h = 38, $fn = loc_res);
        }
        
        // space for wheel
        hull() {
            translate([0, 0, 11])
            cylinder(d = 64, h = 40, $fn = loc_res);
            translate([0, 0, 6])
            cylinder(d = 54, h = 40, $fn = loc_res);
        }
        translate([22+20, -15, -1])
        cube([20, 50, 32]);

        // see into
        //translate([0, 0, -5]) cube([50, 50, 50]);
    }

    *translate([50, 20, 51/2])
    rotate([90, 0, 0])
    cylinder(d = 3.2, h = 60+10, $fn = loc_res);
}

module side_part1_v1_0(show = 1, loc_res = 32) {
    difference() {
        union() {
            translate([0, 0, 0])
            cube([80, 28, 6]);
            translate([0, 28-2.5, -10])
            cube([80, 2.5, 16]);
        }

        translate([-2, 6, 2])
        rotate([0, 90, 0])
        cylinder(d = 3.2, h = 80+28, $fn = loc_res);
        hull() {
            translate([4, 6, 2])
            rotate([0, 90, 0]) {
                cylinder(d = 6.5, h = 2.5, $fn = 6);
                translate([4 ,0, 0])
                cylinder(d = 6.5, h = 2.5, $fn = 6);
            }
        }
        translate([-2, 28-6, 2])
        rotate([0, 90, 0])
        cylinder(d = 3.2, h = 80+28, $fn = loc_res);
        hull() {
            translate([4, 28-6, 2])
            rotate([0, 90, 0]) {
                cylinder(d = 6.5, h = 2.5, $fn = 6);
                translate([4 ,0, 0])
                cylinder(d = 6.5, h = 2.5, $fn = 6);
            }
        }
        hull() {
            translate([80-2.5-4, 6, 2])
            rotate([0, 90, 0]) {
                cylinder(d = 6.5, h = 2.5, $fn = 6);
                translate([4 ,0, 0])
                cylinder(d = 6.5, h = 2.5, $fn = 6);
            }
        }
        hull() {
            translate([80-2.5-4, 28-6, 2])
            rotate([0, 90, 0]) {
                cylinder(d = 6.5, h = 2.5, $fn = 6);
                translate([4 ,0, 0])
                cylinder(d = 6.5, h = 2.5, $fn = 6);
            }
        }

        // gimbal pcb mount
        hull() {
            translate([10, 32, -4.5])
            rotate([90, 0, 0])
            cylinder(d = 3.2, h = 10, $fn = loc_res);
            translate([80-6, 32, -4.5])
            rotate([90, 0, 0])
            cylinder(d = 3.2, h = 10, $fn = loc_res);
        }

        // stand mount
        translate([40, 14, -2])
        rotate([0, 0, 0])
        cylinder(d = 5.0, h = 10, $fn = loc_res);

        translate([30, 14, -2])
        rotate([0, 0, 0])
        cylinder(d = 3.2, h = 10, $fn = loc_res);
    }
}

module top_part_v1_0(show = 1, loc_res = 32) {
    difference() {
        translate([0, 0, 0])
        cube([28, 72, 6]);

        translate([28-12, 8, -1])
        cube([12+1, 72-16, 8]);
        
        translate([6, 4, -8])
        rotate([0, 0, 0])
        cylinder(d = 3.2, h = 20, $fn = loc_res);
        translate([6, 4, +3])
        rotate([0, 0, 0])
        cylinder(d = 6.5, h = 20, $fn = loc_res);

        translate([28-6, 4, -8])
        rotate([0, 0, 0])
        cylinder(d = 3.2, h = 20, $fn = loc_res);
        translate([28-6, 4, +3])
        rotate([0, 0, 0])
        cylinder(d = 6.5, h = 20, $fn = loc_res);

        translate([6, 72-4, -8])
        rotate([0, 0, 0])
        cylinder(d = 3.2, h = 20, $fn = loc_res);
        translate([6, 72-4, +3])
        rotate([0, 0, 0])
        cylinder(d = 6.5, h = 20, $fn = loc_res);

        translate([28-6, 72-4, -8])
        rotate([0, 0, 0])
        cylinder(d = 3.2, h = 20, $fn = loc_res);
        translate([28-6, 72-4, +3])
        rotate([0, 0, 0])
        cylinder(d = 6.5, h = 20, $fn = loc_res);
    }
}

*translate([43, -14, 36-6])
side_part1_v1_0();

//motor_leg_v1_1(1, 32);

module put_together() {
    translate([43, -14, +(36-6)])
    side_part1_v1_0();
    translate([43, -14, -(36-6)])
    mirror([0, 0, 1])
    side_part1_v1_0();

    translate([125, -14, -72/2])
    rotate([90, 0, 90])    
    top_part_v1_0();

    //translate([0, 0, +0.1])
    motor_leg_v1_1(1, 32);
    //translate([0, 0, -0.1])
    rotate([180, 0, 0])    
    motor_leg_v1_1(1, 32);

    translate([100, 10, -52/2])
    rotate([90, -90, 0])
    gimbal_arduino_v3_0_pcb();
}

//put_together();

// printing
//lego_wheel_holder(64);    // 2x
//motor_leg_v1_1(0, 64);  // 2x

//mirror([0, 0, 1])
//side_part1_v1_0();  // 1 x right, 1 x left
top_part_v1_0(0, 64);
