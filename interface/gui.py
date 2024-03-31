import tkinter as tk
from fonksiyonlar import *
import serial_seyleri

def start_gui():
    #GUI'yi başlat
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

    # Pencere rengini ayarla
    root.configure(background="#ffffff")

    # Butonları ortalay
    ppr_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)
    angle_range_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)
    button_pins_button.pack(side=tk.LEFT, pady=10, padx=10, fill=tk.X)

    menubar = tk.Menu(root)
    com_menu = tk.Menu(menubar,tearoff=0)
    
    com_menu.add_command(label="Yenile",command=lambda: display_ports())

    for i in serial_seyleri.available_ports():
        com_menu.add_command(label="Port "+str(i),command=lambda: serial_seyleri.set_port(i))
    
    def display_ports():
        com_menu.delete(1,tk.END)
        for i in serial_seyleri.available_ports():
            com_menu.add_command(label="Port "+str(i),command=lambda: serial_seyleri.set_port(i))

    menubar.add_cascade(label="COM", menu=com_menu)

    root.config(menu = menubar)

    # GUI'yi çalıştır
    root.mainloop()