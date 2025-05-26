lc = 0.4;

Point(1) = {-1.0, -0.5, 0.0, lc};
Point(2) = {0.0, -0.5, 0.0, lc};
Point(3) = {+1.0, -0.5, 0.0, lc};
Point(4) = {+1.0, 0.0, 0.0, lc};
Point(5) = {+1.0, +0.5, 0.0, lc};
Point(6) = {0.0, +0.5, 0.0, lc};
Point(7) = {-1.0, +0.5, 0.0, lc};
Point(8) = {-1.0, 0.0, 0.0, lc};
Point(9) = {0.0, 0.0, 0.0, lc};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};
Line(9) = {8, 9};
Line(10) = {9, 4};
Line Loop(1) = {1, 2, 3, -10, -9, 8};
Plane Surface(1) = {1};
Line Loop(2) = {9, 10, 4, 5, 6, 7};
Plane Surface(2) = {2};

Physical Surface(1) = {1, 2};
Physical Line("bottom") = {1, 2};
Physical Line("right") = {3, 4};
Physical Line("top") = {5, 6};
Physical Line("left") = {7, 8};
Physical Line("interface") = {9, 10};
Physical Point("no_disp_x") = {2, 6};
Physical Point("no_disp_y") = {4, 8};
