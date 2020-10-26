implemented:
- triangle intersections
- working reflection
- area lights
- AABB intersection
- multi-threading

notes:
- sphere pov format requires a separation between the center vector and radius
- run.sh is for building cmake and running the executable


timing:
--------------------------------------------------------------------
p3_e4.pov with foggy mode = 1, aa = 20, depth = 50, and bunny.obj

real    22m49.854s
user    177m35.981s
sys     0m9.879s
--------------------------------------------------------------------
p3_e4.pov with foggy mode = 1, aa = 100, depth = 50

real    2m53.571s
user    22m40.561s
sys     0m1.912s
--------------------------------------------------------------------
p3_simp.pov with phong mode = 0, aa = 20, depth = 50, and bunny.obj

real    2m14.139s
user    17m7.031s
sys     0m1.562s
--------------------------------------------------------------------
p3_simp.pov with phong mode = 0, aa = 100, depth = 50, and icoNoNormals.obj

real    0m17.246s
user    2m4.562s
sys     0m0.614s
--------------------------------------------------------------------
p3_e3.pov with foggy mode = 1, aa = 100, depth = 50

real    0m15.269s
user    1m55.831s
sys     0m1.126s
--------------------------------------------------------------------
p3_e2-1.pov with foggy mode = 1, aa = 100, depth = 50

real    0m16.224s
user    2m2.931s
sys     0m1.321s
--------------------------------------------------------------------
p3_e1.pov with foggy mode = 1, aa = 100, depth = 50

real    0m10.831s
user    1m18.439s
sys     0m1.174s
--------------------------------------------------------------------
P3_r3.pov with foggy mode = 1, aa = 100, depth = 50

real    0m8.348s
user    1m1.907s
sys     0m1.179s
--------------------------------------------------------------------
P3_r2.pov with foggy mode = 1, aa = 100, depth = 50

real    0m9.178s
user    1m3.257s
sys     0m1.036s
--------------------------------------------------------------------
P3_r1.pov with foggy mode = 1, aa = 100, depth = 50

real    0m8.687s
user    1m3.138s
sys     0m1.195s