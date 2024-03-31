import json
import serial_seyleri

def default_ayarlar():
    return {
        "aci" : [-450,450,900],
        "ppr" : 1000,
        "butons" : []
    }
    
ayarlar = default_ayarlar()

def ayarlari_yukle():
    # open a file named ayarlar.txt if it not exist create one
    try:
        with open('ayarlar.txt', 'r') as ayarlar_file:
            ayarlar.update(json.load(ayarlar_file))
    except FileNotFoundError:
        with open('ayarlar.txt', 'w') as ayarlar_file:
            json.dump(default_ayarlar(), ayarlar_file)

def ayarlari_kaydet():
    with open('ayarlar.txt', 'w') as ayarlar_file:
        json.dump(ayarlar, ayarlar_file)
        serial_seyleri.send_values()
        
def ayar_degistir(ayar,deger):
    ayarlar[ayar] = deger
    
def ayarlari_al():
    return ayarlar