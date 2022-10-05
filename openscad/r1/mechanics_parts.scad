/**
 * Martin Egli
 * 2022-09-14
 * mechanical parts
 */

use <parts.scad>
use <screws.scad>

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


module flycat2204_rotor_cut(loc_res = 32) {
    // M2 holes rotor
    for(n=[0:1:3])
        translate([6*cos(n*90+45), 6*sin(n*90+45), 2.5])
        cylinder(d = 2.2, h = 20, $fn = loc_res);
}

module flycat2204_rotor_cut_m2x6mm(loc_res = 32) {
    // M2 holes rotor
    for(n=[0:1:3])
        translate([6*cos(n*90+45), 6*sin(n*90+45), 0])
        m2x6mm_cut();
}

module flycat2204_stator_cut(loc_res = 32) {
    // M2.5 holes stator
    for(n=[0:1:3])
    translate([6*cos(n*90+45), 6*sin(n*90+45), -5])
    cylinder(d = 2.2, h = 3.5+5, $fn = loc_res);
}

module flycat2204(loc_res = 32) {
    origin();
    color("Gray") {
        difference() {
            union() {
                // base
                translate([0, 0, 0])
                cylinder(d = 28, h = 3.4, $fn = loc_res);
                // rotor
                translate([0, 0, 3.5])
                cylinder(d = 28, h = 13-3.5, $fn = loc_res);
            }
            hull() {
                translate([6+2, +(6-2), -0.5])
                cylinder(r = 2, h = 1, $fn = loc_res);
                translate([6+2+6, +(6-2), -0.5])
                cylinder(r = 2, h = 1, $fn = loc_res);
                translate([6+2+6, -(6-2), -0.5])
                cylinder(r = 2, h = 1, $fn = loc_res);
                translate([6+2, -(6-2), -0.5])
                cylinder(r = 2, h = 1, $fn = loc_res);
            }
            translate([0, 0, -1])
            cylinder(d = 3.5, h = 13+2, $fn = loc_res);

            flycat2204_rotor_cut();
            flycat2204_stator_cut();
        }
    }
}

//bldc5010_mockup();
flycat2204();
