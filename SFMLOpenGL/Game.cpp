#include <Game.h>
#include <Cube.h>

GLuint	vsid,		// Vertex Shader ID
fsid,		// Fragment Shader ID
progID,		// Program ID
vao = 0,	// Vertex Array ID
vbo,		// Vertex Buffer ID
vib,		// Vertex Index Buffer
to,			// Texture ID 1 to 32
positionID,	// Position ID
colorID,	// Color ID
textureID,	// Texture ID
uvID,		// UV ID
mvpID;		// Model View Projection ID

			//const string filename = ".//Assets//Textures//coordinates.tga";
			//const string filename = ".//Assets//Textures//cube.tga";
			//const string filename = ".//Assets//Textures//grid.tga";
const string filename = ".//Assets//Textures//grid_wip.tga";
//const string filename = ".//Assets//Textures//minecraft.tga";
//const string filename = ".//Assets//Textures//texture.tga";
//const string filename = ".//Assets//Textures//texture_2.tga";
//const string filename = ".//Assets//Textures//uvtemplate.tga";


int width;			// Width of texture
int height;			// Height of texture
int comp_count;		// Component of texture

unsigned char* img_data;		// image data

mat4 mvp, projection, viewLeft, viewRight, model; // Model View Projection

glm::vec3 cameraFront;

Game::Game() :
	window(VideoMode(800, 600),
		"Pseudo VR"),
	firstMouse(true),
	lastX(800.0f / 2.0),
	lastY(600.0f / 2.0),
	diertionX(90.0f),
	diertionY(0.0f)
{
	
}

Game::Game(sf::ContextSettings settings) :
	window(VideoMode(800, 600),
		"Pseudo VR",
		sf::Style::Default,
		settings),
	firstMouse(true),
	lastX(800.0f / 2.0),
	lastY(600.0f / 2.0),
	diertionX(90.0f),
	diertionY(0.0f)
{
	
}

Game::~Game() {}


void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}

			//get the time since last update and restart the clock
			timeSinceLastUpdate += m_clock.restart();
			//update every 60th of a second
			if (timeSinceLastUpdate > timePerFrame)
			{

				if (event.type == Event::Closed)
				{
					isRunning = false;
				}

				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					// Set Model Rotation
					model = rotate(model, -0.01f, glm::vec3(0, 1, 0)); // Rotate
				}

				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					// Set Model Rotation
					model = rotate(model, 0.01f, glm::vec3(0, 1, 0)); // Rotate
				}

				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					// Set Model Rotation
					model = rotate(model, -0.01f, glm::vec3(1, 0, 0)); // Rotate
				}

				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					// Set Model Rotation
					model = rotate(model, 0.01f, glm::vec3(1, 0, 0)); // Rotate
				}


				update();
				render();

				timeSinceLastUpdate = sf::Time::Zero;
			}
		}


	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();

}

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	//Copy UV's to all faces
	for (int i = 1; i < 6; i++)
		memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	glGenVertexArrays(1, &vao); //Gen Vertex Array
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);		//Gen Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Vertices (3) x,y,z , Colors (4) RGBA, UV/ST (2)
	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Gen Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	//Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

	const char* vs_src =
		"#version 400\n\r"
		""
		//"layout(location = 0) in vec3 sv_position; //Use for individual Buffers"
		//"layout(location = 1) in vec4 sv_color; //Use for individual Buffers"
		//"layout(location = 2) in vec2 sv_texel; //Use for individual Buffers"
		""
		"in vec3 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_uv;"
		""
		"out vec4 color;"
		"out vec2 uv;"
		""
		"uniform mat4 sv_mvp;"
		""
		"void main() {"
		"	color = sv_color;"
		"	uv = sv_uv;"
		//"	gl_Position = vec4(sv_position, 1);"
		"	gl_Position = sv_mvp * vec4(sv_position, 1);"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	const char* fs_src =
		"#version 400\n\r"
		""
		"uniform sampler2D f_texture;"
		""
		"in vec4 color;"
		"in vec2 uv;"
		""
		"out vec4 fColor;"
		""
		"void main() {"
		//"	vec4 lightColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); "
		//"	fColor = vec4(0.50f, 0.50f, 0.50f, 1.0f);"
		//"	fColor = texture2D(f_texture, uv);"
		//"	fColor = color * texture2D(f_texture, uv);"
		//"	fColor = lightColor * texture2D(f_texture, uv);"
		//"	fColor = color + texture2D(f_texture, uv);"
		//"	fColor = color - texture2D(f_texture, uv);"
		"	fColor = color;"
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);

	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	//Use Progam on GPU
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			//2D Texture Image
		0,						//Mipmapping Level 
		GL_RGBA,				//GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					//Width
		height,					//Height
		0,						//Border
		GL_RGBA,				//Bitmap
		GL_UNSIGNED_BYTE,		//Specifies Data type of image data
		img_data				//Image Data
		);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	uvID = glGetAttribLocation(progID, "sv_uv");
	textureID = glGetUniformLocation(progID, "f_texture");
	mvpID = glGetUniformLocation(progID, "sv_mvp");


	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
		);


	// Model matrix
	model = mat4(
		1.0f					// Identity Matrix
		);



	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

void Game::update()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Updating...");
#endif
	// Update Model View Projection
	m_mousePos = sf::Mouse::getPosition(window);

	mouseMovement();
	//view = lookAt(vec3(0.0f, 0.0f, 10.0f),	// Camera positon
	//			cameraFront,		// looking dirction
	//			vec3(0.0f, 1.0f, 0.0f)		// up and down
	//			);

	mvp = projection * viewLeft * viewRight * model;


}

void Game::leftViewports()
{
	glViewport(0, 0, 400, 600);
	glLoadIdentity();
	viewLeft = lookAt(vec3(0.0f, 0.0f, 5.0f),	// Camera (x,y,z), in World Space
			cameraFront,	// Camera looking at origin
			vec3(0.0f, 1.0f, 0.0f)	// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
			);
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
}

void Game::rightViewports()
{
	glViewport(400, 0, 400, 600);
	glLoadIdentity();
	viewRight = lookAt(vec3(0.0f, 0.0f, 5.0f),	// Camera (x,y,z), in World Space
		cameraFront,	// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)	// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);

	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//VBO Data....vertices, colors and UV's appended
	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), uvs);

	// Send transformation to shader mvp uniform
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	//Set pointers for each parameter (with appropriate starting positions)
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);

	//Draw Element Arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	leftViewports();
	rightViewports();

	window.display();

	//Disable Arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);	//Delete Shader
	glDeleteBuffers(1, &vbo);	//Delete Vertex Buffer
	glDeleteBuffers(1, &vib);	//Delete Vertex Index Buffer
	stbi_image_free(img_data);		//Free image
}

void Game::mouseMovement()
{
	float xpos = static_cast<float>(m_mousePos.x);
	float ypos = static_cast<float>(m_mousePos.y);

	// get mouse positoin changing
	GLfloat x = xpos - lastX;
	GLfloat y = lastY - ypos; 
	//record the last mouse positon
	lastX = xpos;
	lastY = ypos;

	// move camera direction
	diertionX += -x;
	diertionY += y;

	
	// Make sure that when y direction is out of bounds, screen doesn't get flipped
	if (diertionY > 90.0f)
		diertionY = 90.0f;
	if (diertionY < -90.0f)
		diertionY = -90.0f;

	// Make sure that when y direction is out of bounds, screen doesn't get flipped
	if (diertionX > 180.0f)
		diertionX = 180.0f;
	if (diertionX < 0.0f)
		diertionX = 0.0f;

	// get the camera looking at origin
	glm::vec3 front;
	front.x = cos(glm::radians(diertionX)) * cos(glm::radians(diertionY));
	front.y = sin(glm::radians(diertionY));
	front.z = sin(glm::radians(diertionX)) * cos(glm::radians(diertionY));
	cameraFront = glm::normalize(front);
}