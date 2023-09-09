#include "stdio.h"
#include "fir_hls.h"

// FIR filter implementation
void fir(const float input[], float output[]) {
    // Initialize shift register
    static float shift_reg[NUM_TAPS] = {0}; // Initialize to zero

    // Main computation loop
    for (int j = 0; j < SIZE; j++) {
        #pragma HLS pipeline II=1

        float acc = 0;

        // FIR filter computation
        for (int i = NUM_TAPS - 1; i > 0; i--) {
            shift_reg[i] = shift_reg[i - 1];
            acc += shift_reg[i] * taps[i];
        }

        // Update shift register and calculate output
        acc += input[j] * taps[0];
        shift_reg[0] = input[j];
        output[j] = acc;
    }
}

// HLS interface pragmas
#pragma HLS INTERFACE m_axi port=input bundle=gmem0 offset=slave depth=1024
#pragma HLS INTERFACE m_axi port=output bundle=gmem1 offset=slave depth=1024
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=0
