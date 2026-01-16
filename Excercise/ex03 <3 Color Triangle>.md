# _정점 배열에 3개의 부동 소수점 숫자로 색상데이터를 추가 후, 삼각형의 각 꼭지점에 각각 빨강,녹색,파랑 색상 할당_ 
1. **정점 배열에 색상 데이터 추가**
```cpp
float vertices[] = {
0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
-0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,
0.0f,0.5f,0.0f,0.0f,0.0f,1.0f
};
```
2. **버텍스 셰이더에서 색상 값을 버텍스 속성 입력으로 수신해야 함**
```cpp
#version 330 core
layout(location=0) in vec3 aPos;  //위치는 속성 위치 0 
layout(location=1) in vec3 aColor;  //색상은 속성 위치 1
out vec3 ourColor; //프래그먼트 셰이더에서 색상 출력변수

{
gl_Position = vec4(aPos,1.0); 
ourColor=aColor;  //aColor의 값을 ourColor에 넣음 
}
```
3. **VBO에 vertices 데이터 업데이트**
<img width="1037" height="306" alt="image" src="https://github.com/user-attachments/assets/ea8d6fea-90d9-42d0-8c8e-64ce1e6ff5b7" />
▲현재 VBO 메모리의 시각 구현
<br><br>

4. **glVertexAttribPointer 정점형식 업데이트**<br>
   
```cpp
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1,3,GL_FLOAT,GL_FLASE,6*sizeof(float),(void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);
```
💡```cppglVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);```<br>
```glVertexAttribPointer 함수``` : VBO에 저장된 데이터를 셰이더의 입력 변수에 연결하는 역할  
```index=0``` : 셰이더의 ```cpplayout(location=0)```->```aPos```에 연결  
```size = 3``` : x,y,z -> 3개의 flaot type
```type=GL_FLOAT``` : 데이터 타입은 FLOAT  
```nomalized = GL_FALSE``` : 정규화 X  
``` ⭐stride=6*sizeof(float)``` : 한 정점의 전체 크기 (위치 3 + 색상 3 = 6개 float = 24byte)  
```pointer = (void*)0``` :위치 데이터는 정점의 시작 위치에 있음 (offset=0)  
<br>  
**🎀해석해보기**<br>
```glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLASE, 6 * sizeof(float), (void*)(3 * sizeof(float)));```  
버텍스 쉐이더에의 location=1에 해당하는 변수에 연결, abc 3개, 데이터타입 float, 정규화 x, 한 정점의 전체크기 6, 색상 데이터는 위치 데이터 다음에 시작하니*3  
<br>  
<img width="592" height="526" alt="image" src="https://github.com/user-attachments/assets/b613fdbc-bf01-449c-9f0f-29b68c2e906b" />



