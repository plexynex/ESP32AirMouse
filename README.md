# 🖱️ ESP32 Air Mouse

🚀 **ESP32 Air Mouse** adalah proyek yang memungkinkan ESP32 digunakan sebagai mouse nirkabel berbasis gerakan menggunakan sensor **MPU6050**. Proyek ini mendukung tombol klik kiri, klik kanan, scrolling, serta berbagai mode sensitivitas. PCB 6x8

## 📌 Fitur
- 🖱️ **Gerakan Mouse**: Menggunakan sensor **MPU6050** untuk mengontrol kursor dengan gerakan.
- 🎛 **Mode Sensitivitas**: Dapat beralih antara **rendah, sedang, tinggi, dan ultra tinggi** dengan tombol tengah.
- 🏁 **Kalibrasi**: Menekan tombol kalibrasi akan menghentikan gerakan mouse sementara untuk penyetelan ulang.
- 🔄 **Scrolling**: Tombol tambahan untuk scrolling **atas (IO32) dan bawah (IO33)**.
- 💡 **Indikator LED**:
  - ✨ **Berkedip** saat belum terhubung.
  - ✅ **Mati** saat sudah terhubung untuk menghemat daya.
- 🔵 **BLE Mouse**: Menggunakan **Bluetooth Low Energy (BLE)** untuk konektivitas nirkabel ke komputer.

## 🔧 Wiring
| Komponen | ESP32 Pin |
|----------|----------|
| **MPU6050 SDA** | GPIO21 |
| **MPU6050 SCL** | GPIO22 |
| **Klik Kiri** | GPIO25 |
| **Klik Kanan** | GPIO26 |
| **Tombol Kalibrasi** | GPIO23 |
| **Tombol Tengah (Mode Sensitivitas)** | GPIO27 |
| **Scrolling Atas** | GPIO32 |
| **Scrolling Bawah** | GPIO33 |
| **LED Indikator** | GPIO4 |

## 📜 Instalasi
1. 🛠️ **Persiapkan Hardware**
   - ESP32 Board
   - MPU6050 Gyroscope/Accelerometer
   - Push Buttons untuk klik, kalibrasi, dan sensitivitas
   - LED untuk indikator status

2. 🔗 **Hubungkan ESP32 dengan sensor dan tombol sesuai tabel Wiring.**

3. 📝 **Unggah Kode ke ESP32**
   - Gunakan **Arduino IDE** atau **PlatformIO**
   - Install library yang diperlukan: `MPU6050`, `Wire`, `BleMouse`
   - Unggah kode dan jalankan

4. 🔍 **Hubungkan dengan Komputer**
   - Buka **Pengaturan Bluetooth** di komputer
   - Cari dan hubungkan ke **"ESP32 Air Mouse"**

## 🎮 Penggunaan
- **Gerakkan ESP32** untuk mengontrol kursor.
- **Tekan tombol klik kiri (GPIO25) dan klik kanan (GPIO26) untuk interaksi.**
- **Gunakan tombol kalibrasi (GPIO23) untuk menonaktifkan gerakan sementara.**
- **Tekan tombol tengah (GPIO27) untuk mengubah mode sensitivitas.**
- **Gunakan scrolling atas (GPIO32) dan bawah (GPIO33) untuk navigasi halaman.**

## 🚀 Pengembangan Lanjutan
- 🔥 Menambahkan fitur **gestur khusus**
- 🎤 Integrasi **perintah suara**
- 🔋 Optimasi **konsumsi daya** lebih lanjut

📢 **Kontribusi dan saran sangat diterima!** Jika ada pertanyaan, jangan ragu untuk bertanya! 😃

