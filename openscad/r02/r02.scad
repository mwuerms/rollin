/**
 * Martin Egli
 * 2022-09-14
 */

use <parts.scad>
use <screws.scad>
use <mechanics_parts.scad>
use <electronics_parts.scad>

module lego_wheel_holder(show = 1, loc_res = 32) {
    if(show) {
        translate([0, 0, -19])
        bldc5010_motor(0);
        color("Gray")
        translate([0, 0, 4])
        lego_axis_cut();
    }
    difference() {
        union() {
            translate([0, 0, 0])
            cylinder(d = 18, h = 2, $fn = loc_res);
            translate([0, 0, 2])
            cylinder(d1 = 18, d2 = 7.5, h = 2, $fn = loc_res);
            translate([0, 0, 0])
            cylinder(d = 7.5, h = 9, $fn = loc_res);
        }
        translate([0, 0, -1])
        cylinder(d = 4.5, h = 4+1, $fn = loc_res);
        translate([0, 0, 4])
        lego_axis_cut();

        translate([0, 0, -1])
        bldc5010_m3cut_rotor(0);
    }
}

/*
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
*/
lego_wheel_holder(0);