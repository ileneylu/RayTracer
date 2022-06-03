floor = gr.material({0.6, 0.6, 0.6}, {0.7, 0.7, 0.7}, 0, 0, 0)
wall = gr.material({0.87, 0.87, 0.87}, {0.0, 0.0, 0.0}, 0, 0, 0)
bareModel = gr.material({0.4, 0.4, 0.4}, {0.7, 0.7, 0.7}, 0, 0, 0)
completeReflection = gr.material({0.1, 0.1, 0.1}, {1.0, 1.0, 1.0}, 0, 1, 0)
glossyReflection = gr.material({0.1, 0.1, 0.1}, {0.4, 0.4, 0.4}, 0.8, 0, 0)
obj2 = gr.material({0.1, 0.1, 0.1}, {0.11, 0.38, 0.59}, 0.0, 0.0, 1.6)
color3 = gr.material({0.1, 0.1, 0.1}, {0.11, 0.38, 0.59}, 0.5, 0, 0)
perlin = gr.material({0.1, 0.1, 0.1}, {0.98, 0.8, 0.42}, 0.5, 0, 0)
ring = gr.material({0.1, 0.1, 0.1}, {0.88, 0.85, 0.78}, 0, 0, 0)

scene = gr.node('scene')
-- scene:rotate('X', 23)

plane = gr.nh_box('plane', {0, 0, 0}, 1)
scene:add_child(plane)
plane:set_material(floor)
plane:set_texture_map('marble')
plane:scale(40, 10, 40)
plane:translate(-20, -18, -25)

rightWall = gr.nh_box('rightWall', {0, 0, 0}, 1)
scene:add_child(rightWall)
rightWall:set_material(wall)
rightWall:scale(10, 40, 300)
rightWall:translate(15, -8, -25)

backWall = gr.nh_box('backWall', {0, 0, 0}, 1)
scene:add_child(backWall)
backWall:set_material(wall)
backWall:scale(40, 40, 10)
backWall:translate(-20, -8, -30)

frontWall = gr.nh_box('frontWall', {0, 0, 0}, 1)
scene:add_child(frontWall)
frontWall:set_material(wall)
frontWall:scale(40, 40, 10)
frontWall:translate(-20, -8, 50)

-- mirrors

mirror1 = gr.nh_box('mirror1', {0, 0, 0}, 1)
scene:add_child(mirror1)
mirror1:set_material(completeReflection)
mirror1:scale(5.5, 5.5, 1)
mirror1:translate(2, 5, -20)

mirror2 = gr.nh_box('mirror2', {0, 0, 0}, 1)
scene:add_child(mirror2)
mirror2:set_material(completeReflection)
mirror2:scale(5.5, 5.5, 1)
mirror2:translate(-8, 5, -20)

mirror3 = gr.nh_box('mirror3', {0, 0, 0}, 1)
scene:add_child(mirror3)
mirror3:set_material(completeReflection)
mirror3:scale(5.5, 5.5, 1)
mirror3:translate(-18, 5, -20)

-- planet models

earth = gr.nh_sphere('earth', {0, 0, 0}, 1)
scene:add_child(earth)
earth:set_material(bareModel)
earth:scale(4, 4, 4)
earth:set_texture_map('earthmap')
earth:translate(-9, 3, -8)

earthSupport1 = gr.cylinder('earthSupport1', 0.4)
scene:add_child(earthSupport1)
earthSupport1:set_material(glossyReflection)
earthSupport1:scale(0.1, 8, 0.1)
earthSupport1:translate(-9.1, -6.8, -8)

earthSupport2 = gr.cylinder('earthSupport2', 0.4)
scene:add_child(earthSupport2)
earthSupport2:set_material(glossyReflection)
earthSupport2:scale(2.5, 0.3, 2.5)
earthSupport2:translate(-9, -8, -8)

saturn = gr.nh_sphere('saturn', {0, 0, 0}, 1)
scene:add_child(saturn)
saturn:set_material(bareModel)
saturn:scale(4, 4, 4)
saturn:set_texture_map('saturnmap')
saturn:translate(0, 2, 0)

saturnRing = gr.torus('ring', 0.15)
scene:add_child(saturnRing)
saturnRing:set_material(ring)
saturnRing:scale(6, 1.5, 6)
saturnRing:rotate('Z', 5)
saturnRing:translate(-0.3, 1.8, 0)

saturnSupport1 = gr.cylinder('saturnSupport1', 0.4)
scene:add_child(saturnSupport1)
saturnSupport1:set_material(glossyReflection)
saturnSupport1:scale(0.1, 6, 0.1)
saturnSupport1:translate(0, -6.8, 0)

saturnSupport2 = gr.cylinder('saturnSupport2', 0.4)
scene:add_child(saturnSupport2)
saturnSupport2:set_material(glossyReflection)
saturnSupport2:scale(2.5, 0.3, 2.5)
saturnSupport2:translate(0.1, -8, 0)

-- newton's cradle

-- ncBox = gr.nh_box('ncBox', {0, 0, 0}, 1)
-- scene:add_child(ncBox)
-- ncBox:set_material(color3)
-- ncBox:scale(8, 3, 8)
-- ncBox:translate(5, -8, -12)

-- ncSupport1 = gr.nh_box('ncSupport1', {0, 0, 0}, 1)
-- scene:add_child(ncSupport1)
-- ncSupport1:set_material(glossyReflection)
-- ncSupport1:scale(0.1, 3, 0.1)
-- ncSupport1:translate(3, -5, -8)
-- ncSupport1:rotate('X', 20)

-- ncSupport2 = gr.nh_box('ncSupport2', {0, 0, 0}, 1)
-- scene:add_child(ncSupport2)
-- ncSupport2:set_material(glossyReflection)
-- ncSupport2:scale(0.1, 3, 0.1)
-- ncSupport2:translate(3, -5, -7.5)
-- ncSupport2:rotate('X', -20)


-- ncSupport3 = gr.nh_box('ncSupport3', {0, 0, 0}, 1)
-- scene:add_child(ncSupport3)
-- ncSupport3:set_material(glossyReflection)
-- ncSupport3:scale(0.1, 3, 0.1)
-- ncSupport3:translate(8, -2, -8)
-- ncSupport3:rotate('X', 20)

-- ncSupport4 = gr.nh_box('ncSupport4', {0, 0, 0}, 1)
-- scene:add_child(ncSupport4)
-- ncSupport4:set_material(glossyReflection)
-- ncSupport4:scale(0.1, 3, 0.1)
-- ncSupport4:translate(8, -2, -7.5)
-- ncSupport4:rotate('X', -20)

-- ncSupport5 = gr.nh_box('ncSupport5', {0, 0, 0}, 1)
-- scene:add_child(ncSupport5)
-- ncSupport5:set_material(glossyReflection)
-- ncSupport5:scale(5, 0.1, 0.1)
-- ncSupport5:translate(3, 4, -7.5)


-- balls
-- ncball1 = gr.nh_sphere('obj', {0, 0, 0}, 1)
-- scene:add_child(ncball1)
-- ncball1:set_material(perlin)
-- ncball1:translate(10, -4, -6.5)


gr.render(scene,
	  'final_scene.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5},
    {
    gr.light({-200.0, 100.0, 250.0},  {0, 0, 0}, {0.9, 0.9, 0.9}, {2.5, 0, 0}),
		-- gr.light({14.0, 10.0, -19.0},  {0, 0, 0}, {0.9, 0.9, 0.9}, {2, 0.1, 0}),
    })
