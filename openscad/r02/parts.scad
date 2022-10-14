/**
 * Martin Egli
 * 2022-09-14
 * common parts
 */

module origin(length = 5, loc_res = 32) {
    color("Red")
    translate([0, 0, 0])
    cylinder(d1 = 0, d2 = 1, h = length, $fn = loc_res);
}
