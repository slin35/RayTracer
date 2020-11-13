camera {
  location  <0, 0, 12>
  up        <0,  1,  0>
  right     <1.3333, 0,  0>
  look_at   <0, 0, 0>
}

sphere { 
   <0, 0, 0>, 0.5
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
  scale <2, 2, 2>
  translate <-4, 0 ,0>
}

sphere { 
   <0, 0, 0>, 0.5
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
  scale <2, 2, 2>
  rotate <20, 0, 0>
  translate <-2, 0 ,0>
}

sphere { 
   <0, 0, 0>, 0.5
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
    scale <2, 2, 2>
    rotate <40, 0, 0>
  translate <0, 0 ,0>
}

sphere { 
   <0, 0, 0>, 0.5
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
    scale <2, 2, 2>
  rotate <60, 0, 0>
  translate <2, 0 ,0>
}

sphere { 
   <0, 0, 0>, 0.5
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
    scale <2, 2, 2>
    rotate <80, 0, 0>
    translate <4, 0 ,0>
}

sphere { 
   <0, 0, 0>, 1
  pigment {
    image_map {jpg "../imgs/mosaic.jpg"}
  }
  scale <30, 30, 30>
  rotate <80, 0, 0>
  translate <0, -32 ,0>
}