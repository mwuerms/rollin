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

module motor_holder_v1_1(show = 1, loc_res = 32) {
    bldc_angle = -45;
    as5600_angle = 45;
    if(show) {
        translate([0, 0, 0])
        rotate([0, 0, bldc_angle])
        bldc5010_motor(0);

        *translate([0, 0, -10])
        rotate([0, 0, as5600_angle])
        pcbAS5600();

        translate([0, 0, -4])
        magnet_holder_5mm_v1_0();
    }
    difference() {
        union() {
            translate([0, 0, 3])
            cylinder(d = 50, h = 3, $fn = loc_res);
            hull() {
                translate([0, 0, 3])
                cylinder(d = 50, h = 1, $fn = loc_res);
                translate([0, 0, -5])
                rotate([0, 0, as5600_angle])
                pcbAS5600_pcb(pcb_th = 1, loc_res = loc_res);
            }
            translate([0, 0, -7])
            rotate([0, 0, as5600_angle])
            pcbAS5600_mount_holes_cut(6, 6, loc_res);

            // connection rods
            translate([10, +12, 1])
            rotate([0, 90, 0])
            cylinder(d = 8-1.2, h = 30, $fn = loc_res);
            translate([10, -12, 1])
            rotate([0, 90, 0])
            cylinder(d = 8-1.2, h = 30, $fn = loc_res);
        }
        translate([0, 0, -3])
        rotate([0, 0, bldc_angle])
        bldc5010_m3cut_stator(0);

        // magnetic encoder
        translate([0, 0, -10])
        rotate([0, 0, as5600_angle])
        pcbAS5600_mount_holes_cut(cut_dia = 2.5);
        // magnet hole
        translate([0, 0, -10])
        cylinder(d = 10, h = 20, $fn = loc_res);

        // motor base plate
        translate([0, 0, 0])
        cylinder(d = 32, h = 10, $fn = loc_res);

        // motor cable channel
        hull() {
            translate([0, +3, 3])
            rotate([0, 90, 0])
            cylinder(d = 6, h = 30, $fn = loc_res);
            translate([0, -3, 3])
            rotate([0, 90, 0])
            cylinder(d = 6, h = 30, $fn = loc_res);
            translate([0, +3, 6])
            rotate([0, 90, 0])
            cylinder(d = 6, h = 30, $fn = loc_res);
            translate([0, -3, 6])
            rotate([0, 90, 0])
            cylinder(d = 6, h = 30, $fn = loc_res);
        }
        // m3 holes for connection rods
        translate([28, +12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([36, +12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([28, -12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([36, -12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);

        // look inside translate([0, 0, -20]) cube(40);
    }
}

module motor_holder_stand_v1_0(loc_res = 32) {
    difference() {
        union() {
            hull() {
                translate([0, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([0, +20, 45])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);
            }
            hull() {
                translate([0, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([0, -20, 45])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);
            }
            hull() {
                translate([0, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([0, +20, -15])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);
            }
            hull() {
                translate([0, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([0, -20, -15])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);
            }
            hull() {
                translate([0, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([0, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);
            }

            hull() {
                translate([0, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 3, $fn = loc_res);

                translate([0, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);
            }

            hull() {
                translate([0, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([4, +12, 1])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 4, $fn = loc_res);
            }
            hull() {
                translate([0, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 16, h = 3, $fn = loc_res);

                translate([4, -12, 1])
                rotate([0, 90, 0])
                cylinder(d = 8, h = 4, $fn = loc_res);
            }
            
            // connection rods
            translate([0, +12, 1])
            rotate([0, 90, 0])
            cylinder(d = 8-1.2, h = 30, $fn = loc_res);
            translate([0, -12, 1])
            rotate([0, 90, 0])
            cylinder(d = 8-1.2, h = 30, $fn = loc_res);
        }
        // m3 holes for connection rods
        translate([18, +12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([26, +12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([18, -12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
        translate([26, -12, -4])
        cylinder(d = 3, h = 10, $fn = loc_res);
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

module connection_rod_template_v1_0(loc_res = 32) {
    difference() {
        union() {
            translate([0, -6, -6])
            cube([24, 12, 12]);
        }
        translate([-1, 0, 0])
        rotate([0, 90, 0])
        cylinder(d = 8.2, h = 35, $fn = loc_res);
    
        // m3 holes for connection rods
        translate([8, 0, -15])
        cylinder(d = 3, h = 30, $fn = loc_res);
        translate([16, 0, -15])
        cylinder(d = 3, h = 30, $fn = loc_res);
    }
}

//bldc5010_test_stand_put_together();
// printing
//bldc5010_test_arm_v1_0(0); // 1x
//bldc5010_test_stand_v1_0(); // 1x
//lego_wheel_holder_v1_1(0);  // 2x

//motor_holder_v1_1(0, 64); // 2x
//magnet_holder_5mm_v1_0(64); // 2x

/*translate([100, 0, 0])
rotate([0, 0, 180])
motor_holder_v1_1();
motor_holder_stand_v1_0(64); // 1x*/
connection_rod_template_v1_0(64); // 1 .. 2 x