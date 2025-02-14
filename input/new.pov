camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 14, 0> color rgb <1.0, 1.0, 1.0>}


sphere { <-1, 0, 0>, 2
  pigment { color rgb <0.0, 0.9, 0.0>}
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.11, 0.67, 0.83>}
}

sphere { <-3, 0, 0>, 2
  pigment { color rgb <0.8, 0.3, 0.3>}
}

plane {<0, 1, 0>, -2
      pigment {color rgb <1.0, 1.0, 1.0>}
}