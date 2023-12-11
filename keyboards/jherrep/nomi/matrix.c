// #include "spi_master.h"
#include "quantum.h"
#include "nomi.h"
#include "spi_master.h"
#include "matrix.h"

extern uint8_t g_matrix[ROWS][COLS];

/*
// TODO: print.h doesn't play nicely with nomi.c
void iox_bank_print(uint8_t iox_data[8]) {
    int i = 0;
    uint8_t data = 0;

    for (i = 0; i < NUM_BANKS; i++) {
        data = iox_data[i];
        uprintf("%x ", data);
    }
    print("\n");
}

void print_matrix(void) {
    int i = 0, j = 0;

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            uprintf("%d |", g_matrix[i][j]);
        }
        print("\n");
    }
    print("------------------\n");
}
*/

void clean_matrix(void) {
    int i = 0, j = 0;

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            g_matrix[i][j] = 0;
        }
    }
}

void matrix_init_custom(void) {
    // setPinOutput(SPI_CS_PIN);
    // writePinHigh(SPI_CS_PIN);
    iox_bank_init();
    chThdSleepMicroseconds(10000);      // 10ms
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    uint8_t iox_data[8] = {};
    bool changed = false;

    iox_bank_read(iox_data);
    // iox_bank_print(iox_data);
    iox_data2mtx(iox_data);

    for (uint8_t row = 0; row < ROWS; row++) {
        matrix_row_t tmp = 0;
        matrix_row_t old_row = current_matrix[row];
        for (uint8_t col = 0; col < COLS; col++) {
            tmp  |= g_matrix[row][col] << col;
        }
        if (tmp != old_row) {
            current_matrix[row] = tmp;
            changed = true;
        }
    }

    clean_matrix();

    return changed;
}
