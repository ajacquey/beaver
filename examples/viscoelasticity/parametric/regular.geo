size_coarse = 65;
size_fine = 4.5;

Lz = 660;
Ly = 450;

h = 205.189718;
H = 115.96607696095386;
R = 54.42210251952307;

Point(1) = {0.0, 0.0, 0.0, size_coarse};
Point(2) = {Ly, 0.0, 0.0, size_coarse};
Point(3) = {Ly, 0.0, Lz, size_coarse};
Point(4) = {0.0, 0.0, Lz, size_coarse};

Point(5) = {0.0, 0.0, h, size_fine};
Point(6) = {0.0, 0.0, h+R, size_coarse};
Point(7) = {0.0, 0.0, h+R+H, size_coarse};
Point(8) = {0.0, 0.0, h+R+H+R, size_fine};

Point(9) = {R, 0.0, h+R, size_fine};
Point(10) = {R, 0.0, h+R+H, size_fine};

Point(11) = {0, R, h+R+H, size_fine};
Point(12) = {0, R, h+R, size_fine};
Point(13) = {0, Ly, 0, size_coarse};
Point(14) = {0, Ly, Lz, size_coarse};

Point(15) = {Ly, Ly, 0, size_coarse};
Point(16) = {Ly, Ly, Lz, size_coarse};

Line(1) = {1, 2};
Line(2) = {2, 15};
Line(3) = {15, 13};
Line(4) = {13, 1};
Line(5) = {1, 5};
Line(6) = {8, 4};
Line(7) = {4, 3};
Line(8) = {3, 16};
Line(9) = {16, 14};
Line(10) = {14, 4};
Line(11) = {13, 14};
Line(12) = {15, 16};
Line(13) = {2, 3};
Line(14) = {9, 10};
Line(15) = {12, 11};
Circle(16) = {5, 6, 9};
Circle(17) = {5, 6, 12};
Circle(18) = {12, 6, 9};
Circle(19) = {11, 7, 10};
Circle(20) = {10, 7, 8};
Circle(21) = {8, 7, 11};

Curve Loop(1) = {16, -18, -17};
Surface(1) = {1};
Curve Loop(2) = {19, 20, 21};
Surface(2) = {2};
Curve Loop(3) = {14, -19, -15, 18};
Surface(3) = {3};
Curve Loop(4) = {5, 16, 14, 20, 6, 7, -13, -1};
Plane Surface(4) = {-4};
Curve Loop(5) = {5, 17, 15, -21, 6, -10, -11, 4};
Plane Surface(5) = {5};
Curve Loop(6) = {4, 1, 2, 3};
Plane Surface(6) = {-6};
Curve Loop(7) = {2, 12, -8, -13};
Plane Surface(7) = {7};
Curve Loop(8) = {12, 9, -11, -3};
Plane Surface(8) = {-8};
Curve Loop(9) = {9, 10, 7, 8};
Plane Surface(9) = {9};

Surface Loop(1) = {9, 8, 6, 7, 4, 5, 1, 3, 2};
Volume(1) = {1};

Physical Surface("Top", 22) = {9};
Physical Surface("South", 23) = {4};
Physical Surface("North", 24) = {8};
Physical Surface("East", 25) = {7};
Physical Surface("West", 26) = {5};
Physical Surface("Bottom", 27) = {6};
Physical Surface("Cavern", 29) = {3, 1, 2};
Physical Volume("Salt", 28) = {1};
Physical Curve("Wall_profile", 30) = {20, 14, 16};
