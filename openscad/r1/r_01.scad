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

module motor_leg_v1_0(show = 1, loc_res = 32) {
    if(show) {
        translate([0, 0, 6]) 
        motorA();

        color("DarkGray")
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
            translate([18, -14, 0])
            cube([20, 28, 30]);
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
            translate([12, 0, -1]) 
            cylinder(d = 6, h = 2, $fn = loc_res);
            translate([62, 0, -1]) 
            cylinder(d = 6, h = 2, $fn = loc_res);
        }

        // M3 holes
        translate([18, -7, -1]) 
        cylinder(d = 3, h = 8, $fn = loc_res);
        translate([18, +7, -1]) 
        cylinder(d = 3, h = 8, $fn = loc_res);
        translate([33, -7, -1]) {
            cylinder(d = 3, h = 38, $fn = loc_res);
            translate([0, 0, 8]) 
            cylinder(d = 6.5, h = 38, $fn = loc_res);
            hull() {
                translate([0, 0, 8]) 
                cylinder(d = 8, h = 3, $fn = 6);
                translate([4, 0, 8]) 
                cylinder(d = 8, h = 3, $fn = 6);
            }
        }
        translate([33, +7, -1]) {
            cylinder(d = 3, h = 38, $fn = loc_res);
            translate([0, 0, 8]) 
            cylinder(d = 6.5, h = 38, $fn = loc_res);
            hull() {
                translate([0, 0, 8]) 
                cylinder(d = 8, h = 3, $fn = 6);
                translate([4, 0, 8]) 
                cylinder(d = 8, h = 3, $fn = 6);
            }
        }

        hull() {
            translate([0, 0, 11])
            cylinder(d = 60, h = 40, $fn = loc_res);
            translate([0, 0, 6])
            cylinder(d = 50, h = 40, $fn = loc_res);
        }
        translate([18+20, -15, -1])
        cube([20, 50, 32]);
    }
}


// printing
//lego_wheel_holder(64);
motor_leg_v1_0(0, 64);
