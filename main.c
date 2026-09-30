#include <math.h>
#include <stdio.h>
#include <unistd.h> // usleep için

#define LINES 24
#define COLS 80

int main() {
  char buffer[LINES][COLS];
  float zbuffer[LINES][COLS];
  char luminance_chars[] = ".,-~:;=!*#$@";
  float Ldir[3] = {0, 1, -1};

  // orta nokta
  int orgin_x = COLS / 2;
  int orgin_y = LINES / 2;

  // simitin yarı çapı
  float r2 = 8.0f;
  // kalınlığın yarı çapı
  float r1 = 4.0f;

  // ekran uzaklığı
  float K1 = 15.0f;
  // çizim boyutu
  float K2 = 20.0f;

  // yatay açı
  float A = 0.0f;
  // dikey açı
  float B = 0.0f;

  printf("\x1b[2J");

  while (1) {
    // Her karenin başında buffer'ları temizle
    for (int i = 0; i < LINES; i++) {
      for (int j = 0; j < COLS; j++) {
        buffer[i][j] = ' ';
        zbuffer[i][j] = 0.0f;
      }
    }

    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    for (float theta = 0.0f; theta < M_PI * 2; theta += 0.07f) {
      float R = r2 + r1 * cos(theta);

      for (float phi = 0.0f; phi < M_PI * 2; phi += 0.02f) {
        // Temel simit koordinatları
        float x = R * cos(phi);
        float y = r1 * sin(theta);
        float z = R * sin(phi);

        // İlk rotasyon (A açısı - X ekseni)
        float y1 = y * cosA - z * sinA;
        float z1 = y * sinA + z * cosA;

        // İkinci rotasyon (B açısı - Z ekseni)
        float x2 = x * cosB - y1 * sinB;
        float y2 = x * sinB + y1 * cosB;
        float z2 = z1;

        // Perspektif (z2 kullanıyoruz!)
        float ooz = 1.0f / (K2 + z2);

        // Normalleri aynı iki rotasyondan geçir
        float Nx = cos(theta) * cos(phi);
        float Ny = sin(theta);
        float Nz = cos(theta) * sin(phi);

        float Ny1 = Ny * cosA - Nz * sinA;
        float Nz1 = Ny * sinA + Nz * cosA;

        float Nx2 = Nx * cosB - Ny1 * sinB;
        float Ny2 = Nx * sinB + Ny1 * cosB;
        float Nz2 = Nz1;

        float L = Nx2 * Ldir[0] + Ny2 * Ldir[1] + Nz2 * Ldir[2];

        // Projeksiyon (x2 ve y2 kullanıyoruz!)
        int screen_x = (int)(orgin_x + 2.0f * K1 * ooz * x2 + 0.5f);
        int screen_y = (int)(orgin_y + K1 * ooz * y2 + 0.5f);
        if (screen_x >= 0 && screen_x < COLS && screen_y >= 0 &&
            screen_y < LINES) {
          if (L > 0) {
            int char_index = (int)(L * 8);
            if (char_index > 11)
              char_index = 11;

            if (ooz > zbuffer[screen_y][screen_x]) {
              zbuffer[screen_y][screen_x] = ooz;
              buffer[screen_y][screen_x] = luminance_chars[char_index];
            }
          }
        }
      }
    }

    // İmleci sol üste taşı ve kareyi bas
    printf("\x1b[H");
    for (int i = 0; i < LINES; i++) {
      for (int j = 0; j < COLS; j++) {
        putchar(buffer[i][j]);
      }
      putchar('\n');
    }

    // Açıyı artır ve biraz bekle
    A += 0.04f;
    B += 0.02f;
    usleep(30000); // 30ms (~33 FPS)
  }

  return 0;
}