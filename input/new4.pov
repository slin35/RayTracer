camera {
  location  <0, 0, 15>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 0, 50> color rgb <1.0, 1.0, 1.0>}


sphere { <0, 0, 0>, 6
  pigment { color rgb <0.9686, 0.88627,0.08235>}
}


sphere { <2, 2, 4>, 1.5
  pigment { color rgb <1, 1, 1>}
}


sphere { <-2, 2, 4>, 1.5
  pigment { color rgb <1, 1, 1>}
}


sphere { <2, -.65, 1>, 4.5
  pigment { color rgb <0.87450, 0.65806, 0.03921>}
}


sphere { <-2, -.65, 1>, 4.5
  pigment { color rgb <0.87450, 0.65806, 0.03921>}
}


sphere { <2.3, 2.3, 5.5>, .5
  pigment { color rgb <0, 0, 0>}
}


sphere { <-2.3, 2.3, 5.5>, .5
  pigment { color rgb <0, 0, 0>}
}

sphere { <0, -.5, 5>, 2.5
  pigment { color rgb <0.317647, 0.184313, 0.09803>}
}

plane { <0,0,1>, 0
  pigment { color rgb <0, 1, 0>}
}
