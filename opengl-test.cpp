#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/texture.hpp>
#include <common/shader.hpp>
#include <common/controls.hpp>


GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)


int width = 1024;
int height = 768;


double lastTime = glfwGetTime();
int nbFrames = 0;



int main( void )
{
	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	
	window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
	    fprintf(stderr, "Failed to initialize GLEW\n");
	    return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );

	//printf("Reading imageprdfdf");
	GLuint Texture = loadBMP_custom( "asdf.bmp" );

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
	    -1.0f,-1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, // triangle 1 : end
	     1.0f, 1.0f,-1.0f, // triangle 2 : begin
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f, // triangle 2 : end
	     1.0f,-1.0f, 1.0f, //3
	    -1.0f,-1.0f,-1.0f,
	     1.0f,-1.0f,-1.0f,
	     1.0f, 1.0f,-1.0f, //4
	     1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f, //5
	    -1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f, 1.0f, //6
	    -1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f, //7
	    -1.0f,-1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, //8
	     1.0f,-1.0f,-1.0f,
	     1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f,-1.0f, //9
	     1.0f, 1.0f, 1.0f,
    	 1.0f,-1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, //10
	     1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f,
	     1.0f, 1.0f, 1.0f, //11
	    -1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, //12
	    -1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f
	};

	

	// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
	static const GLfloat g_uv_buffer_data[] = {
	    0.0f, 0.0, //õ1
	    1.0f, 0.0,
	    1.0f, 1.0, 
	    1.0f, 1.0, //õ2
	    0.0f, 0.0,
	    1.0f, 0.0,
	    1.0f, 1.0, //õ3
	    0.0f, 0.0,
	    1.0f, 0.0,
	    1.0f, 1.0, //ö4
	    0.0f, 1.0,
	    0.0f, 0.0,
	    0.0f, 0.0, //ö5
	    1.0f, 1.0,
	    0.0f, 1.0,
	    1.0f, 1.0, //6
	    0.0f, 1.0,
	    0.0f, 0.0,
	    0.0f, 1.0, //7
	    0.0f, 0.0,
	    1.0f, 0.0,
	    1.0f, 1.0, //8
	    0.0f, 0.0,
	    1.0f, 0.0,
	    0.0f, 0.0, //9
	    1.0f, 1.0,
	    0.0f, 1.0,
	    1.0f, 1.0, //10
	    0.0f, 1.0,
	    0.0f, 0.0,
	    1.0f, 1.0, //11
	    0.0f, 0.0,
	    1.0f, 0.0,
	    1.0f, 1.0, //12
	    0.0f, 1.0,
	    1.0f, 0.0//,
	    //0.0f, 0.0, //ei tea
	};


	
	
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);


	

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	
	

	do{
		// Measure speed
     	double currentTime = glfwGetTime();
     	nbFrames++;
     	if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
         	// printf and reset timer
         	printf("%f ms/frame\n", 1000.0/double(nbFrames));
         	nbFrames = 0;
         	lastTime += 1.0;
     	}


	    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		// glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
		// Camera matrix
		//glm::mat4 View = glm::lookAt(
    	//	glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
	    //	glm::vec3(0,0,0), // and looks at the origin
    	//	glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	    //);
		// Model matrix : an identity matrix (model will be at the origin)
		//glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		//glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around


		// Compute the MVP matrix from keyboard and mouse input
   		computeMatricesFromInputs();
    	glm::mat4 ProjectionMatrix = getProjectionMatrix();
    	glm::mat4 ViewMatrix = getViewMatrix();
    	glm::mat4 ModelMatrix = glm::mat4(1.0);
    	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

	    // Draw
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
   			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   			3,                  // size
   			GL_FLOAT,           // type
   			GL_FALSE,           // normalized?
   			0,                  // stride
   			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : uvs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
    		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    		2,                                // size
    		GL_FLOAT,                         // type
    		GL_FALSE,                         // normalized?
    		0,                                // stride
    		(void*)0                          // array buffer offset
		);
		
		// Draw triangle..
		// Draw the triangle !
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);



	    // Swap buffers
	    glfwSwapBuffers(window);
	    glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       	glfwWindowShouldClose(window) == 0 );
}