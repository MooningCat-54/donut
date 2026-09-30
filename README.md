# ASCII 3D Donut

Sıkıntı anında eğlencesine yazılmış, harici hiçbir grafik kütüphanesi kullanmayan terminal tabanlı küçük bir 3B simit (torus) animasyonu.

---

## Ne Yapıyor?

* Saf **C** dili ile yazıldı; OpenGL veya benzeri bir kütüphaneye ihtiyaç duymaz.
* 3B koordinatları trigonometrik açılarla döndürüp terminal karakterleri (`.,-~:;=!*#$@`) ile ekrana yansıtır.
* Basit bir **Z-Buffer** mantığıyla derinlik kontrolü ve temel yüzey aydınlatması yapar.
* ANSI imleç kodları kullanarak terminalde titreşimsiz ~33 FPS döner.

---

## Derleme ve Çalıştırma

Matematik kütüphanesini (`-lm`) ekleyerek tek komutla derleyip çalıştırabilirsiniz:

```bash
gcc donut.c -o donut -lm
./donut
