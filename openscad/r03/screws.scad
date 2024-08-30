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

//m2x6mm_cut();

module m3_cut(len = 10, m3_thread_dia = 3.2, m3_head_dia = 6.2, loc_res = 32) {
    translate([0, 0, 0]) 
    cylinder(d = m3_thread_dia, h = len, $fn = loc_res);
    translate([0, 0, -1.5]) 
    cylinder(d2 = m3_thread_dia, d1= m3_head_dia, h = 1.5, $fn = loc_res);
    translate([0, 0, -(len+1.5)]) 
    cylinder(d = m3_head_dia, h = len, $fn = loc_res);
}
//m3_cut();

module m3_nut_cut(len = 3, m3_nut_dia = 6.2, loc_res = 32) {
    translate([0, 0, 0]) 
    cylinder(d = m3_nut_dia, h = len, $fn = 6);
}
//m3_nut_cut();

module m3_nut_bolt_cut(m3_nut_len = 3, m3_bolt_len = 10, m3_thread_dia = 3.2, m3_nut_dia = 6.2, loc_res = 32) {
    translate([0, 0, 0]) 
    cylinder(d = m3_nut_dia, h = m3_nut_len, $fn = 6);
    translate([0, 0, -m3_bolt_len]) 
    cylinder(d = m3_thread_dia, h = m3_bolt_len, $fn = loc_res);
}
m3_nut_bolt_cut();