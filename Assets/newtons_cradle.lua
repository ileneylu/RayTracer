floor = gr.material({0.87, 0.87, 0.87}, {0.0, 0.0, 0.0}, 0, 0, 0)
wall = gr.material({0.87, 0.87, 0.87}, {0.0, 0.0, 0.0}, 0, 0, 0)
redWall = gr.material({0.65, 0.10, 0.10}, {0.0, 0.0, 0.0}, 0, 0, 0)
blueWall = gr.material({0.10, 0.10, 0.65}, {0.0, 0.0, 0.0}, 0, 0, 0)
obj1 = gr.material({0.1, 0.1, 0.1}, {0.6, 0.6, 0.2}, 0.5, 0, 0)
glossy = gr.material({0.1, 0.1, 0.1}, {0.6, 0.6, 0.6}, 0.8, 0, 0)
glass = gr.material({0.1, 0.1, 0.1}, {0.11, 0.38, 0.59}, 0.0, 0.0, 1.6)
color3 = gr.material({0.1, 0.1, 0.1}, {0.11, 0.38, 0.59}, 0.5, 0, 0)
perlin = gr.material({0.1, 0.1, 0.1}, {0.98, 0.8, 0.42}, 0.5, 0, 0)

scene = gr.node('scene')
-- scene:rotate('X', 23)

plane = gr.nh_box('plane', {0, 0, 0}, 1)
scene:add_child(plane)
plane:set_material(floor)
plane:scale(50, 10, 60)
plane:translate(-25, -18, -25)

backWall = gr.nh_box('backWall', {0, 0, 0}, 1)
scene:add_child(backWall)
backWall:set_material(wall)
backWall:scale(50, 40, 10)
backWall:translate(-25, -8, -30)

-- object = gr.cylinder('obj', 0.4)
-- Main Objects

support1 = gr.cylinder('obj', 0.4)
scene:add_child(support1)
support1:set_material(glossy)
support1:scale(0.1, 5, 0.1)
support1:rotate('X', 15)
support1:translate(-4.0, -2.5, -1.2)

support2 = gr.cylinder('obj', 0.4)
scene:add_child(support2)
support2:set_material(glossy)
support2:scale(0.1, 5, 0.1)
support2:rotate('X', -15)
support2:translate(-4.0, -2.5, 1.2)

support3 = gr.cylinder('obj', 0.4)
scene:add_child(support3)
support3:set_material(glossy)
support3:scale(0.1, 5, 0.1)
support3:rotate('X', 15)
support3:translate(4.0, -2.5, -1.2)

support4 = gr.cylinder('obj', 0.4)
scene:add_child(support4)
support4:set_material(glossy)
support4:scale(0.1, 5, 0.1)
support4:rotate('X', -15)
support4:translate(4.0, -2.5, 1.2)

support5 = gr.cylinder('obj', 0.4)
scene:add_child(support5)
support5:set_material(glossy)
support5:scale(0.1, 5, 0.1)
support5:rotate('Z', 90)
support5:translate(0, 1.5, 0)

ball1 = gr.nh_sphere('obj', {0, 0, 0}, 1)
scene:add_child(ball1)
ball1:set_material(color3)
ball1:scale(0.6, 0.6, 0.6)
ball1:translate(-2.4, -4.5, 0)

ball2 = gr.nh_sphere('obj', {0, 0, 0}, 1)
scene:add_child(ball2)
ball2:set_material(color3)
ball2:scale(0.6, 0.6, 0.6)
ball2:translate(-1.2, -4.5, 0)

ball3 = gr.nh_sphere('obj', {0, 0, 0}, 1)
scene:add_child(ball3)
ball3:set_material(color3)
ball3:scale(0.6, 0.6, 0.6)
ball3:translate(0, -4.5, 0)

ball4 = gr.nh_sphere('obj', {0, 0, 0}, 1)
scene:add_child(ball4)
ball4:set_material(color3)
-- ball4:set_movement({0.5, 0, 0}, {-0.1, 0, 0})
ball4:scale(0.6, 0.6, 0.6)
ball4:translate(1.2, -4.5, 0)

ball5 = gr.nh_sphere('obj', {0, 0, 0}, 1)
scene:add_child(ball5)
ball5:set_material(color3)
ball5:set_movement({-0.5, -0.2, 0}, {0.1, 0.04, 0})
ball5:scale(0.6, 0.6, 0.6)
ball5:translate(3.4, -4.2, 0)


gr.render(scene,
	  'nc.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},
    {
    gr.light({200.0, 100.0, 250.0},  {0, 0, 0}, {0.9, 0.9, 0.9}, {1.4, 0, 0})
    })
