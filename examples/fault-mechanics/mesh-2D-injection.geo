H = 10.0;
L = 20.0;
lc0 = 0.7;
lc1 = 0.07;
lc2 = 0.0014;

Point(1) = {-L/ 2.0, -H/2.0, 0.0, lc0};
Point(2) = {0.0, -H/2.0, 0.0, lc0};
Point(3) = {L/2.0, -H/2.0, 0.0, lc0};
Point(4) = {L/2.0, 0.0, 0.0, lc1};
Point(5) = {L/20.0, 0.0, 0.0, lc2};
Point(6) = {0.0, 0.0, 0.0, lc2};
Point(7) = {-L/20.0, 0.0, 0.0, lc2};
Point(8) = {-L/2.0, 0.0, 0.0, lc1};

Point(9) = {L/2.0, H/2.0, 0.0, lc0};
Point(10) = {0.0, H/2.0, 0.0, lc0};
Point(11) = {-L/2.0, H/2.0, 0.0, lc0};

// Lower block
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};
Plane Surface(1) = {1};

// Upper block
Line(9) = {4, 9};
Line(10) = {9, 10};
Line(11) = {10, 11};
Line(12) = {11, 8};
Line Loop(2) = {-7, -6, -5, -4, 9, 10, 11, 12};
Plane Surface(2) = {2};

Physical Surface(0) = {1};
Physical Surface(1) = {2};
Physical Line("bottom") = {1, 2};
Physical Line("right") = {3, 9};
Physical Line("top") = {10, 11};
Physical Line("left") = {8, 12};
Physical Point("injection") = {6};
Physical Point("no_disp_x") = {2, 10};
Physical Point("no_disp_y") = {4, 8};
Physical Line("interface") = {4, 5, 6, 7};