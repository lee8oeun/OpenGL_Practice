## Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data

 ```cpp
 float vertices[] = {
	0.1f, 0.0f, 0.0f,
	-0.1f, 0.0f, 0.0f,
	0.1f, 0.1f, 0.0f,

	0.5f,0.0f,0.0f,
	0.3f,0.0f,0.0f,
	0.5f,0.1f,0.0f
};
...
while(!glfwWindowShouldClose(window))
{...
glDrawArrays(GL_TRIANGLES, 0, 3);
glDrawArrays(GL_TRIANGLES, 9, 3);
...
}
```
-> X (first triangle generate but, second triangle not generate)

Why?

**GL_TRIANGLES**
-> ***This mode renders 3 vertices grouped into a single triangle.***<br>*🌟vertices : (x,y,z) -> 3 float data*  

```cpp
glDrawArrays(mode,first,count);
```
mode : triangle,lines etc..
first : Index of the first vertex to use
count : Amount of total vertices to draw  

glDrawArrays(GL_TRIANGLES,9,3)<br>-> Draw a triangle by reading three vertices from index 9 of the vertex array.  

```cpp
float vertices[] = {
	0.1f, 0.0f, 0.0f,  //v0
	-0.1f, 0.0f, 0.0f,	//v1
	0.1f, 0.1f, 0.0f,  //v2

	0.5f,0.0f,0.0f,  //v3
	0.3f,0.0f,0.0f,  //v4
	0.5f,0.1f,0.0f  //v5
};
```
have only 6 vertices. -> index[0~5]

**🌟Matter : first=9 : not exist index[9]**<br><br>  
**So, then how to modify?**  
1.
```cpp
glDrawArrays(GL_TRIANGLES,0,3);
glDrawArrays(GL_TRIANGLES,3,3);
```
2.<br>
```cpp
glDrawArrays(GL_TRIANGLES,0,6);
```
