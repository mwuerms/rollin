/**
 * Martin Egli
 * 2022-09-14
 * electronics
 */

use <parts.scad>

module pcbMPU9250(loc_res = 32) {
    origin();
    color("Blue") {
        difference() {
            translate([0, 0, 0])
            cube([26, 16, 1.5]);
            translate([3, 16-3, -1])
            cylinder(d = 3, h = 4.5, $fn = loc_res);
            translate([26-3, 16-3, -1])
            cylinder(d = 3, h = 4.5, $fn = loc_res);
        }
    }
    color("Black") {
        translate([26/2-2, 16/2-2, 1])
        cube([4, 4, 2]);
    }
}

module pcbMPU9250cut(loc_res = 32) {
    hull() {
        translate([3/2, 3/2, -3])
        cylinder(d = 3, h = 5, $fn = loc_res);
        translate([26-3/2, 3/2, -3])
        cylinder(d = 3, h = 5, $fn = loc_res);
    }
}

module pcbMPU9250mounts(loc_res = 32) {
    translate([3, 16-3, 0])
    cylinder(d = 2, h = 2, $fn = loc_res);
    translate([26-3, 16-3, 0])
    cylinder(d = 2, h = 2, $fn = loc_res);
}

module pcbBluePill(loc_res = 32) {
    origin();
    color("Blue") {
        translate([0, 0, 0])
        cube([53, 22.5, 1.5]);
    }
    color("Black") {
        translate([21, 11, 1])
        rotate([0, 0, -45])
        cube([8, 8, 2]);
    }
    color("Silver") {
        translate([0, 7, 1.5])
        cube([6, 8, 2]);
        
        for(n = [0:1:20-1]) {
            translate([2.3+n*2.54, 3.3, -5])
            cylinder(d = 0.6, h = 8, $fn = loc_res);
            translate([2.3+n*2.54, 3.3+6*2.54, -5])
            cylinder(d = 0.6, h = 8, $fn = loc_res);
        }
    }
}

module pcbBluePill_header_holes(loc_res = 32) {
    for(n = [0:1:20-1]) {
        translate([2.3+n*2.54, 3.3, -5])
        cylinder(d = 1, h = 8, $fn = loc_res);
        translate([2.3+n*2.54, 3.3+6*2.54, -5])
        cylinder(d = 1, h = 8, $fn = loc_res);
    }
}

//pcbMPU9250();
//pcbBluePill();