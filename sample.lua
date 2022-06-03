
table = gr.material({0.44, 0.44, 0.44}, {0.2, 0.2, 0.2}, 0.2)
model = gr.material({0.69, 0.19, 0.4}, {0.5, 0.7, 0.5}, 25)

scene = gr.node('scene')
scene:scale(1.1, 1.1, 1.1)
scene:rotate('X', 15)

plane = gr.cube( 'building1', 'cube.obj' )
scene:add_child(plane)
plane:set_material(table)
plane:scale(20, 15, 15)
plane:translate(-4, -15, -2)

plane2 = gr.cube( 'building2', 'cube.obj' )
scene:add_child(plane2)
plane2:set_material(table)
plane2:scale(15, 25, 15)
plane2:translate(-25, -20, -5)

cube1 = gr.cube( 'cube1', 'cube.obj' )
scene:add_child(cube1)
cube1:set_material(model)
cube1:scale(1, 4, 1)
cube1:rotate('Y', 60)
cube1:translate(5, 0, 5)

cube2 = gr.cube( 'cube2', 'cube.obj' )
scene:add_child(cube2)
cube2:set_material(model)
cube2:scale(4, 1, 1)
cube2:rotate('X', 45)
cube2:rotate('Y', 15)
cube2:translate(3.6, 2, 4.6)

sphere1 = gr.nh_sphere('s', {0, 0, 0}, 1)
scene:add_child(sphere1)
sphere1:set_material(gr.material({0.78, 0.43, 0.05}, {0.5, 0.7, 0.5}, 25))
sphere1:scale(10, 10, 10)
sphere1:translate(20, 4, 15)

sphere2 = gr.sphere('s1', 'sphere.obj')
scene:add_child(sphere2)
sphere2:set_material(gr.material({0.78, 0.43, 0.05}, {0.5, 0.7, 0.5}, 25))
sphere2:scale(1, 1, 1)
sphere2:translate(3, 1.4, 11)

sphere2Child = gr.cube( 'sphere2Child', 'cube.obj' )
sphere2:add_child(sphere2Child)
sphere2Child:set_material(gr.material({0.33, 0.42, 0.18}, {0.5, 0.7, 0.5}, 25))
sphere2Child:rotate('Y', 20)
sphere2Child:translate(-4, -1.2, 0)

buckyball = gr.mesh( 'buckyball', 'buckyball.obj' )
scene:add_child(buckyball)
buckyball:set_material(gr.material({0.31, 0.45, 0.62}, {0.5, 0.7, 0.5}, 25))
buckyball:scale(1.2, 1.2, 1.2)

buckyball:translate(-2, 1.4, 2)

sphere3 = gr.nh_sphere('s1', {0, 0, 0}, 1)
scene:add_child(sphere3)
sphere3:set_material(gr.material({0.31, 0.65, 0.76}, {0.5, 0.7, 0.5}, 25))
sphere3:scale(1.5, 1.5, 1.5)
sphere3:translate(7, 11, 0)



gr.render(scene,
	  'sample.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.2, 0.2, 0.2}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}),
                      gr.light({-300, 100, 150}, {0.56, 0.37, 0.6}, {1, 0, 0})})
