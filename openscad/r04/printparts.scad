/**
 * Martin Egli
 * 2025-06-20
 * good enough parts to be printed and reused
 */

use <parts.scad>
use <screws.scad>
use <mechanics_parts.scad>
use <electronics_parts.scad>

module lego_axis_holder_v1_1(show = 1, loc_res = 32) {
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

module magnet_holder_5mm_v1_0(loc_res = 32) {
    difference() {
        translate([0, 0, 0])
        cylinder(d = 6, h = 4, $fn = loc_res);

        // magnet dia 4 x 2mm 
        translate([0, 0, -1])
        cylinder(d = 4.1, h = 4, $fn = loc_res);

        // axis, 4 mm
        translate([0, 0, -1])
        cylinder(d = 4.1, h = 10, $fn = loc_res);
    }
}

// print
lego_axis_holder_v1_1(0);
*magnet_holder_5mm_v1_0(128);