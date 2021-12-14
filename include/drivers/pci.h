#ifndef __PCI_H__
#define __PCI_H__

#include <types.h>

/* Header type 0x0 */
#define PCI_VENDOR_ID 0x00 // 2
#define PCI_DEVICE_ID 0x02 // 2
#define PCI_COMMAND 0x04   // 2
#define PCI_STATUS 0x06    // 2

#define PCI_REVISION_ID 0x08 // 1
#define PCI_PROF_IF 0x09     // 1
#define PCI_SUBCLASS 0x0A    // 1
#define PCI_CLASS_CODE 0x0B  // 1

#define PCI_CACHE_LINE_SIZE 0x0C // 1
#define PCI_LATENCY_TIMER 0x0D   // 1
#define PCI_HEADER_TYPE 0x0E     // 1

#define PCI_BIST 0x0F // 1
#define PCI_BAR0 0x10 // 4
#define PCI_BAR1 0x14 // 4
#define PCI_BAR2 0x18 // 4
#define PCI_BAR3 0x1C // 4
#define PCI_BAR4 0x20 // 4
#define PCI_BAR5 0x24 // 4

#define PCI_CARDBUS_CIS_POINTER 0x28  // 4
#define PCI_SUBSYSTEM_VENDOR_ID 0x2C  // 2
#define PCI_SUBSYSTEM_DEVICE_ID 0x2E  // 2
#define PCI_EXPANSION_ROM_BAR 0x30    // 4
#define PCI_CAPABILITIES_POINTER 0x34 // 1
#define PCI_RESERVED1 0x35            // 3
#define PCI_RESERVED2 0x38            // 4

#define PCI_INTERRUPT_LINE 0x3C // 1
#define PCI_INTERRUPT_PIN 0x3D  // 1
#define PCI_MIN_GRANT 0x3E      // 1
#define PCI_MAX_LATENCY 0x3F    // 1

/* Header type 0x1 (PCI-PCI bridge) */
#define PCI_SECONDARY_BUS 0x19 // 1

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#define PCI_NO_DEVICE 0xFFFF

#define PCI_ENABLE 0x80000000

struct pci_driver {
    const char *name;
};

struct pci_device {
    uint16_t vendor_id, device_id;
    uint16_t command, status;
    uint8_t revision_id, prog_if, class_code, subclass;
    uint8_t cache_line_size, latency_timer, header_type, bist;
    uint32_t base_address[6];
    uint32_t cardbus_cis;
    uint16_t subsystem_vendor_id, subsystem_id;
    uint32_t rom_address;
    uint32_t reserved1, reserved2;
    uint8_t interrupt_pin, interrupt_line, max_latency, min_grant;
} __attribute__((packed));

void pci_init();
void pci_probing(); // Probing all the PCI devices
void pci_check_device(uint8_t bus, uint8_t device); // Check all the function of a device
void check_bus(uint8_t bus); // Check all the device in a bus
void check_function(uint8_t bus, uint8_t device, uint8_t func); // Check if a bridge or not

/* PCI config reading */
uint8_t pci_read_field_byte(uint8_t bus, uint8_t device, uint8_t func,
                            uint8_t offset);
uint16_t pci_read_field_word(uint8_t bus, uint8_t device, uint8_t func,
                             uint8_t offset);
uint32_t pci_read_field_dword(uint8_t bus, uint8_t device, uint8_t func,
                              uint8_t offset);
#endif // !__PCI_H__
