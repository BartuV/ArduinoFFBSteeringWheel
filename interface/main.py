import serial
from serial.tools import list_ports
import tkinter as tk
from tkinter import ttk,simpledialog,messagebox
from struct import pack

# Mevcut seri portları bul
def available_ports():
    return [port.device for port in list_ports.comports()]

def set_port(port):
    global ser
    ser = serial.Serial(port, 9600, timeout=1)
    messagebox.showinfo("Başarı",f"{port} portu seçildi ve ayarlandı.")

def not_connected():
    messagebox.showerror("Hata","Arduino bağlı değil")

# Kullanıcıdan alınan değerleri Arduino'ya gönder
def send_values(command, values):
    if not ser.writable() or ser == None: 
        not_connected()
        return
    ser.write(command.encode() + b':' + pack('<' + 'h'*len(values), *values) + b'\n')

# PPR değerini gönder
def set_ppr():
    if not ser.writable() or ser == None: 
        not_connected()
        return
    ppr_value = simpledialog.askinteger("PPR Değeri", "PPR değerini girin:")
    if ppr_value is not None:
        send_values('0', [ppr_value])

# Buton pinlerini ayarla
def set_button_pins():
    if not ser.writable() or ser == None:  
        not_connected()
        return
    button_pins = simpledialog.askstring("Buton Pinleri", "Buton pinlerini virgülle ayırarak girin:")
    if button_pins is not None:
        pin_list = button_pins.split(',')
        send_values('1', pin_list)

# Açı aralığını ayarla
def set_angle_range():
    if not ser.writable() or ser == None: 
        not_connected()
        return
    max_angle = simpledialog.askinteger("Maksimum Açı", "Maksimum açıyı girin:")
    if max_angle is not None:
        minval = int(-(max_angle/2))
        maxval = int((max_angle/2))
        send_values('2', [minval,maxval])
        
def set_button_encoder():
    if not ser.writable() or ser == None: 
        not_connected()
        return
    

# GUI'yi başlat
root = tk.Tk()
root.title("Arduino Kontrol Arayüzü")

# Pencereyi ortalay
root.geometry("+%d+%d" % (root.winfo_screenwidth() // 2 - root.winfo_width() // 2,
                           root.winfo_screenheight() // 2 - root.winfo_height() // 2))

# PPR ayarlama butonu
ppr_button = tk.Button(root, text="PPR Ayarla", command=set_ppr)

# Buton pinleri ayarlama butonu
button_pins_button = tk.Button(root, text="Buton Pinlerini Ayarla", command=set_button_pins)

# Açı aralığı ayarlama butonu
angle_range_button = tk.Button(root, text="Açı Aralığını Ayarla", command=set_angle_range)

# Encoder ayak
button_encoder_button = tk.Button(root, text="Buton Encoderini Ayarla", command=set_button_encoder)

# Pencere rengini ayarla
root.configure(background="#ffffff")

# Butonları ortalay
ppr_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)
angle_range_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)
button_pins_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)
button_encoder_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)

menubar = tk.Menu(root)
com_menu = tk.Menu(menubar,tearoff=0)

for i in available_ports():
    com_menu.add_command(label="Port "+str(i),command=lambda: set_port(i))

menubar.add_cascade(label="COM", menu=com_menu)

root.config(menu = menubar)

# GUI'yi çalıştır
root.mainloop()