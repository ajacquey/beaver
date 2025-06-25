R = 1.0;
H = 10*R;
L = 20*R;
lc0 = 0.8*R;
lc1 = 0.02*R;
lc2 = 0.002*R;

// Interface
Point(1) = {0, 0, 0, lc2};
Point(2) = {-R, 0, 0, lc1};
Point(3) = {0, -R, 0, lc1};
Point(4) = {+R, 0, 0, lc1};
Point(5) = {0, +R, 0, lc1};

Point(6) = {-L/2, 0, 0, lc0};
Point(7) = {-L/2, -L/2, 0, lc0};
Point(8) = {0, -L/2, 0, lc0};
Point(9) = {+L/2, -L/2, 0, lc0};
Point(10) = {+L/2, 0, 0, lc0};
Point(11) = {+L/2, +L/2, 0, lc0};
Point(12) = {0, +L/2, 0, lc0};
Point(13) = {-L/2, +L/2, 0, lc0};

Circle(1) = {2, 1, 3};
Circle(2) = {3, 1, 4};
Circle(3) = {4, 1, 5};
Circle(4) = {5, 1, 2};
Line Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

Line(5) = {6, 7};
Line(6) = {7, 8};
Line(7) = {8, 3};
Line(8) = {2, 6};
Line Loop(2) = {5, 6, 7, -1, 8};
Plane Surface(2) = {2};

Line(9) = {8, 9};
Line(10) = {9, 10};
Line(11) = {10, 4};
Line Loop(3) = {9, 10, 11, -2, -7};
Plane Surface(3) = {3};

Line(12) = {10, 11};
Line(13) = {11, 12};
Line(14) = {12, 5};
Line Loop(4) = {12, 13, 14, -3, -11};
Plane Surface(4) = {4};

Line(15) = {12, 13};
Line(16) = {13, 6};
Line Loop(5) = {15, 16, -8, -4, -14};
Plane Surface(5) = {5};

// Lower block
Point(14) = {0, 0, -H/2, lc0};
Point(15) = {-L/2, -L/2, -H/2, lc0};
Point(16) = {0, -L/2, -H/2, lc0};
Point(17) = {+L/2, -L/2, -H/2, lc0};
Point(18) = {+L/2, 0, -H/2, lc0};
Point(19) = {+L/2, +L/2, -H/2, lc0};
Point(20) = {0, +L/2, -H/2, lc0};
Point(21) = {-L/2, +L/2, -H/2, lc0};
Point(22) = {-L/2, 0, -H/2, lc0};

Line(17) = {15, 16};
Line(18) = {16, 14};
Line(19) = {14, 22};
Line(20) = {22, 15};
Line Loop(6) = {17, 18, 19, 20};
Plane Surface(6) = {6};

Line(21) = {16, 17};
Line(22) = {17, 18};
Line(23) = {18, 14};
Line Loop(7) = {21, 22, 23, -18};
Plane Surface(7) = {7};

Line(24) = {18, 19};
Line(25) = {19, 20};
Line(26) = {20, 14};
Line Loop(8) = {24, 25, 26, -23};
Plane Surface(8) = {8};

Line(27) = {20, 21};
Line(28) = {21, 22};
Line Loop(9) = {27, 28, -19, -26};
Plane Surface(9) = {9};

Line(29) = {7, 15};
Line(30) = {9, 17};
Line(31) = {11, 19};
Line(32) = {13, 21};
Line Loop(10) = {17, 21, -30, -9, -6, 29};
Plane Surface(10) = {10};
Line Loop(11) = {22, 24, -31, -12, -10, 30};
Plane Surface(11) = {11};
Line Loop(12) = {25, 27, -32, -15, -13, 31};
Plane Surface(12) = {12};
Line Loop(13) = {28, 20, -29, -5, -16, 32};
Plane Surface(13) = {13};

Surface Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
Volume(1) = {1};

// Upper block
Point(23) = {0, 0, +H/2, lc0};
Point(24) = {-L/2, -L/2, +H/2, lc0};
Point(25) = {0, -L/2, +H/2, lc0};
Point(26) = {+L/2, -L/2, +H/2, lc0};
Point(27) = {+L/2, 0, +H/2, lc0};
Point(28) = {+L/2, +L/2, +H/2, lc0};
Point(29) = {0, +L/2, +H/2, lc0};
Point(30) = {-L/2, +L/2, +H/2, lc0};
Point(31) = {-L/2, 0, +H/2, lc0};

Line(33) = {24, 25};
Line(34) = {25, 23};
Line(35) = {23, 31};
Line(36) = {31, 24};
Line Loop(14) = {33, 34, 35, 36};
Plane Surface(14) = {14};

Line(37) = {25, 26};
Line(38) = {26, 27};
Line(39) = {27, 23};
Line Loop(15) = {37, 38, 39, -34};
Plane Surface(15) = {15};

Line(40) = {27, 28};
Line(41) = {28, 29};
Line(42) = {29, 23};
Line Loop(16) = {40, 41, 42, -39};
Plane Surface(16) = {16};

Line(43) = {29, 30};
Line(44) = {30, 31};
Line Loop(17) = {43, 44, -35, -42};
Plane Surface(17) = {17};

Line(45) = {7, 24};
Line(46) = {9, 26};
Line(47) = {11, 28};
Line(48) = {13, 30};
Line Loop(18) = {6, 9, 46, -37, -33, -45};
Plane Surface(18) = {18};
Line Loop(19) = {10, 12, 47, -40, -38, -46};
Plane Surface(19) = {19};
Line Loop(20) = {13, 15, 48, -43, -41, -47};
Plane Surface(20) = {20};
Line Loop(21) = {16, 5, 45, -36, -44, -48};
Plane Surface(21) = {21};

Surface Loop(2) = {1, 2, 3, 4, 5, 14, 15, 16, 17, 18, 19, 20, 21};
Volume(2) = {2};

// Physical entities
Physical Volume(0) = {1};
Physical Volume(1) = {2};
Physical Surface("bottom") = {6, 7, 8, 9};
Physical Surface("right") = {11, 19};
Physical Surface("top") = {14, 15, 16, 17};
Physical Surface("left") = {13, 21};
Physical Surface("front") = {10, 18};
Physical Surface("back") = {12, 20};
Physical Surface("interface") = {1, 2, 3, 4, 5};
// Physical Line("no_disp_x") = {18, 26, 34, 42};
// Physical Line("no_disp_y") = {19, 23, 35, 39};
// Physical Line("no_disp_z") = {5, 16, 10, 12};
Physical Point("no_disp_x") = {16, 20, 25, 29};
Physical Point("no_disp_z") = {18, 22, 27, 31};
Physical Point("no_disp_y") = {6, 8, 10, 12};