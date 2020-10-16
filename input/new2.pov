camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 14, 0> color rgb <1.0, 1.0, 1.0>}

sphere { <-1.5, 1.5, 0>, 0.5
  pigment { color rgb <0, 0, 0>}
}

sphere { <2.5, 1.5, 0>, 0.5
  pigment { color rgb <0, 0, 0>}
}

sphere { <0, 0, 0>, 0.8
  pigment { color rgb <1.0, 0, 0>}
}

sphere { <-2, 1.5, 0>, 1
  pigment { color rgb <1.0, 1.0, 1.0>}
}

sphere { <2, 1.5, 0>, 1
  pigment { color rgb <1.0, 1.0, 1.0>}
}

sphere { <0, 0, 0>, 4
  pigment { color rgb <0.0, 1.0, 0.0>}
}

plane {<0, 1, 0>, 2
      pigment {color rgb <0.5, 0.5, 0.5>}
}