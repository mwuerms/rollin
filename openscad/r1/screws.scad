/**
 * Martin Egli
 * 2022-09-26
 * mechanical parts
 */

use <parts.scad>

module m2x6mm(loc_res = 32) {
    color("LightGray") {
        translate([0, 0, 0])
        cylinder(d = 2, h = 8, $fn = loc_res);
        translate([0, 0, 6])
        cylinder(d = 3.4, h = 2, $fn = loc_res);
    }
}

module m2x6mm_cut(loc_res = 32) {
    translate([0, 0, -10])
    cylinder(d = 2.2, h = 11, $fn = loc_res);
    translate([0, 0, 0])
    cylinder(d = 4, h = 10, $fn = loc_res);
}

m2x6mm_cut();