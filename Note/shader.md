# _Vertex Shader_  
```cpp
#version 330 core   //GLSL 버전 3.30 사용
layout (location =0) in vec3 aPos;  //정점 데이터에서 위치정보를 받아오는 입력 변수 : attrib index0번
out vec4 vertexColor; //버텍스 쉐이더에서 계산한 색상을 프래그먼트 쉐이더로 전달하기 위한 출력변수 

void main()
{
  gl_Position = vec4(aPos, 1.0);  //입력된 정점좌표(aPos)를 클립공간 좌표로 변환해 GPU파이프라인에 넘김
  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); //짙은 빨간색 색상 지정
}
```
💡**왜 색상을 버텍스 쉐이더에서 정해서 프래그먼트 쉐이더로 전달할까?**  

  **버텍스 쉐이더에서 색상 지정** -> 정점별 색상 -> __GPU가 자동 보간__ -> 프래그먼트 쉐이더에서 픽셀별 색상 출력
  **프래그먼트 쉐이더에서 색상 지정** -> 모든 픽셀이 동일한 색상 -> 단색 도형 

# _Fragment Shader_
```cpp
#version 330 core
out vec4 FragColor; //최종색상 출력변수, 이 값이 화면 픽셀 색으로 렌더링됨. 
in vec4 vertexColor; //버텍스 쉐이더에서 계산된 색상 값을 입력으로 받음

void main()
{
  FragColor = vertexColor; //전달받은 색상을 그대로 픽셀 색상으로 출력
}
```
<br><br>
**⭐Pipe Line**  

<img width="910" height="500" alt="image" src="https://github.com/user-attachments/assets/b2547f0f-4c87-4ee1-b5e9-1435f671b388" />
