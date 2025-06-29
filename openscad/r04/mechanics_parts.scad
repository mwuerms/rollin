/**
 * Martin Egli
 * 2022-09-14
 * mechanical parts
 */

use <parts.scad>
use <screws.scad>

module lego_axis_cut(len = 10) {
    hull() {
        translate([-1.0, -0.9, 0]) 
        cube([2.0, 1.8, len]);
        translate([-0.9, -1.0, 0]) 
        cube([1.8, 2.0, len]);
    }
    hull() {
        translate([-2.4, -0.7, 0]) 
        cube([4.8, 1.4, len]);
        translate([-2.2, -0.9, 0]) 
        cube([4.4, 1.8, len]);
    }
    hull() {
        translate([-0.9, -2.2, 0]) 
        cube([1.8, 4.4, len]);
        translate([-0.7, -2.4, 0]) 
        cube([1.4, 4.8, len]);
    }
}

module bldc5010_m3cut_rotor(show = 1, len = 10, loc_res = 32) {
    if(show) {
        origin();
    }
    r1 = (15+9.6)/2/2;
    for(n=[0:1:3]) {
        translate([r1*cos(n*90), r1*sin(n*90), 0]) {
            rotate([180, 0, 0])
            m3_cut(len = len, loc_res = loc_res);
        }
    }
}

module bldc5010_m3cut_stator(show = 1, len = 10, loc_res = 32) {
    if(show) {
        origin();
    }
    r1 = (19+13.4)/2/2;
    for(n=[0:2:3]) {
        translate([r1*cos(n*90), r1*sin(n*90), 0]) {
            m3_cut(len = len, loc_res = loc_res);
        }
    }
    r2 = (22+16.6)/2/2;
    for(n=[1:2:3]) {
        translate([r2*cos(n*90), r2*sin(n*90), 0]) {
            m3_cut(len = len, loc_res = loc_res);
        }
    }
}

module bldc5010_motor(show = 1, loc_res = 32) {
    if(show) {
        origin(25);
        #translate([0, 0, 20])
        bldc5010_m3cut_rotor();
        #translate([0, 0, -2])
        bldc5010_m3cut_stator();
    }
    color("LightGray") {
        difference() {
            union() {
                // from bottom
                translate([0,0, 0])
                cylinder(d = 25, h = 2, $fn = loc_res);
                translate([0,0, 0])
                cylinder(d = 14.4, h = 19, $fn = loc_res);
                translate([0,0, 7])
                cylinder(d = 50, h = 10, $fn = loc_res);
                translate([0,0, 17])
                cylinder(d1 = 50, d2 = 18, h = 2, $fn = loc_res);
            }
            translate([0, 0, 20]) 
            bldc5010_m3cut_rotor();
            translate([0, 0, -2])
            bldc5010_m3cut_stator();
        }
    }
    color("DarkGray") {
        translate([0,0, -1.5])
        cylinder(d = 4, h = 24, $fn = loc_res);
    }
    translate([0, 0, 3])
    rotate([0, 0, 45]) {
        color("Red")
        translate([4, -2.5, 0])
        rotate([0, 90, 0])
        cylinder(d = 2.5, h=20, $fn = loc_res);
        color("Black")
        translate([4, 0, 0])
        rotate([0, 90, 0])
        cylinder(d = 2.5, h=20, $fn = loc_res);
        color("Yellow")
        translate([4, +2.5, 0])
        rotate([0, 90, 0])
        cylinder(d = 2.5, h=20, $fn = loc_res);
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

//bldc5010_motor();
//flycat2204();
