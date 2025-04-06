#!/usr/bin/env python3

import os
import subprocess
import time
import argparse
import matplotlib.pyplot as plt
import numpy as np

def create_text_file(path, size_mb):
    """Create a text file of the specified size in MB."""
    os.makedirs(os.path.dirname(path), exist_ok=True)
    
    print(f"Creating {size_mb}MB text file at {path}")
    with open(path, 'w') as f:
        # Write 1MB chunks at a time
        chunk = "A" * (1024 * 1024 - 1) + "\n"  # 1MB of text with newline
        for _ in range(int(size_mb)):
            f.write(chunk)
            
    print(f"Created file: {path} ({os.path.getsize(path)} bytes)")

def test_axon(axon_path, size_mb, opt_level="auto"):
    """Test Axon with text files using specific optimization level."""
    # Create test files
    test_file = f"./axon_test/test_{size_mb}MB.txt"
    encrypted_file = f"./axon_test/test_{size_mb}MB.encrypted.txt"
    decrypted_file = f"./axon_test/test_{size_mb}MB.decrypted.txt"
    
    os.makedirs("./axon_test", exist_ok=True)
    if not os.path.exists(test_file):
        create_text_file(test_file, size_mb)
    
    # Run encryption with specified optimization level
    print(f"Encrypting {size_mb}MB file with optimization level: {opt_level}...")
    start_time = time.time()
    subprocess.run([axon_path, test_file, encrypted_file, "password123", "e", opt_level], check=True)
    encrypt_time = time.time() - start_time
    print(f"Encryption completed in {encrypt_time:.2f} seconds")
    print(f"Encrypted file size: {os.path.getsize(encrypted_file)} bytes")
    
    # Run decryption with specified optimization level
    print(f"Decrypting file with optimization level: {opt_level}...")
    start_time = time.time()
    subprocess.run([axon_path, encrypted_file, decrypted_file, "password123", "d", opt_level], check=True)
    decrypt_time = time.time() - start_time
    print(f"Decryption completed in {decrypt_time:.2f} seconds")
    print(f"Decrypted file size: {os.path.getsize(decrypted_file)} bytes")
    
    return encrypt_time, decrypt_time

def benchmark_optimization_levels(axon_path, size_mb):
    """Run benchmarks for all optimization levels."""
    levels = [
        ("0", "None"),
        ("1", "SSE2"),
        ("2", "AVX"),
        ("3", "AVX2"),
        ("auto", "Auto")
    ]
    
    results = {}
    
    print(f"\n=== Benchmarking optimization levels with {size_mb}MB file ===\n")
    
    for level_code, level_name in levels:
        print(f"\n--- Testing {level_name} optimization (level {level_code}) ---")
        try:
            enc_time, dec_time = test_axon(axon_path, size_mb, level_code)
            results[level_name] = (enc_time, dec_time)
            print(f"Results for {level_name}: Encryption: {enc_time:.2f}s, Decryption: {dec_time:.2f}s")
        except subprocess.CalledProcessError as e:
            print(f"Error running benchmark with level {level_code}: {e}")
    
    # Print summary
    print("\n=== Benchmark Summary ===")
    print(f"File size: {size_mb}MB")
    print("-" * 60)
    print(f"{'Optimization Level':<20} {'Encryption (s)':<15} {'Decryption (s)':<15} {'Speedup':<10}")
    print("-" * 60)
    
    # Find baseline for speedup calculation
    if "None" in results:
        base_enc, base_dec = results["None"]
    else:
        # Use the first available result as baseline
        first_key = list(results.keys())[0]
        base_enc, base_dec = results[first_key]
        
    for level, (enc_time, dec_time) in results.items():
        enc_speedup = base_enc / enc_time
        print(f"{level:<20} {enc_time:<15.2f} {dec_time:<15.2f} {enc_speedup:<10.2f}x")
    
    # Create visualization
    try:
        create_benchmark_chart(results, size_mb)
    except Exception as e:
        print(f"Error creating chart: {e}")
    
    return results

def create_benchmark_chart(results, size_mb):
    """Create a bar chart comparing the benchmark results."""
    try:
        import matplotlib.pyplot as plt
        import numpy as np
    except ImportError:
        print("Matplotlib not installed. Skipping chart generation.")
        return
        
    levels = list(results.keys())
    enc_times = [results[level][0] for level in levels]
    dec_times = [results[level][1] for level in levels]
    
    x = np.arange(len(levels))  # the label locations
    width = 0.35  # the width of the bars
    
    fig, ax = plt.subplots(figsize=(10, 6))
    rects1 = ax.bar(x - width/2, enc_times, width, label='Encryption')
    rects2 = ax.bar(x + width/2, dec_times, width, label='Decryption')
    
    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Time (seconds)')
    ax.set_title(f'Performance by Optimization Level ({size_mb}MB file)')
    ax.set_xticks(x)
    ax.set_xticklabels(levels)
    ax.legend()
    
    fig.tight_layout()
    
    # Save chart
    output_file = f"axon_benchmark_{size_mb}MB.png"
    plt.savefig(output_file)
    print(f"Benchmark chart saved to: {output_file}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Test Axon with text files")
    parser.add_argument("--axon", required=True, help="Path to Axon executable")
    parser.add_argument("--size", type=float, default=1.0, help="File size in MB")
    parser.add_argument("--opt-level", default="auto", 
                        choices=["0", "1", "2", "3", "auto"],
                        help="Optimization level (0=None, 1=SSE2, 2=AVX, 3=AVX2, auto=Automatic)")
    parser.add_argument("--benchmark", action="store_true", 
                        help="Run benchmarks for all optimization levels")
    args = parser.parse_args()
    
    if args.benchmark:
        benchmark_optimization_levels(args.axon, args.size)
    else:
        test_axon(args.axon, args.size, args.opt_level)