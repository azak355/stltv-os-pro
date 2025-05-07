SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
SDL_Window *window = SDL_CreateWindow("Custom TV OS",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      1280, 720,
                                      SDL_WINDOW_OPENGL);
SDL_GLContext glContext = SDL_GL_CreateContext(window);

// Initialize OpenGL ES, compile shaders, set up VAO/VBO for UI elements

// Main loop
bool running = true;
while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        // Process input for remote keys
    }
    
    // Clear, render UI elements with animations, and swap buffers
    glClear(GL_COLOR_BUFFER_BIT);
    // Render your UI scene here
    SDL_GL_SwapWindow(window);
}

SDL_GL_DeleteContext(glContext);
SDL_DestroyWindow(window);
SDL_Quit();
void process_remote_input() {
    int ir_code = get_remote_input();  // Read the IR or Bluetooth signal
    switch (ir_code) {
        case IR_UP:
            handle_remote_input(UP);
            break;
        case IR_DOWN:
            handle_remote_input(DOWN);
            break;
        case IR_OK:
            handle_remote_input(OK);
            break;
        // Handle other buttons
    }
}
void navigate_menu() {
    if (remote_button == UP) {
        move_up();
    } else if (remote_button == DOWN) {
        move_down();
    } else if (remote_button == OK) {
        select_option();
    }
}
