lc = 0.4;

Point(1) = {-1.0, -0.5, 0.0, lc};
Point(2) = {+1.0, -0.5, 0.0, lc};
Point(3) = {+1.0, 0.0, 0.0, lc};
Point(4) = {+1.0, +0.5, 0.0, lc};
Point(5) = {-1.0, +0.5, 0.0, lc};
Point(6) = {-1.0, 0.0, 0.0, lc};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};
Line(7) = {3, 6};
Line Loop(1) = {1, 2, 7, 6};
Plane Surface(1) = {1};
Line Loop(2) = {-7, 3, 4, 5};
Plane Surface(2) = {2};

Physical Surface(1) = {1, 2};
Physical Line("bottom") = {1};
Physical Line("right") = {2, 3};
Physical Line("top") = {4};
Physical Line("left") = {5, 6};
Physical Line("interface") = {7};