#include <asm/ports.h>
#include <drivers/ata.h>
#include <drivers/pci.h>
#include <stdio.h>

/* Macro to get the address of PCI configuration */
#define PCI_ADDRESS(bus, device, func, offset)                                 \
    (uint32_t)((PCI_ENABLE) | ((uint32_t)bus << 16) |                          \
               ((uint32_t)device << 11) | ((uint32_t)func << 8) |              \
               (offset & 0xFC))

int num_of_dev = 0;

void pci_init() {
    kprintf("[PCI] Initializing\n");
    pci_probing();
    kprintf("[PCI] Found %d devices\n", num_of_dev);
}

void pci_probing() {
    /* Recursively find all the devices */
    uint8_t header_type = pci_read_field_byte(0, 0, 0, PCI_HEADER_TYPE);
    /* If the bit-8 of the header is 1, that device is multidevice */
    if ((header_type & 0x80) == 0) {
        /* Single device */
        check_bus(0);
    } else {
        /* Multi device */
        /* Scan through all the function */
        for (uint8_t func = 0; func < 8; func++) {
            if (pci_read_field_word(0, 0, func, PCI_VENDOR_ID) !=
                PCI_NO_DEVICE) {
                break;
            }
            check_bus(func);
        }
    }
}

void pci_check_device(uint8_t bus, uint8_t device) {
    uint8_t func = 0;
    uint16_t vendor_id = pci_read_field_word(bus, device, func, PCI_VENDOR_ID);
    if (vendor_id == PCI_NO_DEVICE) {
        return;
    }

    uint8_t class_code, subclass;
    /* Print device info */
    class_code = pci_read_field_byte(bus, device, func, PCI_CLASS_CODE);
    subclass = pci_read_field_byte(bus, device, func, PCI_SUBCLASS);
    num_of_dev++;
    /* Check if this is a bridge */
    check_function(bus, device, func);

    /* IDE controller */
    if (class_code == 0x01 && subclass == 0x01) {
        ata_init(bus, device, func);
    }

    uint8_t header_type =
        pci_read_field_byte(bus, device, func, PCI_HEADER_TYPE);
    /* Check multidevice, scan all the function if it is multidevice */
    if ((header_type & 0x80) != 0) {
        for (uint8_t func = 1; func < 8; func++) {
            if (pci_read_field_word(bus, device, func, PCI_VENDOR_ID) !=
                PCI_NO_DEVICE) {
                /* Print device info */
                class_code =
                    pci_read_field_byte(bus, device, func, PCI_CLASS_CODE);
                subclass = pci_read_field_byte(bus, device, func, PCI_SUBCLASS);
                num_of_dev++;

                if (class_code == 0x01 && subclass == 0x01) {
                    ata_init(bus, device, func);
                }
                check_function(bus, device, func);
            }
        }
    }
}

void check_bus(uint8_t bus) {
    /* Scan through 32 device in a bus */
    for (uint8_t device = 0; device < 32; device++) {
        pci_check_device(bus, device);
    }
}

void check_function(uint8_t bus, uint8_t device, uint8_t func) {
    /* Scan more if it is a bridge */
    uint8_t class_code = pci_read_field_byte(bus, device, func, PCI_CLASS_CODE);
    uint8_t subclass = pci_read_field_byte(bus, device, func, PCI_SUBCLASS);

    /* PCI-PCI bridge class and subclass */
    if (class_code == 0x6 && subclass == 0x4) {
        check_bus(pci_read_field_byte(bus, device, func, PCI_SECONDARY_BUS));
    }
}

uint8_t pci_read_field_byte(uint8_t bus, uint8_t device, uint8_t func,
                            uint8_t offset) {
    uint32_t addr = PCI_ADDRESS(bus, device, func, offset);

    port_dword_out(PCI_CONFIG_ADDRESS_PORT, addr);
    uint8_t result =
        (uint8_t)(port_dword_in(PCI_CONFIG_DATA_PORT) >> ((offset & 0x03) * 8));

    return result;
}

uint16_t pci_read_field_word(uint8_t bus, uint8_t device, uint8_t func,
                             uint8_t offset) {
    uint32_t addr = PCI_ADDRESS(bus, device, func, offset);

    port_dword_out(PCI_CONFIG_ADDRESS_PORT, addr);
    uint16_t result = (uint16_t)(port_dword_in(PCI_CONFIG_DATA_PORT) >>
                                 ((offset & 0x03) * 8));

    return result;
}

uint32_t pci_read_field_dword(uint8_t bus, uint8_t device, uint8_t func,
                              uint8_t offset) {
    uint32_t addr = PCI_ADDRESS(bus, device, func, offset);

    port_dword_out(PCI_CONFIG_ADDRESS_PORT, addr);
    uint32_t result = (uint32_t)(port_dword_in(PCI_CONFIG_DATA_PORT) >>
                                 ((offset & 0x03) * 8));

    return result;
}
