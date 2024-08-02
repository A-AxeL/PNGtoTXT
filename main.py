from PIL import Image

def convert_image_to_text(image_path, output_width, output_height):
    img = Image.open(image_path).convert('L')
    img = img.resize((output_width, output_height))

    pixels = list(img.getdata())
    pixels = [pixels[i * output_width:(i + 1) * output_width] for i in range(output_height)]  # Convert to 2D list

    mapping = {i: str(i // 26) for i in range(256)}

    text_art = [[mapping[pixel] for pixel in row] for row in pixels]

    return text_art

def save_text_art(text_art, output_file):
    with open(output_file, 'w') as f:
        for row in text_art:
            f.write(''.join(row) + '\n')

text_art = convert_image_to_text('map.png', 20, 20)
save_text_art(text_art, 'map3.txt')
