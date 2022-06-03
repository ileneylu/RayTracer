floor = gr.material({0.87, 0.87, 0.87}, {0.0, 0.0, 0.0}, 0, 0, 0)
wall = gr.material({0.87, 0.87, 0.87}, {0.0, 0.0, 0.0}, 0, 0, 0)
redWall = gr.material({0.65, 0.10, 0.10}, {0.0, 0.0, 0.0}, 0, 0, 0)
blueWall = gr.material({0.10, 0.10, 0.65}, {0.0, 0.0, 0.0}, 0, 0, 0)
obj1 = gr.material({0.1, 0.1, 0.1}, {0.2, 0.6, 0.2}, 0.5, 0, 0)
obj2 = gr.material({0.1, 0.1, 0.1}, {0.2, 0.6, 0.2}, 0.0, 0.0, 1.6)

scene = gr.node('scene')
-- scene:rotate('X', 23)

plane = gr.nh_box('plane', {0, 0, 0}, 1)
scene:add_child(plane)
plane:set_material(floor)
plane:scale(30, 10, 60)
plane:translate(-15, -18, -15)

rightWall = gr.nh_box('rightWall', {0, 0, 0}, 1)
scene:add_child(rightWall)
rightWall:set_material(blueWall)
rightWall:scale(10, 30, 60)
rightWall:translate(10, -8, -15)

leftWall = gr.nh_box('leftWall', {0, 0, 0}, 1)
scene:add_child(leftWall)
leftWall:set_material(redWall)
leftWall:scale(10, 30, 60)
leftWall:translate(-20, -8, -15)

backWall = gr.nh_box('backWall', {0, 0, 0}, 1)
scene:add_child(backWall)
backWall:set_material(wall)
backWall:scale(30, 30, 10)
backWall:translate(-15, -8, -25)

ceiling = gr.nh_box('ceiling', {0, 0, 0}, 1)
scene:add_child(ceiling)
ceiling:set_material(wall)
ceiling:scale(30, 10, 60)
ceiling:translate(-15, 15, -15)

frontWall = gr.nh_box('frontWall', {0, 0, 0}, 1)
scene:add_child(frontWall)
frontWall:set_material(wall)
frontWall:scale(30, 30, 10)
frontWall:translate(-15, -8, 40)

-- Main Objects

object = gr.nh_sphere('obj', {0, 0, 0}, 1)
-- object = gr.cylinder('obj', 0.4)
-- object = gr.nh_box('obj', {0, 0, 0}, 1)
-- object = gr.torus('obj', 0.5)
scene:add_child(object)
object:set_material(obj1)
-- object:set_texture_map('saturnmap')
object:set_bump_map('bump')
object:scale(3, 3, 3)
-- object:rotate('Z', 45)
-- object:rotate('X', 45)
object:translate(0, -4.5, 2)

-- object1 = gr.nh_sphere('obj', {0, 0, 0}, 1)
-- scene:add_child(object1)
-- object1:set_material(obj2)
-- object1:scale(3, 3, 3)
-- object1:translate(-3, -4.5, -7)


gr.render(scene,
	  'room.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},
    {
    -- gr.light({0, 0, 15}, {0, 0, 0},{1.0, 1.0, 1.0}, {1, 0.001, 0.002}),
    gr.light({-9.95, 7, 15}, {0, 1.6, 1.6}, {1.0, 1.0, 1.0}, {0.8, 0.001, 0.008}),
    gr.light({9.95, 0, 15}, {0, 1.6, 1.6}, {1.0, 1.0, 1.0}, {0, 0.001, 0.008})
    })
