from PIL import Image, ImageDraw, ImageFont
input1_path = 'input.txt'
input2_path = 'output.txt'
cell_size = 50

def parse_input1(file):
    with open(file, 'r') as f:
        lines = f.readlines()
    grid_size = tuple(map(int, lines[0].split()[1:]))
    source = tuple(map(int, lines[1].split()[1:]))
    sink = tuple(map(int, lines[2].split()[1:]))
    num_blocks = int(lines[3].split()[1])
    blocks = []
    for i in range(4, 4 + num_blocks):
        parts = lines[i].split()
        block_x, block_y, block_w, block_h = map(int, parts[1:])
        blocks.append((block_x, block_y, block_w, block_h))
    return grid_size, source, sink, blocks

def parse_input2(file):
    with open(file, 'r') as f:
        lines = f.readlines()
    total_length = int(lines[0].split()[1])
    source = tuple(map(int, lines[1].split()[1:]))
    num_bends = int(lines[2].split()[1])
    bends = [tuple(map(int, line.split())) for line in lines[3:3 + num_bends]][::-1]
    sink = tuple(map(int, lines[-1].split()[1:]))
    return total_length, source, bends, sink

def draw_grid(draw, grid_size):
    font = ImageFont.load_default()
    for i in range(grid_size[0] ):
        draw.line([(i * cell_size , 0), (i * cell_size , grid_size[1] * cell_size)], fill="black")
        draw.text((i * cell_size+ cell_size/2, grid_size[1] * cell_size ), str(i), fill="black", font=font)
    for j in range(grid_size[1]):
        draw.line([(0, j * cell_size), (grid_size[0] * cell_size, j * cell_size)], fill="black")
        draw.text((grid_size[0] * cell_size + 5, j * cell_size +  cell_size/2), str(grid_size[1]-1-j), fill="black", font=font)

def mark_blocks(draw, blocks):
    for block in blocks:
        block_x, block_y, block_w, block_h = block
        x1, y1 = block_x * cell_size, block_y * cell_size
        x2, y2 = (block_x + block_w) * cell_size, (block_y + block_h) * cell_size
        draw.rectangle([x1, y1, x2, y2], fill="gray")

def draw_path(draw, path, bends, highlight=None):
    bend=path[len(path)-1]
    for i in range(len(path) - 1):
        start = (path[i][0] * cell_size + cell_size // 2, path[i][1] * cell_size + cell_size // 2)
        end = (path[i + 1][0] * cell_size + cell_size // 2, path[i + 1][1] * cell_size + cell_size // 2)
        draw.line([start, end], fill="green", width=3)

def draw_bend(draw,bends,bendcode):
    font = ImageFont.load_default()
    for bend in bends:
        if bendcode>0:
            draw.rectangle(
            [bend[0] * cell_size, bend[1] * cell_size, (bend[0] + 1) * cell_size,
             (bend[1] + 1) * cell_size], fill="green")
            draw.text((bend[0]* cell_size + cell_size / 2, bend[1]* cell_size + cell_size / 2), "V", fill="white", font=font)
            bendcode-=1
        else:
            break

def create_frames(grid_size, source, sink, blocks, bends):
    """Create video frames showing the path progression."""
    frames = []
    img_size = (grid_size[0] * cell_size + 50, grid_size[1] * cell_size + 50)

    path = [source]
    current_pos = source
    bendcode=0
    for bend in bends + [sink]:
        img = Image.new("RGB", img_size, "white")
        draw = ImageDraw.Draw(img)
        draw_grid(draw, grid_size)
        mark_blocks(draw, blocks)

        # Update path step-by-step
        while current_pos != bend:
            if current_pos[0] != bend[0]:
                current_pos = (current_pos[0] + (1 if bend[0] > current_pos[0] else -1), current_pos[1])
            elif current_pos[1] != bend[1]:
                current_pos = (current_pos[0], current_pos[1] + (1 if bend[1] > current_pos[1] else -1))
            path.append(current_pos)

            draw_path(draw, path, bends, highlight=current_pos)
            draw_bend(draw, bends, bendcode)
            draw.rectangle(
                [source[0] * cell_size, source[1] * cell_size, (source[0] + 1) * cell_size,
                 (source[1] + 1) * cell_size],fill="yellow")
            draw.rectangle(
                [sink[0] * cell_size, sink[1] * cell_size, (sink[0] + 1) * cell_size,
                 (sink[1] + 1) * cell_size], fill="red")

            frames.append(img.copy())
        bendcode=bendcode+1

    for _ in range(20):
        frames.append(img.copy())
    print("It can take time to complete drawing...")
    return frames

def save_gif(frames, output_path, duration=200):
    print("Loading...")
    frames[0].save(
        output_path,
        save_all=True,
        append_images=frames[1:],
        duration=duration,
        loop=0
    )
    print(f"Please open GIF at: {output_path}")

def main():
    grid_size, source, sink, blocks = parse_input1(input1_path)
    _, _, bends, _ = parse_input2(input2_path)

    source = (source[0], grid_size[1] - source[1] - 1)
    bends = [(x, grid_size[1] - y - 1) for x, y in bends]
    sink = (sink[0], grid_size[1] - sink[1] - 1)
    blocks = [(x, grid_size[1] - y - h, w, h) for x, y, w, h in blocks]

    print("Generating GIF...")
    frames = create_frames(grid_size, source, sink, blocks, bends)
    save_gif(frames, "routing_gif.gif", duration=200)

if __name__ == "__main__":
    main()