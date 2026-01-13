## Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data

float vertices[] = {
	0.1f, 0.0f, 0.0f,
	-0.1f, 0.0f, 0.0f,
	0.1f, 0.1f, 0.0f,

	0.5f,0.0f,0.0f,
	0.3f,0.0f,0.0f,
	0.5f,0.5f,0.0f
};
...
while(!glfwWindowShouldClose(window))
{...
glDrawArrays(GL_TRIANGLES, 0, 3);
glDrawArrays(GL_TRIANGLES, 9, 3);
...
}   -> X

Why?
GL_TRIANGLES
glDrawArrays(GL_TRIANGLE,first,count)

