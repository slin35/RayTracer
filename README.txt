Added:
this is for (b)
- added a hit class to keep track of all hits for the closest hit
- get rid of extra parameters and passing in pointers instead for a boost of performance for foggy and phong

Acknowledgement:
Thanks for the help from everyone in my lab group!!!
(Nathan Irwin, Ziyi Wang, William O'Brien, Justin Privitera)


implemented:
- phong diffuse
- shadow with point light (added epsilon for shadow acne)
- foggy day diffsue
- anti-aliasing and gamma correction
- plane intersections

notes:
- added CMakeLists.txt
- run.sh is for building cmake and running the executable