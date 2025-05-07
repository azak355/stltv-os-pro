void kernel_main(void) {
    // Basic output to framebuffer
    // Init memory
    // Init devices
    // Start scheduler or main loop
}
uint32_t* fb = (uint32_t*) FRAMEBUFFER_ADDR;
fb[0] = 0xFF0000; // Red pixel
//network
#define NIC_BASE 0xFE000000
volatile uint32_t* nic_regs = (volatile uint32_t*) NIC_BASE;
while (packet_ready()) {
    uint8_t* pkt = nic_read_packet();
    size_t len = nic_packet_length();
    net_process_packet(pkt, len); // Pass to your stack
}
void nic_send_packet(uint8_t* data, size_t len) {
    write_to_tx_buffer(data, len);
    trigger_transmit();
}
#define RTL_IOBASE 0xC000    // Example I/O port base

void rtl8139_init() {
    outb(RTL_IOBASE + 0x37, 0x10); // Software reset
    while (inb(RTL_IOBASE + 0x37) & 0x10); // Wait for reset

    // Init RX buffer, set registers
    // Enable RX + TX
}

void rtl8139_send(uint8_t* data, size_t len) {
    // Copy to TX buffer, write length, start TX
}

void rtl8139_poll() {
    // Check ISR, read packets from RX buffer
    // Call your net_process_packet()
}
struct netif my_netif;

err_t myif_output(struct netif* netif, struct pbuf* p) {
    for (each pbuf segment) {
        nic_send_packet(p->payload, p->len);
    }
    return ERR_OK;
}

err_t myif_init(struct netif* netif) {
    netif->hwaddr_len = 6;
    memcpy(netif->hwaddr, my_mac, 6);
    netif->mtu = 1500;
    netif->output = etharp_output;
    netif->linkoutput = myif_output;
    return ERR_OK;
}
uint16_t status = read_phy_register(PHY_STATUS_REG);
if (status & LINK_UP) {
    // Good to go
}

send_message("power", "reboot", NULL);
while (1) {
    render_menu(true);
    int key = get_input(); // from remote or keyboard
    if (key == KEY_OK || key == KEY_ENTER) {
        install_and_reboot("/mnt/usb/update.tvup");
    }
}
#define IR_PIN 17  // GPIO pin for IR signal

void setup_ir_receiver() {
    gpio_set_mode(IR_PIN, GPIO_INPUT);
    // Add interrupt handling for IR signal detection
}

void ir_interrupt_handler() {
    int ir_code = read_ir_code(IR_PIN);
    handle_ir_code(ir_code);
}

int read_ir_code(int pin) {
    // Logic to decode the IR signal from the pin
    return decoded_code;
}

void handle_ir_code(int code) {
    switch (code) {
        case POWER_KEY:
            handle_power();
            break;
        case UP_KEY:
            handle_up();
            break;
        case DOWN_KEY:
            handle_down();
            break;
        // Add other button mappings
    }
}
enum remote_button { UP, DOWN, OK, BACK, VOLUME_UP, VOLUME_DOWN, POWER };

void handle_remote_input(enum remote_button button) {
    switch (button) {
        case UP:
            move_selection_up();
            break;
        case DOWN:
            move_selection_down();
            break;
        case OK:
            select_option();
            break;
        case BACK:
            go_back();
            break;
        case VOLUME_UP:
            increase_volume();
            break;
        case VOLUME_DOWN:
            decrease_volume();
            break;
        case POWER:
            handle_power();
            break;
    }
}
void ui_loop() {
    while (1) {
        process_remote_input();  // Get remote input

        if (selected_update_button) {
            draw_button(BTN_X, BTN_Y, BTN_W, BTN_H, "System Restart Software Update", true);
        } else {
            draw_button(BTN_X, BTN_Y, BTN_W, BTN_H, "System Restart Software Update", false);
        }

        if (remote_button == OK) {
            install_and_reboot("/mnt/usb/update.tvup");
        }
        
        update_display();
    }
}

