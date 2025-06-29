/**
 * Martin Egli
 * 2025-06-25
 * an other test stand, for r04
 */

use <parts.scad>
use <printparts.scad>
use <screws.scad>
use <mechanics_parts.scad>
use <electronics_parts.scad>

module rtest04_base_v1_0(show = 1, col = "LightBlue", bldc_angle= 45, as5600_angle = 45, loc_res = 32) {
    // show components
    if(show) {
        translate([0, 0, 19])
        lego_axis_holder_v1_1(0, loc_res = loc_res);

        translate([0, 0, 0])
        rotate([0, 0, bldc_angle])
        bldc5010_motor(0);

        translate([0, 0, -4])
        magnet_holder_5mm_v1_0();

        translate([0, 0, -7.6])
        rotate([0, 0, as5600_angle])
        pcbAS5600();
    }

    difference() {
        union() {
            // main body
            translate([-40, -15, -8])
            cube([80, 30, 8]);
        }
        // cut center hole
        translate([0, 0, -11])
        cylinder(d=10, h=12, $fn = loc_res);

        // cut for AS5600
        translate([0, 0, -(8+2)])
        rotate([0, 0, as5600_angle])
        pcbAS5600_pcb_cut(pcb_th = 4, loc_res = loc_res);
        // cut screw holes
        translate([0, 0, -(8+2)])
        rotate([0, 0, as5600_angle])
        pcbAS5600_mount_holes_cut(cut_dia = 2.5, loc_res = loc_res);
        // cut space for cables
        rotate([0, 0, +as5600_angle])
        translate([-4, -30, -9])
        cube([8, 60, 3.5]);
        rotate([0, 0, -as5600_angle])
        translate([-4, -30, -9])
        cube([8, 60, 3.5]);

        // cut bldc m3 mounting screws
        translate([0, 0, -3.2])
        rotate([0, 0, bldc_angle])
        bldc5010_m3cut_stator(loc_res = loc_res);

        // cut side m3 mounting holes to the rigth
        translate([-41, +8, -4])
        rotate([0, +90, 0]) {
            // cut screw hole
            cylinder(d = 4, h=15, $fn = loc_res);
            // cut m3 nut
            hull() {
                translate([+6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
                translate([-6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
            }
        }
        translate([-41, -8, -4])
        rotate([0, +90, 0]) {
            // cut screw hole
            cylinder(d = 4, h=15, $fn = loc_res);
            // cut m3 nut
            hull() {
                translate([+6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
                translate([-6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
            }
        }
        // cut side m3 mounting holes to the left
        translate([41, +8, -4])
        rotate([0, -90, 0]) {
            // cut screw hole
            cylinder(d = 4, h=15, $fn = loc_res);
            // cut m3 nut
            hull() {
                translate([+6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
                translate([-6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
            }
        }
        translate([41, -8, -4])
        rotate([0, -90, 0]) {
            // cut screw hole
            cylinder(d = 4, h=15, $fn = loc_res);
            // cut m3 nut
            hull() {
                translate([+6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
                translate([-6, 0, 4])
                cylinder(d = 6, h=3, $fn = loc_res);
            }
        }

        // cut down wood screws mounting holes to the right
        translate([-32, 0, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
        translate([-20, +8, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
        translate([-20, -8, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
        // cut down wood screws mounting holes to the left
        translate([+32, 0, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
        translate([+20, +8, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
        translate([+20, -8, -35.2])
        union() {
            // on top of screw head
            translate([0, 0, 35])
            cylinder(d = 8, h = 5, $fn = loc_res);
            // screw head
            translate([0, 0, 35-3])
            cylinder(d2 = 8, d1 = 4, h = 3, $fn = loc_res);
            // screw
            translate([0, 0, 0])
            cylinder(d = 4, h = 35, $fn = loc_res);
        }
    }

}

module rtest04_side_v1_0(loc_res = 32) {

    difference() {
        union() {
            // main body
            translate([0, -15, 0])
            cube([8, 30, 100]);
        }
        
        // cut m3 slots
        translate([0, -8, 0])
        hull() {
            translate([-1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
            translate([-1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
        }
        translate([0, -8, 0])
        hull() {
            translate([-1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d = 7, h = 2, $fn = loc_res);
            translate([-1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d = 7, h = 2, $fn = loc_res);
        }
        translate([0, -8, 0])
        hull() {
            translate([+1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d1 = 7, d2 = 4, h = 2, $fn = loc_res);
            translate([+1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d1 = 7, d2 = 4, h = 2, $fn = loc_res);
        }
        // cut m3 slots
        translate([0, -0, 0])
        hull() {
            translate([-1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
            translate([-1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
        }
        // cut m3 slots
        translate([0, +8, 0])
        hull() {
            translate([-1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
            translate([-1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 10, $fn = loc_res);
        }
        translate([0, +8, 0])
        hull() {
            translate([-1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d = 7, h = 2, $fn = loc_res);
            translate([-1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d = 7, h = 2, $fn = loc_res);
        }
        translate([0, +8, 0])
        hull() {
            translate([+1, 0, 4])
            rotate([0, 90, 0])
            cylinder(d1 = 7, d2 = 4, h = 2, $fn = loc_res);
            translate([+1, 0, 100-4])
            rotate([0, 90, 0])
            cylinder(d1 = 7, d2 = 4, h = 2, $fn = loc_res);
        }
    }
}

module rtest04_v1_0_put_together() {
    // lower motor
    translate([0, 0, 0])
    rotate([0, 0, 0])
    rtest04_base_v1_0(show = 1);
    // upper motor
    translate([0, 0, 72])
    rotate([0, 180, 0])
    rtest04_base_v1_0(show = 1);

    translate([-51, 0, -8])
    rtest04_side_v1_0();

}

// show
*rtest04_base_v1_0(show = 1);
*rtest04_v1_0_put_together();
// print
*rtest04_base_v1_0(show = 0, loc_res = 128);
rtest04_side_v1_0(loc_res = 128);
