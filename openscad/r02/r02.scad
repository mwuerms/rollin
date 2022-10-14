/**
 * Martin Egli
 * 2022-09-14
 */

use <parts.scad>
use <screws.scad>
use <mechanics_parts.scad>
use <electronics_parts.scad>

module lego_wheel_holder_v1_1(show = 1, loc_res = 32) {
    if(show) {
        translate([0, 0, -19])
        bldc5010_motor(0);
        color("Gray")
        translate([0, 0, 4])
        lego_axis_cut(25);
    }
    difference() {
        union() {
            translate([0, 0, 0])
            cylinder(d = 18, h = 7, $fn = loc_res);
            translate([0, 0, 7])
            cylinder(d1 = 18, d2 = 7.8, h = 3, $fn = loc_res);
            translate([0, 0, 0])
            cylinder(d = 7.8, h = 16, $fn = loc_res);
        }
        translate([0, 0, -1])
        cylinder(d = 4.2, h = 4+1, $fn = loc_res);
        translate([0, 0, 4])
        lego_axis_cut(20);

        translate([0, 0, 5])
        bldc5010_m3cut_rotor(0);
    }
}

module bldc5010_test_arm_v1_0(show = 1, loc_res = 32) {
    if(show) {
        translate([0, 0, -19])
        bldc5010_motor(0);
    }
    difference() {
        union() {
            translate([0, 0, 0])
            cylinder(d = 18, h = 7, $fn = loc_res);
            translate([0, 0, 4])
            hull() {
                translate([0, 0, 0]) 
                cylinder(d = 18, h = 3, $fn = loc_res);
                translate([0, 100, 0])
                cylinder(d = 18, h = 3, $fn = loc_res);
            }
            translate([+9, 100, 4])
            cylinder(d = 4, h = 3, $fn = loc_res);
            translate([-9, 100, 4])
            cylinder(d = 4, h = 3, $fn = loc_res);
        }
        translate([0, 0, -1])
        cylinder(d = 4.2, h = 7+2, $fn = loc_res);
        translate([0, 100, -1])
        cylinder(d = 4, h = 7+2, $fn = loc_res);

        translate([0, 0, 5])
        bldc5010_m3cut_rotor(0);
    }
}

module bldc5010_test_stand_v1_0(loc_res = 32) {
    difference() {
        union() {
            hull() {
                translate([0, 0, 0])
                cylinder(d = 28, h = 5, $fn = loc_res);
                translate([30, +25, 0]) 
                cylinder(d = 6, h = 5, $fn = 32);
                translate([30, -25, 0])
                cylinder(d = 6, h = 5, $fn = 32);
            }
            hull() {
                translate([30, +25, -25]) 
                cylinder(d = 6, h = 45, $fn = 32);
                translate([30, -25, -25])
                cylinder(d = 6, h = 45, $fn = 32);
            }
        }
        translate([0, 0, -1])
        cylinder(d = 7, h = 9, $fn = 32);

        translate([0, 0, -1])
        rotate([0, 0, 45])
        bldc5010_m3cut_stator(0);
        // m3 holes to mount to a base plate
        translate([25, 0, -15]) 
        rotate([0, 90, 0])
        cylinder(d = 3.1, h = 20, $fn = 32);
        translate([25, +20, -15]) 
        rotate([0, 90, 0])
        cylinder(d = 3.1, h = 20, $fn = 32);
        translate([25, -20, -15]) 
        rotate([0, 90, 0])
        cylinder(d = 3.1, h = 20, $fn = 32);
    }
}

module bldc5010_test_stand_put_together() {
    translate([0, 0, 19])
    bldc5010_test_arm_v1_0(0);
    translate([0, 0, 0])
    rotate([0, 0, 45])
    bldc5010_motor(0);
    translate([0, 0, -5])
    bldc5010_test_stand_v1_0(0);
}

//bldc5010_test_stand_put_together();
// printing
//bldc5010_test_arm_v1_0(0); // 1x
bldc5010_test_stand_v1_0(); // 1x
//lego_wheel_holder_v1_1(0);  // 2x

