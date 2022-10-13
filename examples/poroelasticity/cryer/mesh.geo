r = 1.0;
lc = 0.05;

Point(1) = {0, 0, 0, lc};
Point(2) = {r, 0, 0, lc};
Point(3) = {0, r, 0, lc};
Point(4) = {0, 0, r, lc};

Line(1) = {1, 2};
Line(2) = {1, 3};
Line(3) = {1, 4};
Circle(4) = {2, 1, 3};
Circle(5) = {2, 1, 4};
Circle(6) = {3, 1, 4};

Line Loop(1) = {1, 4, -2};
Plane Surface(1) = {1};
Line Loop(2) = {2, 6, -3};
Plane Surface(2) = {2};
Line Loop(3) = {1, 5, -3};
Plane Surface(3) = {3};
Line Loop(4) = {4, 6, -5};
Ruled Surface(4) = {4};

Surface Loop(1) = {1, 2, 3, 4};
Volume(1) = {1};

Physical Volume(0) = {1};
Physical Surface("bottom") = {1};
Physical Surface("left") = {2};
Physical Surface("back") = {3};
Physical Surface("out") = {4};