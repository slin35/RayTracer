camera {
  location  <0, 0, -10>
  up        <0,  1,  0>
  right     <0, 0, 1.33333>
  look_at   <0, 0, 0>
}

light_source {<-12, 50, -100> color rgb <1.0, 1.0, 1.0>}

// Sun
sphere { <60, 25, 200>, 40
  pigment { color rgb <1, 1, 0>}
}

// Right hill
sphere { <-20, -46, 50>, 50.0
  pigment { color rgb <0.5, 1.0, 0.25>}
}

// Left hill
sphere { <30, -65, 80>, 75.0
  pigment { color rgb <0.5, 1.0, 0.25>}
}

// Snow patches
sphere { <8, -8.5, 10>, 5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <-6, -8.5, 5>, 5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <-12, -8.5, 20>, 5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <20, -8.5, 28>, 5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <8, -8.5, 32>, 5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <1, -8.5, -1>, 4.7
  pigment { color rgb <1.0, 1.0, 1.0>}
}

// Snowman
sphere { <0, -2, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <0, 1, 0>, 1.5
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <0, 3, 0>, 1
  pigment { color rgb <1.0, 1.0, 1.0>}
}
sphere { <0.4, 3, -1>, 0.1
  pigment { color rgb <0.0, 0.0, 0.0>}
}
sphere { <-0.4, 3, -1>, 0.1
  pigment { color rgb <0.0, 0.0, 0.0>}
}

// Ground
plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.9, 0.2>}
}

// Sky
plane {<0, 1, 1>, 250
  pigment {color rgb <0.4, 0.6, 0.7> }
}