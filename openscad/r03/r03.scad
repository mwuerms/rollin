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

// - keep this so far ----------------



// - new parts for r03 ----------------
module r03_motor_holder_1b(col = "LightBlue", th_1 = 6+6, bldc_angle = -45, as5600_angle = 0, loc_res = 32) {

    color(col)
    difference() {
        union() {
            // main body
            translate([0, 0, -6])
            cylinder(d = 50, h = th_1, $fn = loc_res);
            
            translate([0, -50/2, -6])
            cube([32, 50, th_1]);
        }
        // cuts
        // mounting plate
        translate([0, 0, -1])
        cylinder(d = 25.5, h = th_1+2, $fn = loc_res);
        // cable canal
        hull() {
            translate([10, +4, 1])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 40, $fn = loc_res);
            translate([10, -4, 1])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 40, $fn = loc_res);
            translate([10, +4, 2])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 40, $fn = loc_res);
            translate([10, -4, 2])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 40, $fn = loc_res);
        }
        hull() {
            translate([10, +4, 2])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 12, $fn = loc_res);
            translate([10, -4, 2])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 12, $fn = loc_res);
            translate([10, +4, th_1])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 1, $fn = loc_res);
            translate([10, -4, th_1])
            rotate([0, 90, 0])
            cylinder(d = 4, h = 1, $fn = loc_res);
        }
        // cuts
        // motor shaft and magnet holder
        translate([0, 0, -7])
        cylinder(d = 10, h = th_1+2, $fn = loc_res);
        
        // m3 screws to hold the motor
        translate([0, 0, -4])
        rotate([0, 0, bldc_angle])
        bldc5010_m3cut_stator(loc_res = loc_res);
        // space for the magnetig sensor, cut m3 thread into 2.5 mm holes
        translate([0, 0, -10])
        rotate([0, 0, as5600_angle])
        pcbAS5600_mount_holes_cut(cut_dia = 2.5, cut_len = 20, loc_res = loc_res);
        
        // cut m3 nuts and bolts
        translate([27, 9, 6-2])
        rotate([180, 0, 0])
        m3_cut(loc_res = loc_res);
        
        translate([27, 20, 6-2])
        m3_nut_bolt_cut();
        
        translate([27, -9, 6-2])
        m3_nut_bolt_cut();
            
        translate([27, -20, 6-2])
        rotate([180, 0, 0])
        m3_cut(loc_res = loc_res);
        
        // cut bottom cap
        translate([-50/2, -50/2, -7])
        cube([10, 50, th_1+2]);
        // m3 bolts to hold bottom cap
        translate([-12, +15, 0])
        rotate([0, 90, 0]) {
            m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 50, loc_res = loc_res);
            hull() {
                translate([0, 0, 0])
                m3_nut_cut();
                translate([10, 0, 0])
                m3_nut_cut();
            }
        }
        translate([26, +15, 0])
        rotate([0, 90, 0]) {
            hull() {
                translate([0, 0, 0])
                m3_nut_cut();
                translate([10, 0, 0])
                m3_nut_cut();
            }
        }
        translate([-12, -15, 0])
        rotate([0, 90, 0]) {
            m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 50, loc_res = loc_res);
            hull() {
                translate([0, 0, 0])
                m3_nut_cut();
                translate([10, 0, 0])
                m3_nut_cut();
            }
        }
        translate([26, -15, 0])
        rotate([0, 90, 0]) {
            hull() {
                translate([0, 0, 0])
                m3_nut_cut();
                translate([10, 0, 0])
                m3_nut_cut();
            }
        }
        
        // look inside 
        *translate([-20, 0, -20]) cube(80);
    }
}

module r03_motor_holder_2b(col = "LightGreen", th_2 = 2*4, bldc_angle = -45, as5600_angle = 0, loc_res = 32) {

    color(col)
    difference() {
        union() {
            // main body
            translate([0, 0, -0])
            cylinder(d = 50, h = th_2, $fn = loc_res);
            
            translate([0, -50/2, -0])
            cube([32, 50, th_2]);
        }
        // cut
        // cut m3 bolts to hold _1 and _2 together
        translate([27, 9, -1])
        m3_bolt_cut(m3_bolt_len = (th_2+2), loc_res = loc_res);
        
        translate([27, 18, -1])
        m3_bolt_cut(m3_bolt_len = (th_2+2), loc_res = loc_res);
        
        translate([27, -9, -1])
        m3_bolt_cut(m3_bolt_len = (th_2+2), loc_res = loc_res);
            
        translate([27, -18, -1])
        m3_bolt_cut(m3_bolt_len = (th_2+2), loc_res = loc_res);
        
        // m3 holes to top layer
        translate([23, 14, th_2/2])
        rotate([0, 90, 0])
        cylinder(d = 3.2, h = 10, $fn = loc_res);
        translate([23, -14, th_2/2])
        rotate([0, 90, 0])
        cylinder(d = 3.2, h = 10, $fn = loc_res);
        
        // cut walls
        translate([0, 0, -1])
        cylinder(d = 46, h = th_2+2, $fn = loc_res);
        translate([0, -(50/2-2), -1])
        cube([24, 46, th_2+2]);
        translate([0, -10/2, -1])
        cube([34, 10, th_2+2]);
        // cut bottom cap
        translate([-50/2, -50/2, -1])
        cube([10, 50, th_2+2]);
        // look inside 
        *translate([0, 0, -20]) cube(80);
    }
}


module r03_motor_holder_3b(col = "CadetBlue", th_1 = 6, th_2 = 6+4, loc_res = 32) {
    th_12 = th_1+th_2;
    color(col)
    translate([50/2-10, 0, 0])
    difference() {
        union() {
            // main body
            translate([0, 0, 0])
            cylinder(d = 50, h = th_12*2, $fn = loc_res);
        }
        // cuts
        // cut bottom cap
        translate([-(50/2-10), -50/2, -1])
        cube([50, 50, th_12*2+2]);
        // m3 bolts to hold bottom cap
        translate([-12, +15, th_12+th_2])
        rotate([0, 90, 0])
        m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 20, loc_res = loc_res);
        translate([-12, +15, th_12-th_2])
        rotate([0, 90, 0])
        m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 20, loc_res = loc_res);
        translate([-12, -15, th_12+th_2])
        rotate([0, 90, 0])
        m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 20, loc_res = loc_res);
        translate([-12, -15, th_12-th_2])
        rotate([0, 90, 0])
        m3_nut_and_bolt_cut(m3_bolt_and_nut_distance = 4, m3_bolt_len = 20, loc_res = loc_res);
        
        // sensor cut away
        translate([-19, -23/2, 2])
        cube([7, 23, 28]);
    }
}

module r03_motor_holder_1_2_3_v1_0_puttogether(bldc_angle = -45, as5600_angle = 45, loc_res = 32) {
    // components
    *translate([0, 0, 0])
    rotate([0, 0, bldc_angle])
    bldc5010_motor(0);

    *translate([0, 0, -7.6])
    rotate([0, 0, as5600_angle])
    pcbAS5600();

    *color("Red")
    translate([0, 0, -4])
    magnet_holder_5mm_v1_0();
    
    translate([0, 0, 0])
    r03_motor_holder_1b(bldc_angle = bldc_angle, as5600_angle = as5600_angle,loc_res = loc_res);
    
    translate([0, 0, -8-6])
    r03_motor_holder_2b(bldc_angle = bldc_angle, as5600_angle = as5600_angle,loc_res = loc_res);
    
    *translate([-(50/2-10), 0, -(6+4)-(6+6+4)])
    r03_motor_holder_3b();
}
*r03_motor_holder_1_2_3_v1_0_puttogether();

module r03_motor_holder_put_together(bldc_angle = -45, as5600_angle = 45, loc_res = 32) {
    // components
    *translate([0, 0, 0])
    rotate([0, 0, bldc_angle])
    bldc5010_motor(0);

    *translate([0, 0, -7.6])
    rotate([0, 0, as5600_angle])
    pcbAS5600();

    *color("Red")
    translate([0, 0, -4])
    magnet_holder_5mm_v1_0();
    
    translate([0, 0, 0])
    r03_motor_holder_1b(bldc_angle = bldc_angle, as5600_angle = as5600_angle,loc_res = loc_res);
    
    translate([0, 0, -8-6])
    r03_motor_holder_2b(bldc_angle = bldc_angle, as5600_angle = as5600_angle,loc_res = loc_res);
    
    translate([0, 0, -8-6-6])
    rotate([180, 0, 0])
    r03_motor_holder_1b(bldc_angle = bldc_angle, as5600_angle = as5600_angle,loc_res = loc_res);
    
    translate([-(50/2-10), 0, -(6+4)-(6+6+4)])
    r03_motor_holder_3b();
}
*r03_motor_holder_put_together(loc_res = 64);



// - print parts ------------------------------------------
*r03_motor_holder_1b(bldc_angle = -45, as5600_angle = 45,loc_res = 64);
*r03_motor_holder_2b(bldc_angle = -45, as5600_angle = 45,loc_res = 64);
*r03_motor_holder_3b(loc_res = 128);
lego_wheel_holder_v1_1(show = 0, loc_res = 128);



















// - old stuff

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
