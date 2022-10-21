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

module mpu650pcb(loc_res = 32) {
    origin();
    color("Blue")
    translate([0, 0, 0])
    cube([16, 14, 1]);
    color("White")
    translate([3.5, -2, 1])
    cube([10, 5.5, 4]);
    color("Gray")
    translate([7, 7, 1])
    cube([4, 4, 1]);
}

module gimbal_arduino_v3_0_pcb(loc_res = 32) {
    origin();
    // PCB
    difference() {
        union() {
            color("Blue")
            translate([0, 0, 0])
            cube([51, 52, 1.6]);

            color("Silver")
            translate([2.4, 4, -0.05])
            cylinder(d=4, h = 1.7, $fn = loc_res);
            color("Silver")
            translate([51-3.4, 4, -0.05])
            cylinder(d=4, h = 1.7, $fn = loc_res);
            color("Silver")
            translate([2.4, 52-3, -0.05])
            cylinder(d=4, h = 1.7, $fn = loc_res);
            color("Silver")
            translate([51-3.4, 52-3, -0.05])
            cylinder(d=4, h = 1.7, $fn = loc_res);
        }
        translate([2.4, 4, -0.1])
        cylinder(d=3.4, h = 1.8, $fn = loc_res);
        translate([51-3.4, 4, -0.1])
        cylinder(d=3.4, h = 1.8, $fn = loc_res);
        translate([2.4, 52-3, -0.1])
        cylinder(d=3.4, h = 1.8, $fn = loc_res);
        translate([51-3.4, 52-3, -0.1])
        cylinder(d=3.4, h = 1.8, $fn = loc_res);
    }
    // mini USB
    color("Silver")
    translate([35, -2, 1.5])
    cube([7.5, 9, 4]);
    // i2c - mpu650
    color("White")
    translate([3.5, -2, 1])
    cube([10, 5.5, 4]);
    // MCU
    color("Gray")
    translate([14, 13, 1.6])
    cube([7, 7, 1]);

    // 6xMOSFET
    color("Gray")
    for(n=[0:1:5])
    translate([10+n*6, 40, 1.6])
    cube([5, 4, 1]);
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

module pcbAS5600_mount_holes_cut(cut_dia = 3.5, cut_len = 10, loc_res = 32) {
    a = (19.7+12.4)/2/2;
    translate([+a,+a, 0])
    cylinder(d = cut_dia, h = cut_len, $fn = loc_res);
    translate([+a, -a, 0])
    cylinder(d = cut_dia, h = cut_len, $fn = loc_res);
    translate([-a, +a, 0])
    cylinder(d = cut_dia, h = cut_len, $fn = loc_res);
    translate([-a, -a, 0])
    cylinder(d = cut_dia, h = cut_len, $fn = loc_res);
}

module pcbAS5600_pcb(pcb_th = 1.5, loc_res = 32) {
    a = 23/2;
    hull() {
        translate([+a, +a, 0])
        cylinder(r = 3, h = pcb_th, $fn = loc_res);
        translate([+a, -a, 0])
        cylinder(r = 3, h = pcb_th, $fn = loc_res);
        translate([-a, +a, 0])
        cylinder(r = 3, h = pcb_th, $fn = loc_res);
        translate([-a, -a, 0])
        cylinder(r = 3, h = pcb_th, $fn = loc_res);
    }
}

module pcbAS5600(loc_res = 32) {
    difference() {
        union() {
            color("White")
            translate([0, 0, 0])
            pcbAS5600_pcb();
            // SOIC-8 4*5 mm
            color("Gray")
            translate([-2, -2.5, 1.5])
            cube([4, 5, 1]);
        }
        translate([0, 0, -1])
        pcbAS5600_mount_holes_cut();
    }
}

//pcbMPU9250();
//pcbBluePill();
