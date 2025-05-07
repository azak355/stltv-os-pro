if (button_pressed("Restart")) {
    syscall(SYS_REBOOT);
}
int main() {
    draw_ui();
    while (1) {
        handle_input();

        if (selected("Reboot")) {
            syscall(SYS_REBOOT);
        } else if (selected("Update")) {
            install_update("/mnt/usb/update.tvup");
        } else if (selected("Remotes")) {
            start_remote_pairing();
        }

        render_frame();
    }
}
void draw_button(int x, int y, int w, int h, const char* label, bool selected) {
    if (selected) draw_filled_rect(x, y, w, h, COLOR_HIGHLIGHT);
    else draw_rect(x, y, w, h, COLOR_BORDER);

    draw_text_centered(x + w / 2, y + h / 2, label);
}
#define BTN_X 100
#define BTN_Y 300
#define BTN_W 400
#define BTN_H 60

void render_menu(bool highlight_update) {
    clear_screen();
    draw_button(BTN_X, BTN_Y, BTN_W, BTN_H, "System Restart Software Update", highlight_update);
    update_display();
}
lv_obj_t* btn = lv_btn_create(lv_scr_act());
lv_obj_set_size(btn, 300, 50);
lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);

lv_obj_t* label = lv_label_create(btn);
lv_label_set_text(label, "System Restart Software Update");

lv_obj_add_event_cb(btn, update_callback, LV_EVENT_CLICKED, NULL);
void update_callback(lv_event_t* e) {
    install_and_reboot("/mnt/usb/update.tvup");
}
int check_for_update(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}
void apply_update(const char* update_path) {
    // Parse and copy files to their target locations
    show_spinner("Installing...", 0);
    for (int i = 0; i < total_files; ++i) {
        copy_file(update_files[i]);
        show_spinner("Installing...", (i * 100) / total_files);
    }
    sync(); // Flush filesystem changes
    reboot();
}
#define SYS_REBOOT 0x10
syscall(SYS_REBOOT);
void draw_spinner(int angle, int percent) {
    clear_screen();
    draw_circle_outline(CENTER_X, CENTER_Y, RADIUS);
    draw_arc(CENTER_X, CENTER_Y, RADIUS, 0, angle); // % progress
    draw_text(CENTER_X, CENTER_Y + RADIUS + 20, "%d%%", percent);
    update_display();
}
for (int i = 0; i <= 100; i++) {
    draw_spinner((i * 360) / 100, i);
    delay_ms(50);
}
void install_and_reboot(const char* update_path) {
    int total_steps = get_total_files(update_path);
    for (int i = 0; i < total_steps; ++i) {
        copy_update_file(i);
        draw_spinner((i * 360) / total_steps, (i * 100) / total_steps);
        delay_ms(30); // Simulate progress
    }
    sync();
    draw_text(CENTER_X, CENTER_Y + 50, "Rebooting...");
    update_display();
    delay_ms(2000);
    syscall(SYS_REBOOT);
}
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A simple structure for the framebuffer (this will depend on your system)
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t* pixels;
} framebuffer_t;

// Simple BMP loading function (this is just an outline, not complete)
int load_bmp(const char* filename, uint32_t** pixels, uint32_t* width, uint32_t* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return -1; // Error opening file
    }

    fseek(file, 18, SEEK_SET);
    fread(width, sizeof(uint32_t), 1, file);
    fread(height, sizeof(uint32_t), 1, file);

    uint32_t size = (*width) * (*height);
    *pixels = (uint32_t*)malloc(size * sizeof(uint32_t));

    fseek(file, 54, SEEK_SET); // Skip to pixel data
    fread(*pixels, sizeof(uint32_t), size, file);

    fclose(file);
    return 0;
}

// Function to render a background image to the framebuffer
void render_background(framebuffer_t* fb, uint32_t* image_data) {
    for (uint32_t y = 0; y < fb->height; y++) {
        for (uint32_t x = 0; x < fb->width; x++) {
            uint32_t pixel = image_data[y * fb->width + x];
            fb->pixels[y * fb->width + x] = pixel;
        }
    }
}

int main() {
    framebuffer_t fb;
    fb.width = 800;  // Example screen width
    fb.height = 600; // Example screen height
    fb.pixels = (uint32_t*)malloc(fb.width * fb.height * sizeof(uint32_t));

    // Load BMP image for background
    uint32_t* image_data = NULL;
    uint32_t img_width, img_height;
    if (load_bmp("background.bmp", &image_data, &img_width, &img_height) == 0) {
        render_background(&fb, image_data);
    } else {
        printf("Error loading background image.\n");
    }

    // Now, you can render the framebuffer to the screen (depends on your platform)
    // Example: draw the framebuffer to the screen using your specific display method.

    free(fb.pixels);
    free(image_data);

    return 0;
}
//user
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define FILE_PATH "users.txt"  // File to store the user info

// Function to hash a password using SHA256
void hash_password(const char* password, unsigned char* hash_output) {
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, password, strlen(password));
    SHA256_Final(hash_output, &sha256_ctx);
}

// Function to store the username and hashed password in a file
void store_user(const char* username, unsigned char* hashed_password) {
    FILE* file = fopen(FILE_PATH, "a");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "Username: %s\n", username);
    fprintf(file, "Password Hash: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        fprintf(file, "%02x", hashed_password[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

int main() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    unsigned char hashed_password[SHA256_DIGEST_LENGTH];

    // Input the username and password
    printf("Enter username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline character

    // Hash the password
    hash_password(password, hashed_password);

    // Store the username and password hash
    store_user(username, hashed_password);

    printf("User account created successfully!\n");

    return 0;
}
