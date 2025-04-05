#!/usr/bin/env python3

import os
import subprocess
import time
import argparse

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

def test_axon(axon_path, size_mb):
    """Test Axon with text files."""
    # Create test files
    test_file = f"./axon_test/test_{size_mb}MB.txt"
    encrypted_file = f"./axon_test/test_{size_mb}MB.encrypted.txt"
    decrypted_file = f"./axon_test/test_{size_mb}MB.decrypted.txt"
    
    os.makedirs("./axon_test", exist_ok=True)
    create_text_file(test_file, size_mb)
    
    # Run encryption
    print(f"Encrypting {size_mb}MB file...")
    start_time = time.time()
    subprocess.run([axon_path, test_file, encrypted_file, "password123", "e"], check=True)
    encrypt_time = time.time() - start_time
    print(f"Encryption completed in {encrypt_time:.2f} seconds")
    print(f"Encrypted file size: {os.path.getsize(encrypted_file)} bytes")
    
    # Run decryption
    print(f"Decrypting file...")
    start_time = time.time()
    subprocess.run([axon_path, encrypted_file, decrypted_file, "password123", "d"], check=True)
    decrypt_time = time.time() - start_time
    print(f"Decryption completed in {decrypt_time:.2f} seconds")
    print(f"Decrypted file size: {os.path.getsize(decrypted_file)} bytes")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Test Axon with text files")
    parser.add_argument("--axon", required=True, help="Path to Axon executable")
    parser.add_argument("--size", type=float, default=1.0, help="File size in MB")
    args = parser.parse_args()
    
    test_axon(args.axon, args.size)