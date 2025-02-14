camera {
  location  <0, 0, 5>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {
    <0, 6, 0>
    color rgb <1.5, 1.5, 1.5>
    area_light <8, 0, 0>, <0, 0, 8>
  }


sphere { <0, -2, 0>, 0.5 
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {reflection 0.0}
}

plane {<0, 1, 0>, -2.5
      pigment {color rgb <0.8, 0.6, 0.2>}
}

plane {<0, 0, 1>, -6 
      pigment {color rgb <0.8, 0.3, 0.3>}
}

plane {<1, 0, 0>, -5
      pigment {color rgb <0.1, 0.1, 0.8>}
}

plane {<-1, 0, 0>, -5
      pigment {color rgb <0.0, 0.9, 0.0>}
}
