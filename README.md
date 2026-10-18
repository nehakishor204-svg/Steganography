# Image Steganography in C

A C-based image steganography project that hides secret text files inside BMP images using the **Least Significant Bit (LSB)** technique and retrieves the hidden data through decoding.

## Overview

Steganography is the technique of concealing information within another file or medium so that the presence of the secret data is not immediately obvious.

This project implements encoding and decoding operations using BMP images. During encoding, secret data is embedded into the image's pixel data by modifying the least significant bits. During decoding, the hidden data is extracted and reconstructed into its original form.

## Features

- Hide secret text files inside BMP images.
- Extract hidden data from encoded BMP images.
- Encode and decode data using the LSB technique.
- Validate input files and supported file formats.
- Support custom output filenames for decoded data.
- Preserve the secret file's original contents during encoding and decoding.

## Technologies Used

- **Programming Language:** C
- **Concepts:** File handling, bitwise operations, pointers, structures, command-line arguments
- **Image Format:** BMP

## Project Structure

```text
Steganography/
├── common.h
├── types.h
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── test_encode.c
├── beautiful.bmp
└── secret.txt
```

## Compilation

Compile the project using GCC:

```bash
gcc -Wall -Wextra test_encode.c encode.c decode.c -o a.out
```

## Usage

### 1. Encoding

Hide a secret text file inside a BMP image:

```bash
./a.out -e beautiful.bmp secret.txt stego_img.bmp
```

Arguments:

- `-e` — Encoding operation
- `beautiful.bmp` — Original BMP image
- `secret.txt` — File containing secret data
- `stego_img.bmp` — Output image containing the hidden data

### 2. Decoding

Extract the hidden data from the encoded image:

```bash
./a.out -d stego_img.bmp
```

To specify a custom output filename:

```bash
./a.out -d stego_img.bmp output.txt
```

The extracted data will be saved to the specified text file.

## How It Works

### Encoding

1. Read the original BMP image and secret text file.
2. Validate the input files and determine the secret file size.
3. Embed the secret data into the image's least significant bits.
4. Store the encoded data in the output BMP image.

### Decoding

1. Read the encoded BMP image.
2. Extract the hidden information from the least significant bits.
3. Reconstruct the secret data.
4. Write the extracted content into a text file.

## Testing

The encoding and decoding process was tested using a sample BMP image and a secret text file.

Example verification:

```bash
cmp secret.txt decoded.txt
```

If the command produces no output, the original and decoded files are identical.

## Learning Outcomes

- Understanding bitwise operations and LSB manipulation.
- Working with file pointers and binary file handling.
- Applying modular programming in C.
- Using command-line arguments to control program execution.
- Implementing data hiding and extraction techniques.

## Author

**Neha Kishor**

GitHub: [nehakishor204-svg](https://github.com/nehakishor204-svg)
