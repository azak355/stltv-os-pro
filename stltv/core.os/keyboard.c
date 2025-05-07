
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// A basic function to send HTTP GET request
void send_http_get(const char *host, const char *path) {
    int socket_fd = create_tcp_connection(host, 80);  // TCP connection to host on port 80
    if (socket_fd < 0) {
        printf("Connection failed\n");
        return;
    }

    // Construct HTTP GET request
    char request[512];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    
    // Send the request
    send(socket_fd, request, strlen(request), 0);
    
    // Read the response
    char response[1024];
    int bytes_received = recv(socket_fd, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';
    
    printf("Response: %s\n", response);

    close(socket_fd);
}
// Example function to parse an HTML string (not a full implementation)
void parse_html(const char* html) {
    char tag[64];
    const char* cursor = html;
    
    while (*cursor) {
        // Find start of the tag
        if (*cursor == '<') {
            cursor++;
            int i = 0;
            // Read tag name
            while (*cursor != ' ' && *cursor != '>' && *cursor != '\0') {
                tag[i++] = *cursor++;
            }
            tag[i] = '\0';
            handle_html_tag(tag);
        }
        cursor++;
    }
}

void handle_html_tag(const char* tag) {
    // Simple example of handling tags
    if (strcmp(tag, "html") == 0) {
        // Handle <html> tag
    } else if (strcmp(tag, "body") == 0) {
        // Handle <body> tag
    }
}
void render_html_element(const char* tag, const char* content) {
    if (strcmp(tag, "h1") == 0) {
        render_text(content, FONT_SIZE_LARGE);
    } else if (strcmp(tag, "p") == 0) {
        render_text(content, FONT_SIZE_NORMAL);
    }
}

void render_text(const char* text, int font_size) {
    // Output text to screen (e.g., using framebuffer or SDL)
    framebuffer_draw_text(text, 10, 10);
}
#include <quickjs.h>

void execute_js(const char* script) {
    JSRuntime *rt = JS_NewRuntime();
    JSContext *ctx = JS_NewContext(rt);
    
    // Execute script
    JS_Eval(ctx, script, strlen(script), "<input>", JS_EVAL_TYPE_GLOBAL);
    
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}
