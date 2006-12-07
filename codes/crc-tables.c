/*
 * Copyright (C) 2006 Jean-Baptiste Note <jean-baptiste.note@m4x.org>
 *
 * Table lookup dump for the standard CRC algorithms
 */

#include <stdio.h>
#include <stdint.h>

#define POLY_CRC16_IBM 0x8005
#define POLY_CRC16_IBM_REFLEX 0xa001

static uint32_t g[8];
/* Compute the basis polynomials */

static void
crc16_compute_genpolys(unsigned nbits, unsigned poly) {
  unsigned j;
  g[0] = poly;
  for (j = 1; j < nbits; j++) {
    g[j] = g[j-1] >> 1;
    if (g[j-1] & 1)
      g[j] ^= g[0];
    g[j] &= 0xffff;
  }
}

/* dumps a table for CRC16 lut computation */
static void
crc16_dump_table(unsigned nbits, unsigned poly, FILE *out) {
  unsigned nelems = 1 << nbits;
  unsigned i, j;

  crc16_compute_genpolys(nbits, poly);

  for (i = 0; i < nelems; i++) {
    uint16_t f = 0;
    uint16_t T = i;
    for (j = 0; j < nbits; j++) {
      /* LSB first here, so all OK */
      if (T & 1)
	f ^= g[nbits - 1 -j];
      T >>= 1;
    }
    if (i % 8 == 0)
      fprintf(out, "\n");
    fprintf(out, "0x%04x, ", f);
  }
}

int main(int argc, char *argv[], char **env) {
  FILE *out = stdout;
  fprintf(out, "crc_table_ibm[256] = {\n");
  crc16_dump_table(8, POLY_CRC16_IBM_REFLEX, out);
  fprintf(out, "};\n");

  fprintf(out, "crc_table_ibm[32] = {\n");
  crc16_dump_table(5, POLY_CRC16_IBM_REFLEX, out);
  fprintf(out, "};\n");

  return 0;
}