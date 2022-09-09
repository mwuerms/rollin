/**
 * Martin Egli
 * 2022-09-07
 * holder for MPU-9250 + Bluepill
 * to get some angle measurements
 */


module pcbMPU9250(loc_res = 32) {
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
//cylinder(d = 75.5, h = 10, $fn = 256);

module place_pcbs() {
    translate([(53-26)/2, 25, 0])
    pcbMPU9250();
    pcbBluePill();
}

module pcbHolder(loc_res = 32, show_pcbs = 1) {
    difference() {
        union() {
            // needle
            hull() {
                translate([0, 0, -2])
                cylinder(d = 10, h = 2, $fn = loc_res);
                translate([0, +25, -2])
                cylinder(d = 0.1, h = 2, $fn = loc_res);
            }
            hull() {
                translate([0, 0, -2])
                cylinder(d = 10, h = 2, $fn = loc_res);
                translate([-15, -8, -2])
                cylinder(d = 2, h = 2, $fn = loc_res);
                translate([+15, -8, -2])
                cylinder(d = 2, h = 2, $fn = loc_res);
                translate([-28, -41, -2])
                cylinder(d = 2, h = 2, $fn = loc_res);
                translate([+28, -41, -2])
                cylinder(d = 2, h = 2, $fn = loc_res);
            }
            translate([-29, -43, -2])
            cube([29*2, 2, 28]);
            
            translate([-26/2, -25, 0])
            pcbMPU9250mounts();
        }
        // cut outs
        // center hole
        translate([0, 0, -10])
        cylinder(d = 3, h = 20, $fn = loc_res);
        
        translate([-26/2, -25, 0])
        pcbMPU9250cut();
        
        translate([-53/2, -40, 0])
        rotate([-90, 0, 0])
        translate([0, -26, 0])
        pcbBluePill_header_holes();
    }
    if(show_pcbs) {
        translate([-26/2, -25, 0])
        pcbMPU9250();
        translate([-53/2, -40, 0])
        rotate([-90, 0, 0])
        translate([0, -26, 0])
        pcbBluePill();
    }
}

module stand(loc_res = 32) {
    difference() {
        union() {
            translate([0, 0, 0])
            cylinder(d = 60, h = 2, $fn = loc_res);
            translate([-20, -70, 0])
            cylinder(d = 8, h = 30, $fn = loc_res);
            translate([+20, -70, 0])
            cylinder(d = 8, h = 30, $fn = loc_res);
            hull() {
                translate([0, 0, 0])
                cylinder(d = 8, h = 2, $fn = loc_res);
                translate([-20, -70, 0])
                cylinder(d = 8, h = 2, $fn = loc_res);
                translate([+20, -70, 0])
                cylinder(d = 8, h = 2, $fn = loc_res);
            }
        }
        // center hole
        translate([0, 0, -10])
        cylinder(d = 3, h = 20, $fn = loc_res);
        
        // hollow feets
        translate([-20, -70, -2])
        cylinder(d = 6, h = 30+4, $fn = loc_res);
        translate([+20, -70, -2])
        cylinder(d = 6, h = 30+4, $fn = loc_res);
    }
}

// middle axis in the center
/*translate([0, 0, -10])
cylinder(d = 3, h = 20, $fn = 32);
*/

translate([0, 0, 0])
//pcbHolder(32, 0);

//translate([0, 0, -6])
stand(32);
