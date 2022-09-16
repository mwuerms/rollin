/**
 * Martin Egli
 * 2022-09-14
 * mechanical parts
 */

use <parts.scad>

module bldc5010_mockup(loc_res = 32) {
    origin(25);
    color("Silver") {
        translate([0,0, 7.7])
        cylinder(d = 50, h = 20-10, $fn = loc_res);
        translate([0,0, 7.7+(20-10)])
        cylinder(d1 = 50, d2 = 20, h = 10-7.7, $fn = loc_res);
        translate([0,0, 0])
        cylinder(d = 10, h = 10, $fn = loc_res);
        translate([0,0, 0])
        cylinder(d = 22, h = 2, $fn = loc_res);
    }
}

//bldc5010_mockup();