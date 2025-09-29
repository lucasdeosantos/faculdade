import os
import time
import numpy as np
import matplotlib.pyplot as plt
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes

from src import encrypt as cat_encrypt, decrypt as cat_decrypt

FILENAMES = [
    "../data/three_little_pigs.txt",
    "../data/pride_and_prejudice.txt",
    "../data/little_women.txt"
]

CAT_KEY = ""

AES_KEY = get_random_bytes(16)
AES_IV = get_random_bytes(16)

def measure_cat_speed(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        original_data = f.read()

    encryption_start = time.perf_counter()
    encrypted_data = cat_encrypt(CAT_KEY, original_data)
    encryption_end = time.perf_counter()
    encryption_time_ms = (encryption_end - encryption_start) * 1000

    decryption_start = time.perf_counter()
    cat_decrypt(CAT_KEY, encrypted_data)
    decryption_end = time.perf_counter()
    decryption_time_ms = (decryption_end - decryption_start) * 1000

    return encryption_time_ms, decryption_time_ms

def measure_aes_speed(filename):
    with open(filename, 'rb') as f:
        original_data = f.read()

    aes_cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    encryption_start = time.perf_counter()
    encrypted_data = aes_cipher.encrypt(pad(original_data, AES.block_size))
    encryption_end = time.perf_counter()
    encryption_time_ms = (encryption_end - encryption_start) * 1000

    aes_decipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    decryption_start = time.perf_counter()
    unpad(aes_decipher.decrypt(encrypted_data), AES.block_size)
    decryption_end = time.perf_counter()
    decryption_time_ms = (decryption_end - decryption_start) * 1000

    return encryption_time_ms, decryption_time_ms


def generate_chart(file_sizes, cat_times, aes_times, operation_type):
    labels = [f"{size/1024:.2f}" for size in file_sizes]
    
    operation_idx = 0 if operation_type == 'Encryption' else 1
    cat_operation_times = [t[operation_idx] for t in cat_times]
    aes_operation_times = [t[operation_idx] for t in aes_times]
    
    x = np.arange(len(labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 7))
    rects1 = ax.bar(x - width/2, cat_operation_times, width, label='CAT', color='orange')
    rects2 = ax.bar(x + width/2, aes_operation_times, width, label='AES', color='blue')

    ax.set_ylabel('Tempo (ms)')
    ax.set_xlabel('Tamanho do arquivo (kb)')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    ax.bar_label(rects1, padding=3, fmt='%.2f')
    ax.bar_label(rects2, padding=3, fmt='%.2f')

    fig.tight_layout()
    if operation_type == 'Encryption':
        chart_filename = "cifragem.png"
    else:
        chart_filename = "decifragem.png"
    plt.savefig(chart_filename)
    print(f"Chart saved successfully: '{chart_filename}'")


def main():
    cat_results = []
    aes_results = []
    file_sizes = []

    print("Starting performance comparison...")
    for filename in FILENAMES:
        if not os.path.exists(filename):
            print(f"\nWARNING: File not found, skipping: '{filename}'")
            continue
        
        size = os.path.getsize(filename)
        file_sizes.append(size)
        
        cat_time = measure_cat_speed(filename)
        cat_results.append(cat_time)
        
        aes_time = measure_aes_speed(filename)
        aes_results.append(aes_time)
        
        print(f"\nProcessing: {filename} ({size/(1024*1024):.2f} MB)")
        print(f"  [cat] Encryption: {cat_time[0]:.4f} ms | Decryption: {cat_time[1]:.4f} ms")
        print(f"  [AES] Encryption: {aes_time[0]:.4f} ms | Decryption: {aes_time[1]:.4f} ms")

    if not file_sizes:
        print("\nNo files were processed. Please check the filenames in the 'FILENAMES' list.")
        return

    print("\n--- Generating Charts ---")
    generate_chart(file_sizes, cat_results, aes_results, 'Encryption')
    generate_chart(file_sizes, cat_results, aes_results, 'Decryption')

if __name__ == "__main__":
    main()