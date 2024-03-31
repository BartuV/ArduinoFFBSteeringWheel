# Mevcut seri portları bul
import time
import serial
from serial.tools import list_ports
from tkinter import messagebox
import json
import ayar_ayarlayan

connected = False

def available_ports():
    return [port.device for port in list_ports.comports()]

def set_port(port):
    global ser
    global connected
    if connected:
        ser.close()
        connected = False
        messagebox.showinfo("Başarı",f"{port} portu kapatıldı.")
    else:
        ser = serial.Serial(port, 9600, timeout=1)
        while ser.is_open == False: pass
        connected = True
        messagebox.showinfo("Başarı",f"{port} portu seçildi ve ayarlandı.")
        send_values()

# Kullanıcıdan alınan değerleri Arduino'ya gönder
def send_values():
    if not ser.writable() or ser == None: 
        messagebox.showerror("Hata","Arduino bağlı değil")
        return
    ser.flush()
    ser.write(json.dumps(ayar_ayarlayan.ayarlari_al()).encode())
    time.sleep(1)
    sonuc = ser.read_all().decode()
    if sonuc == "":
        messagebox.showinfo("Başarı","Başarılı bir şekilde ayarlar arduinoya atıldı.")
    else:
        messagebox.showerror("Hata",f"Arduinoya ayar atması başarısız oldu. Hata:{sonuc}")