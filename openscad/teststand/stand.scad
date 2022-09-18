/**
 * Martin Egli
 * 2022-09-18
 * teststand for 1 flycat 2204 bldc gimbal motor and 1 mpu6500
 */

use <parts.scad>


module flycat2204_rotor_cut(loc_res = 32) {
    // M2 holes rotor
    for(n=[0:1:3])
    translate([6*cos(n*90+45), 6*sin(n*90+45), 2.5])
    cylinder(d = 2, h = 20, $fn = loc_res);
}

module flycat2204_stator_cut(loc_res = 32) {
    // M2.5 holes stator
    for(n=[0:1:3])
    translate([6*cos(n*90+45), 6*sin(n*90+45), -5])
    cylinder(d = 2.5, h = 3.5+5, $fn = loc_res);
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

module holder(show = 1, loc_res = 32) {
    if(show) {
        origin();
        translate([40, 9, 3])
        rotate([0, 0, 180])
        mpu650pcb();

        translate([-30, 0, 3])
        flycat2204();
    }
    difference() {
        hull() {
            translate([40, 0, 0])
            cylinder(d = 20, h=3, $fn = loc_res);
            translate([-30, 0, 0])
            cylinder(d = 30, h=3, $fn = loc_res);
        }
        translate([0, 0, -1])
        cylinder(d =3, h=10, $fn = loc_res);
        translate([-30, 0, -1])
        cylinder(d = 5, h=10, $fn = loc_res);

        hull() {
            translate([16, 0, -1])
            cylinder(d =4, h=10, $fn = loc_res);
            translate([8, 0, -1])
            cylinder(d =4, h=10, $fn = loc_res);
        }
        translate([-30, 0, 3])
        flycat2204_stator_cut();

        translate([40.5, 9.5, 2])
        rotate([0, 0, 180])
        cube([17, 15, 1.5]);
    }
}

holder(0);
