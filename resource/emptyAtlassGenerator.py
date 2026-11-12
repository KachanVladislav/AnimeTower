from PIL import Image, ImageDraw

# === НАСТРОЙКИ ===
COLUMNS = 3       # Количество столбцов (тайлов по горизонтали)
ROWS = 1          # Количество строк (тайлов по вертикали)
TILE_WIDTH = 64   # Ширина одного тайла в пикселях
TILE_HEIGHT = 64  # Высота одного тайла в пикселях
FLOOR_LEVEL = 48  # Расстояние от низа тайла до уровня пола

GRID_COLOR = (255, 0, 0, 128)  
FLOOR_COLOR = (0, 255, 0, 128)  
OUTPUT_FILE = f"tile_grid_{ROWS}_{COLUMNS}_{TILE_WIDTH}.png"
# =================

def create_tile_grid():
    img_width = COLUMNS * TILE_WIDTH 
    img_height = ROWS * TILE_HEIGHT

    img = Image.new("RGBA", (img_width, img_height), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    # Рисуем вертикальные линии
    for col in range(1, COLUMNS):
        x = col * TILE_WIDTH
        draw.line([(x, 0), (x, img_height)], fill=GRID_COLOR, width=1)

    # Рисуем горизонтальные линии
    for row in range(ROWS):
        y = (1 + row) * TILE_HEIGHT
        draw.line([(0, y - FLOOR_LEVEL), (img_width, y - FLOOR_LEVEL)], fill=FLOOR_COLOR, width=1)
        draw.line([(0, y), (img_width, y)], fill=GRID_COLOR, width=1)

    img.save(OUTPUT_FILE, "PNG")
    print(f"Готово! Файл '{OUTPUT_FILE}' создан. Размер: {img_width}x{img_height} px.")

if __name__ == "__main__":
    create_tile_grid()

