#include "EclipseMap.h"

using namespace std;

void EclipseMap::initMoonBuffers(){
    glGenVertexArrays(1, &this->moonVAO);
    glBindVertexArray(this->moonVAO);

    glGenBuffers(1, &this->moonEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->moonEBO);
    /*
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle) * triangles.size(),
                &triangles.front(), GL_DYNAMIC_DRAW);
    */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                &indices.front(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &this->moonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->moonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * moonVertices.size(), 
                &moonVertices.front(), GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), (void*) 0);  // position

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), 
                        (void*) sizeof(glm::vec3));  // normal

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), 
                        (void*) (2 * sizeof(glm::vec3)));  // texture coordinates

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    assert(glGetError() == GL_NONE);
}

void EclipseMap::initEarthBuffers(){
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    /*
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle) * triangles.size(),
                &triangles.front(), GL_DYNAMIC_DRAW);
    */

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                &indices.front(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * earthVertices.size(), 
                &earthVertices.front(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), (void*) 0);  // position

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), 
                        (void*) sizeof(glm::vec3));  // normal

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), 
                        (void*) (2 * sizeof(glm::vec3)));  // texture coordinates

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    assert(glGetError() == GL_NONE);
}

void EclipseMap::Render(const char *coloredTexturePath, const char *greyTexturePath, const char *moonTexturePath) {
    // Open window
    GLFWwindow *window = openWindow(windowName, screenWidth, screenHeight);

    // Moon commands
    // Load shaders
    GLuint moonShaderID = initShaders("moonShader.vert", "moonShader.frag");

    initMoonColoredTexture(moonTexturePath, moonShaderID);

    // Set moonVertices
    float horizontal_step = (2 * PI) / horizontalSplitCount;
    float vertical_step = PI / verticalSplitCount;
/*
    float vertices[] = {
        0.5f,  0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f  // top left
    };
    unsigned int indices[] = { // note that we start from 0!
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                         (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);
*/

    // moon's center position
    glm::vec3 moon_center(0.0, (float) MOON_INITIAL_Y, 0.0);

    for (int i=0; i <= verticalSplitCount; i++){
        float beta = i * vertical_step;         // in radians, [0, pi]
        float z = moonRadius * cos(beta);
        float tmp = moonRadius * sin(beta);

        for (int j=0; j <= horizontalSplitCount; j++){
            float alpha = j * horizontal_step;  // in radians, [0 , 2pi]

            float x = tmp * cos(alpha);
            float y = tmp * sin(alpha) + MOON_INITIAL_Y;

            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position - moon_center);
            glm::vec2 texture_coord(((float) j) / horizontalSplitCount, ((float) i) / verticalSplitCount);

            moonVertices.push_back(vertex(position, normal, texture_coord));
        }
    }

    unsigned int u, v;
    for(int i = 0; i < verticalSplitCount; i++){
        u = i * (horizontalSplitCount + 1);
        v = u + horizontalSplitCount + 1;

        for(int j = 0; j < horizontalSplitCount; j++, u++, v++){
            if(i != 0){
                triangles.push_back(triangle(u, v, u+1));

                indices.push_back(u);
                indices.push_back(v);
                indices.push_back(u+1);
            }

            if(i != (verticalSplitCount-1)){
                triangles.push_back(triangle(u+1, v, v+1));

                indices.push_back(u+1);
                indices.push_back(v);
                indices.push_back(v+1);
            }
        }
    }

    // Configure Buffers
    initMoonBuffers();
    
    //World commands
    //Load shaders
    GLuint worldShaderID = initShaders("worldShader.vert", "worldShader.frag");

    initColoredTexture(coloredTexturePath, worldShaderID);
    initGreyTexture(greyTexturePath, worldShaderID);

    // Set worldVertices
    glm::vec3 earth_center(0.0, 0.0, 0.0);

    for (int i=0; i <= verticalSplitCount; i++){
        float beta = i * vertical_step;         // in radians, [0, pi]
        float z = radius * cos(beta);
        float tmp = radius * sin(beta);

        for (int j=0; j <= horizontalSplitCount; j++){
            float alpha = j * horizontal_step;  // in radians, [0 , 2pi]

            float x = tmp * cos(alpha);
            float y = tmp * sin(alpha);

            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position - earth_center);
            glm::vec2 texture_coord(((float) j) / horizontalSplitCount, ((float) i) / verticalSplitCount);

            earthVertices.push_back(vertex(position, normal, texture_coord));
        }
    }
    
    // Configure Buffers
    initEarthBuffers();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 proj = glm::perspective(glm::radians(projectionAngle), aspectRatio, near, far);
    
    // Main rendering loop
    do {
        glViewport(0, 0, screenWidth, screenHeight);

        glClearStencil(0);
        glClearDepth(1.0f);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // TODO: Handle key presses
        handleKeyPress(window);

        // TODO: Manipulate rotation variables
        
        // TODO: Bind textures
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, moonTextureColor);

        // TODO: Use moonShaderID program
        glUseProgram(moonShaderID);

        // TODO: Update camera at every frame

        // TODO: Update uniform variables at every frame
        //model = glm::translate(model, glm::vec3(0.0f, 5.0f, 5.0f));
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition+cameraDirection, cameraUp);

        glm::mat4 MVP = proj * view * model;

        glUniformMatrix4fv(glGetUniformLocation(moonShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

        // TODO: Bind moon vertex array
        glBindVertexArray(moonVAO);

        // TODO: Draw moon object
        glDrawElements(GL_TRIANGLES, moonVertices.size(), GL_UNSIGNED_INT, 0);

        /*************************/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColor);

        // TODO: Use worldShaderID program
        glUseProgram(worldShaderID);

        // TODO: Update camera at every frame

        // TODO: Update uniform variables at every frame
        glUniformMatrix4fv(glGetUniformLocation(worldShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

        // TODO: Bind world vertex array
        glBindVertexArray(VAO);
        
        // TODO: Draw world object
        glDrawElements(GL_TRIANGLES, earthVertices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    // Delete buffers
    glDeleteBuffers(1, &moonVAO);
    glDeleteBuffers(1, &moonVBO);
    glDeleteBuffers(1, &moonEBO);

    // Delete buffers
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
   
    glDeleteProgram(moonShaderID);
    glDeleteProgram(worldShaderID);

    // Close window
    glfwTerminate();
}

void EclipseMap::handleKeyPress(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWwindow *EclipseMap::openWindow(const char *windowName, int width, int height) {
    if (!glfwInit()) {
        getchar();
        return 0;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    glfwSetWindowMonitor(window, NULL, 0, 0, screenWidth, screenHeight, mode->refreshRate);

    if (window == NULL) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 0);

    return window;
}

void EclipseMap::initColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &textureColor);
    cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, textureColor);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
   
    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexColor"), 0);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);
}

void EclipseMap::initGreyTexture(const char *filename, GLuint shader) {
    glGenTextures(1, &textureGrey);
    glBindTexture(GL_TEXTURE_2D, textureGrey);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexGrey"), 1);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);
}

void EclipseMap::initMoonColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &moonTextureColor);
    //cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, moonTextureColor);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);

    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "MoonTexColor"), 2);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);
}
