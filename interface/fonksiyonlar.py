from tkinter import simpledialog
import serial_seyleri
import ayar_ayarlayan

# PPR değerini gönder
def set_ppr():
    ppr_value = simpledialog.askinteger("PPR Değeri", "PPR değerini girin:")
    if ppr_value is not None:
        ayar_ayarlayan.ayar_degistir("ppr",ppr_value)
        ayar_ayarlayan.ayarlari_kaydet()
    
# Buton pinlerini ayarla
def set_button_pins():
    button_pins = simpledialog.askstring("Buton Pinleri", "Buton pinlerini virgülle ayırarak girin:")
    if button_pins is not None:
        pin_list = button_pins.split(',')
        ayar_ayarlayan.ayar_degistir("butons",pin_list)
        ayar_ayarlayan.ayarlari_kaydet()

# Açı aralığını ayarla
def set_angle_range():
    max_angle = simpledialog.askinteger("Maksimum Açı", "Maksimum açıyı girin:")
    if max_angle is not None:
        minval = int(-(max_angle/2))
        maxval = int((max_angle/2))
        ayar_ayarlayan.ayar_degistir("aci",[minval,maxval,max_angle])
        ayar_ayarlayan.ayarlari_kaydet()