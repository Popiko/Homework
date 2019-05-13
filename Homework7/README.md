## Homework7

### Basic

1. 实现方向光源的Shadowing Mapping - Preparation

   - 绘制平面
     ```c++
     // 平面顶点坐标
     float planeVertices[] = {
     	// positions            // normals         // texcoords
     	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
     	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
     	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     
     	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
     	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     	 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
     };
     // plane VAO
     unsigned int planeVBO;
     glGenVertexArrays(1, &planeVAO);
     glGenBuffers(1, &planeVBO);
     glBindVertexArray(planeVAO);
     glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
     glBindVertexArray(0);
     
     ```

   - 绘制正方体

     ```c++
     void renderCube()
     {
     	// 初始化
     	if (cubeVAO == 0)
     	{
     		float vertices[] = {
     			// back face
     			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
     			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
     			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
     			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
     			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
     			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
     			// front face
     			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
     			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
     			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
     			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
     			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     			// left face
     			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
     			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
     			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
     			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
     			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
     			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
     			// right face
     			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
     			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
     			// bottom face
     			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
     			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
     			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
     			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
     			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
     			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
     			// top face
     			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
     			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
     			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
     			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
     			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
     			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
     		};
     		glGenVertexArrays(1, &cubeVAO);
     		glGenBuffers(1, &cubeVBO);
     
     		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
     		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     
     		glBindVertexArray(cubeVAO);
     		glEnableVertexAttribArray(0);
     		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
     		glEnableVertexAttribArray(1);
     		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
     		glEnableVertexAttribArray(2);
     		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
     		glBindBuffer(GL_ARRAY_BUFFER, 0);
     		glBindVertexArray(0);
     	}
     	
     	glBindVertexArray(cubeVAO);
     	glDrawArrays(GL_TRIANGLES, 0, 36);
     	glBindVertexArray(0);
     }
     ```

2. Shadowing Mapping 算法

   这个算法的思想就是先以光源的角度去渲染场景，会得到在光源角度下各个点的深度值。把这些深度值先储存到texture里面。之后再按正常步骤渲染场景，然后把每个点再转到光源视角的坐标系下，得到深度值，再把这个深度值和之前储存再texture里面的深度值进行比较。如果该点深度值大于储存在texture里的深度值，则证明该点处于阴影中。

   ![img](http://learnopengl.com/img/advanced-lighting/shadow_mapping_theory_spaces.png)

   - 以光源的视角渲染场景，得到深度图

     - 计算变换矩阵

       首先，我们与之前做摄像机类似，先根据光源所在的位置，指向的方向以及一个向上的向量计算出view变换矩阵。

       ```c++
       lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
       ```

       选择投影方式，计算出投影矩阵。这里暂时使用正交投影

       ```c++
       lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
       ```

       两个矩阵相乘，得到一个将坐标从世界坐标系变换到光源视角的坐标系的矩阵

       ```c++
       lightSpaceMatrix = lightProjection * lightView;
       ```

     - 生成深度图

       为渲染的深度贴图创建一个帧缓冲对象

       ```c++
       unsigned int depthMapFBO;
       glGenFramebuffers(1, &depthMapFBO);
       ```

       创建2D纹理

       ```c++
       unsigned int depthMap;
       glGenTextures(1, &depthMap);
       glBindTexture(GL_TEXTURE_2D, depthMap);
       
       // 只关心深度值，所以把纹理格式设置为GL_DEPTH_COMPONENT
       glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
       glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
       
       // 绑定buffer
       glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
       glDrawBuffer(GL_NONE); // 不使用颜色数据进行渲染
       glReadBuffer(GL_NONE);
       glBindFramebuffer(GL_FRAMEBUFFER, 0);
       ```

     - 使用深度图渲染场景

       ```c++
       glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
       glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
       glClear(GL_DEPTH_BUFFER_BIT);
       renderScene(simpleDepthShader);
       ```

   - 以camera视角渲染场景，使用Shadowing Mapping算法(比较当前深度值与在DepthMap Texture的深度值)，决定某个点是否在阴影下。

     检查片元是否在阴影下。该步骤在着色器下进行。

     ① 在顶点着色器中，利用之前算出来的变换矩阵将顶点转换到光空间

     ```c++
     vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
     ```

     ② 在片段着色器中，通过判断片元深度值与texture存储的深度值的大小关系，决定渲染的颜色。

     透视除法

     ```c++
     vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
     ```

      将片元深度变换到[0,1]之间

     ```c++
     projCoords = projCoords * 0.5 + 0.5;
     ```

       比较深度

     ```c++
     float closestDepth = texture(shadowMap, projCoords.xy).r;
     float currentDepth = projCoords.z;
     shadow = currentDepth  > closestDepth ? 1.0 : 0.0;
     ```

      计算渲染的颜色

     ```c++
     vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
     ```

   - 效果

     ![image_3.png](https://i.loli.net/2019/05/13/5cd8da14026c665046.png)


### Bonus

1. 实现光源在正交/透视两种投影下的Shadowing Mapping 

   只需要改变投影矩阵即可

   - 正交

     ```c++
     lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
     ```

     ![image_3.png](https://i.loli.net/2019/05/13/5cd8da14026c665046.png)

   - 透视

     ```c++
     lightProjection = glm::perspective(glm::radians(75.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
     ```

     ![image_4.png](https://i.loli.net/2019/05/13/5cd8da9d089d382743.png)

2. 优化Shadowing Mapping

   - 添加 bias 来解决阴影失真

     ![img](http://learnopengl.com/img/advanced-lighting/shadow_mapping_acne_diagram.png)

     由于深度贴图的分辨率比较小，真实场景的多个像素会从深度贴图的同一个点取深度进行比较，就可能会造成比较的不正确而导致一些本该被点亮的像素被置为阴影中。

     ![img](https://pic3.zhimg.com/80/c7edcc2c3faa147e2e7e3989a3e26cb6_hd.jpg)

     由于深度贴图分辨率较小，场景中的四个像素都从中间的黑点（对应深度贴图中一个像素）取深度值。

     ![img](https://pic4.zhimg.com/80/88ffd93a4fa8cc66d31ae25bf2d00366_hd.jpg)

     之后进行深度比较。正常情况下四个像素都要被点亮，但由于深度值是中间黑点对应的像素并且是共用的，会造成其中靠右边的b、c像素因深度值大于深度图中存储的深度值而被渲染成阴影，从而造成阴影失真。

     图片来自<https://www.zhihu.com/question/49090321>

     可以通过添加一个bias来消除这种现象。我们可以使用表面朝向光线的角度计算偏移量。

     ```c++
     float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
     ```

     ![image_5.png](https://i.loli.net/2019/05/13/5cd8db4d8d5ea44977.png)

   - 悬浮（Peter Panning）

     ![image_6.png](https://i.loli.net/2019/05/13/5cd8dbcb861f414999.png)

     使用正面剔除

     ![img](http://learnopengl.com/img/advanced-lighting/shadow_mapping_culling.png)

     ```c++
     if (front_culling_flag) {
     	glCullFace(GL_FRONT);
     }
     renderScene(simpleDepthShader);
     glCullFace(GL_BACK);
     ```

     但是这个方法在作业里效果并不明显，最好是设置好bias值来避免阴影悬浮的问题。

   - 采样过多

     因为超出光的视锥的投影坐标比1.0大，采样的深度纹理就会超出默认的0到1的范围，所以光的视锥之外的区域被视作处于阴影之中。

     我们可以把所有超出深度贴图的坐标的深度设为1.0，这样超出的坐标就不会被处于阴影之中。

     储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER

     ```c++
     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
     GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
     glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
     ```

     当一个点比光的远平面还要远时，它的z值也是大于1.0的。这个时候我们可以在片段着色器里面修改。若z值大于1.0，我们就知道它是远处的点，就直接把shadow值设置为0，表示它不在阴影下。

     ```c++
     if(over_sample_flag == 1){
     	if(projCoords.z > 1.0)
     		shadow = 0.0;
     }
     ```

     ![image_7.png](https://i.loli.net/2019/05/13/5cd8dce1de63778791.png)

   - 使用PCF减小锯齿化程度

     基本思想是从纹理像素四周对深度贴图采样，然后把结果平均起来。

     ```c++
     float shadow = 0.0;
     if(pcf_flag == 1){
     	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
     	for(int x = -1; x <= 1; ++x)
     	{
     		for(int y = -1; y <= 1; ++y)
     		{
     			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
     			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
     		}    
     	}
     	shadow /= 9.0;
     }
     ```

     ![image_8.png](https://i.loli.net/2019/05/13/5cd8ddf04fc6922516.png)

   - 使用CSM（Cascaded Shadow Maps）缓解近度锯齿

     

     ![img](https://i-msdn.sec.s-msft.com/dynimg/IC340447.jpg)

     由于分辨率的问题，可能会有多个像素同时使用深度图里面的一个深度，所以会产生锯齿，在近处这个问题会更加明显（如上图）。

     所以可以通过把一个场景切分，用多张深度图来储存深度。

     ![img](https://i-msdn.sec.s-msft.com/dynimg/IC613284.png)

     以下的内容主要参考<http://ogldev.atspace.co.uk/www/tutorial49/tutorial49.html>，但是实现的效果并不好，可能是自己哪里写错了，看了好久都没看出来。先记录一下便于以后继续更改。

     ① 先把场景根据深度分为三个cascade

     ```c++
     const int NUM_CASCADES = 3;
     float m_cascadeEnd[NUM_CASCADES + 1];
     
     m_cascadeEnd[0] = 1.0f; // 近平面
     m_cascadeEnd[1] = 25.0f,
     m_cascadeEnd[2] = 70.0f,
     m_cascadeEnd[3] = 100.5f; // 远平面
     ```

     ② 计算每个cascade的八个顶点（较近的平面的4个+较远平面的4个）

     ![img](http://ogldev.atspace.co.uk/www/tutorial49/frustum1.png)

     ```c++
     float ar = Height / Width;
     float tanHalfHFOV = tanf(glm::radians(FOV / 2.0f));
     float tanHalfVFOV = tanf(glm::radians((FOV * ar)/ 2.0f));
     
     float xn = m_cascadeEnd[i] * tanHalfHFOV; // x of near plane
     float xf = m_cascadeEnd[i + 1] * tanHalfHFOV; // x of far plane
     float yn = m_cascadeEnd[i] * tanHalfVFOV;
     float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;
     cout << "xn " << xn << " xf " << xf << " yn " << yn << " yf " << yf << endl;
     glm::vec4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
     	// near face
     	glm::vec4(xn, yn, m_cascadeEnd[i], 1.0),
     	glm::vec4(-xn, yn, m_cascadeEnd[i], 1.0),
     	glm::vec4(xn, -yn, m_cascadeEnd[i], 1.0),
     	glm::vec4(-xn, -yn, m_cascadeEnd[i], 1.0),
     
     	// far face
     	glm::vec4(xf, yf, m_cascadeEnd[i + 1], 1.0),
     	glm::vec4(-xf, yf, m_cascadeEnd[i + 1], 1.0),
     	glm::vec4(xf, -yf, m_cascadeEnd[i + 1], 1.0),
     	glm::vec4(-xf, -yf, m_cascadeEnd[i + 1], 1.0)
     
     };
     ```

     ③ 计算对每个cascade计算可以框住它的框（bounding  box）的xmin，ymin，xmax，ymax，zmin，zmax值（要先把顶点坐标变换到光坐标系下）

     ```c++
     glm::vec4 frustumCornersL[NUM_FRUSTUM_CORNERS];
     
     float minX = std::numeric_limits<float>::max();
     float maxX = std::numeric_limits<float>::min();
     float minY = std::numeric_limits<float>::max();
     float maxY = std::numeric_limits<float>::min();
     float minZ = std::numeric_limits<float>::max();
     float maxZ = std::numeric_limits<float>::min();
     
     for (int j = 0; j < NUM_FRUSTUM_CORNERS; j++) {
     	// 变回到世界坐标
     	glm::vec4 vW = camInv * frustumCorners[j];
     	
         // 再变到light space下
     	frustumCornersL[j] = lightViewMatrix * vW;
     
     	// 取边界
     	minX = min(minX, frustumCornersL[j].x);
     	maxX = max(maxX, frustumCornersL[j].x);
     	minY = min(minY, frustumCornersL[j].y);
     	maxY = max(maxY, frustumCornersL[j].y);
     	minZ = min(minZ, frustumCornersL[j].z);
     	maxZ = max(maxZ, frustumCornersL[j].z);
     }
     ```

     把上面的信息储存为正则投影信息，之后渲染cascade的深度图的时候要用到。

     ```c++
     orthoArray[i].r = maxX;
     orthoArray[i].l = minX;
     orthoArray[i].b = minY;
     orthoArray[i].t = maxY;
     orthoArray[i].f = maxZ;
     orthoArray[i].n = minZ;
     ```

     ④ 创建三个深度贴图

     ```c++
     unsigned int depthMapFBO_CSM;
     glGenFramebuffers(1, &depthMapFBO_CSM);
     
     unsigned int depthMap_CSM[3];
     glGenTextures(3, depthMap_CSM);
     for (int i = 0; i < 3; i++) {
     	glBindTexture(GL_TEXTURE_2D, depthMap_CSM[i]);
     	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
     	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
     	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
     	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
     	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
     }
     
     glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_CSM);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_CSM[0], 0); 
     ```

     ⑤ 渲染深度图+渲染场景

     ```c++
     /// csm
     glm::mat4 lightSpaceMatrix_CSM[NUM_CASCADES];
     simpleDepthShader.Use();
     GLuint lightSpaceMatrixLoc = glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix");
     
     
     lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
     CalcOrthoProjs(myCamera, lightView, (float)HEIGHT, (float)WIDTH, 90.0f);
     
     // 使用计算出的三个cascade做ortho投影
     for (int i = 0; i < NUM_CASCADES; i++) {
     	lightProjection = glm::ortho(orthoArray[i].l, orthoArray[i].r, orthoArray[i].b, orthoArray[i].t, orthoArray[i].n, orthoArray[i].f);
     
     	lightSpaceMatrix = lightProjection * lightView;
     
     	// 把每一个部分的lightSpaceMatrix储存在lightSpaceMatrix_CSM里面
     	lightSpaceMatrix_CSM[i] = lightSpaceMatrix;
     
     	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
     	
     	// bind
     	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
     	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_CSM); 
     	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_CSM[i], 0);
     	
     	glClear(GL_DEPTH_BUFFER_BIT);
         
     	renderScene(simpleDepthShader);
     	
     	glBindFramebuffer(GL_FRAMEBUFFER, 0);
     }
     
     // RESET VIEWPORT
     glViewport(0, 0, WIDTH, HEIGHT);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     
     shadowShader.Use();
     
     glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 100.0f);
     glm::mat4 view = myCamera.getViewMatrix();
     glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
     glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
     
     glUniform1fv(glGetUniformLocation(shadowShader.Program, "gCascadeEndClipSpace"), 4, m_cascadeEnd);
     // light uniforms
     glUniform3fv(glGetUniformLocation(shadowShader.Program, "lightPos"), 1, &lightPos[0]);
     glUniform3fv(glGetUniformLocation(shadowShader.Program, "viewPos"), 1, glm::value_ptr(myCamera.getCameraPos()));
     
     // 激活texture
     //glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM"), NUM_CASCADES, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[0])); 
     glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[0]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[0]));
     glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[1]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[1]));
     glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[2]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[2]));
     
     // bias
     GLuint biasLoc = glGetUniformLocation(shadowShader.Program, "bias");
     glUniform1f(biasLoc, bias);
     
     // pcf
     GLuint pcfLoc = glGetUniformLocation(shadowShader.Program, "pcf_flag");
     glUniform1i(pcfLoc, pcf_flag);
     
     // over sample
     GLuint overSampleLoc = glGetUniformLocation(shadowShader.Program, "over_sample_flag");
     glUniform1i(overSampleLoc, deal_with_over_sample_flag);
     ```

3. GUI

   - 设置了切换投影方式的菜单

     ```c++
     ImGui::TextWrapped("Please select a projection model");
     const char* names2[] = { "Ortho", "Perspective" };
     if (ImGui::BeginPopup("my_select_popup2"))
     {
     	ImGui::Text("Projection model");
     	ImGui::Separator();
     	for (int i = 0; i < IM_ARRAYSIZE(names2); i++)
     		if (ImGui::Selectable(names2[i])) {
     			selected_shadow = i;
     		}
     	ImGui::EndPopup();
     }
     if (ImGui::Button("Select projection model.."))
     	ImGui::OpenPopup("my_select_popup2");
     ImGui::SameLine();
     ImGui::TextUnformatted(selected_shadow == -1 ? "<None>" : names2[selected_shadow]);
     ```

   - slide bar 用于修改阴影偏移

     ```c++
     ImGui::SliderFloat("bias", &bias, 0.0f, 1.0f);
     ```

   - 按钮 选择是否使用特定的阴影优化

     ```c++
     if (ImGui::Button("Front Culling")) {
     	front_culling_flag = !front_culling_flag;
     }
     if (ImGui::Button("PCF")) {
     	pcf_flag = abs(1 - pcf_flag);
     }
     if (ImGui::Button("Deal with over sample")) {
     	deal_with_over_sample_flag = abs(1 - deal_with_over_sample_flag);
     }
     if (ImGui::Button("CSM")) {
     	csm_flag = abs(1 - csm_flag);
     }
     if (ImGui::Button("debug")) {
     	debug = !debug;
     }
     ```

     

   

   

   



