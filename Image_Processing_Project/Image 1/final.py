import cv2
import numpy as np
from PIL import Image, ImageFont, ImageDraw
import random
import math

# 1. Load and Resize the Main Image

# Load the main city image
city_img = cv2.imread('city.jpg', cv2.IMREAD_COLOR)
if city_img is None:
    print("Error: 'city.jpg' not found. Please ensure the image exists in the working directory.")
    exit()

# Resize to 1920x1080
new_width, new_height = 1920, 1080
city_img = cv2.resize(city_img, (new_width, new_height), interpolation=cv2.INTER_AREA)

# 2. Define Hologram Areas

# Initialize hologram overlay
holo_overlay = np.zeros_like(city_img)
holo_color = (255, 182, 193)  # Pink color for hologram effect

# Define hologram positions (scaled for 1920x1080)
# Original positions were based on 800x450; scale accordingly
scale_x = new_width / 800
scale_y = new_height / 450

# Original hologram positions in 800x450 image
original_display1_start = (337, 237)
original_display1_end = (436, 272)
original_display2_start = (547, 203)
original_display2_end = (668, 238)

# Scale positions to 1920x1080
display1_start = (int(original_display1_start[0] * scale_x), int(original_display1_start[1] * scale_y))
display1_end = (int(original_display1_end[0] * scale_x), int(original_display1_end[1] * scale_y))
display2_start = (int(original_display2_start[0] * scale_x), int(original_display2_start[1] * scale_y))
display2_end = (int(original_display2_end[0] * scale_x), int(original_display2_end[1] * scale_y))

# Calculate dimensions of holograms
rect_width1 = display1_end[0] - display1_start[0]
rect_height1 = display1_end[1] - display1_start[1]
rect_width2 = display2_end[0] - display2_start[0]
rect_height2 = display2_end[1] - display2_start[1]

# Draw the first hologram area on the overlay
holo_rect1 = np.array([
    display1_start,
    (display1_start[0] + rect_width1, display1_start[1]),
    (display1_start[0] + rect_width1, display1_start[1] + rect_height1),
    (display1_start[0], display1_start[1] + rect_height1)
], np.int32)
cv2.fillPoly(holo_overlay, [holo_rect1], holo_color)

# Draw the second hologram area on the overlay
holo_rect2 = np.array([
    display2_start,
    (display2_start[0] + rect_width2, display2_start[1]),
    (display2_start[0] + rect_width2, display2_start[1] + rect_height2),
    (display2_start[0], display2_start[1] + rect_height2)
], np.int32)
cv2.fillPoly(holo_overlay, [holo_rect2], holo_color)

# Apply the hologram overlay to the main image with transparency
final_img = cv2.addWeighted(holo_overlay, 0.5, city_img, 0.7, 0)

# 3. Configure Scrolling Text

# Scrolling text settings
ad_text1 = "Welcome to the Future"
ad_text2 = "The Future starts here"
scrolling_speed = 3  # Increased speed for faster scrolling
text_color = (255, 240, 100)  # Bright yellow for neon effect
font_path = "C:/Windows/Fonts/Arial.ttf"  # Adjust path if necessary
frame_count = 200  # Increased for longer animation
frames = []

# Verify font path
try:
    font = ImageFont.truetype(font_path, 30)  # Increased font size for visibility
except IOError:
    print(f"Error: Font file not found at {font_path}. Please check the path.")
    exit()

# 4. Define Light Beam Settings

beam_settings = [
    {"start": (58, 406), "color": (255, 0, 255), "angle_speed": random.uniform(0.5, 2), "angle_range": 30},
    {"start": (169, 408), "color": (0, 255, 255), "angle_speed": random.uniform(0.5, 2), "angle_range": 30},
    {"start": (387, 377), "color": (255, 255, 0), "angle_speed": random.uniform(0.5, 2), "angle_range": 30},
    {"start": (642, 400), "color": (0, 255, 128), "angle_speed": random.uniform(0.5, 2), "angle_range": 30}
]

# Scale beam starting points to 1920x1080
for beam in beam_settings:
    beam["start"] = (
        int(beam["start"][0] * scale_x),
        int(beam["start"][1] * scale_y)
    )

angles = [0, -10, 15, -5]  # Initial angles
directions = [1, -1, 1, -1]  # Movement directions

# 5. Define Functions

def draw_beam(image, start_point, angle, color=(255, 0, 255), min_thickness=2, max_thickness=15, max_alpha=0.02):
    """Draws a dynamic light beam on the image."""
    overlay = image.copy()
    try:
        end_x = int(start_point[0] + start_point[1] * math.tan(math.radians(angle)))
    except:
        end_x = start_point[0]
    end_y = 0  # Beam extends to the top of the image
    num_layers = 100
    for i in range(num_layers):
        layer_alpha = max_alpha * (1 - i / num_layers)
        layer_thickness = min_thickness + int((i / num_layers) * (max_thickness - min_thickness))
        layer_end_x = int(start_point[0] + ((i + 1) / num_layers) * (end_x - start_point[0]))
        layer_end_y = int(start_point[1] - ((i + 1) / num_layers) * (start_point[1] - end_y))
        cv2.line(overlay, start_point, (layer_end_x, layer_end_y), color, layer_thickness)
        image = cv2.addWeighted(overlay, layer_alpha, image, 1 - layer_alpha, 0)
    return image

def overlay_char(frame, char_img, x, y, hologram_polygons):
    """Overlay a transparent character image onto the frame at position (x, y) only if within hologram areas."""
    # Get dimensions
    char_height, char_width, char_channels = char_img.shape

    # Calculate ROI coordinates
    y1, y2 = y - char_height // 2, y + char_height // 2
    x1, x2 = x - char_width // 2, x + char_width // 2

    # Ensure ROI is within frame boundaries
    if y1 < 0 or y2 > frame.shape[0] or x1 < 0 or x2 > frame.shape[1]:
        return frame  # Skip if out of bounds

    # Check if the center point is within any hologram polygon
    center_point = (x, y)
    within_hologram = False
    for poly in hologram_polygons:
        result = cv2.pointPolygonTest(poly, center_point, False)
        if result >= 0:
            within_hologram = True
            break

    if not within_hologram:
        return frame  # Skip overlaying if not within hologram

    roi = frame[y1:y2, x1:x2]

    if char_channels == 4:
        # Split channels
        b, g, r, a = cv2.split(char_img)
        mask = a / 255.0

        # Blend the character with the ROI
        for c in range(0, 3):
            roi[:, :, c] = (mask * char_img[:, :, c] + (1 - mask) * roi[:, :, c])
    else:
        # If no alpha channel, use the mask based on brightness
        char_gray = cv2.cvtColor(char_img, cv2.COLOR_BGR2GRAY)
        _, mask = cv2.threshold(char_gray, 1, 255, cv2.THRESH_BINARY)
        mask_inv = cv2.bitwise_not(mask)
        img_bg = cv2.bitwise_and(roi, roi, mask=mask_inv)
        char_fg = cv2.bitwise_and(char_img, char_img, mask=mask)
        dst = cv2.add(img_bg, char_fg)
        frame[y1:y2, x1:x2] = dst

    return frame

# 6. Define Hologram Polygons for Text Restriction

# Create list of hologram polygons for text overlay restriction
holo_polygons = [holo_rect1, holo_rect2]

# 7. Add Futuristic Effects

# Add a digital grid overlay for a futuristic look
def add_digital_grid(image, grid_size=50, color=(0, 255, 255), thickness=1):
    """Adds a grid overlay to the image."""
    grid_img = image.copy()
    for x in range(0, new_width, grid_size):
        cv2.line(grid_img, (x, 0), (x, new_height), color, thickness)
    for y in range(0, new_height, grid_size):
        cv2.line(grid_img, (0, y), (new_width, y), color, thickness)
    # Blend the grid with the original image
    blended = cv2.addWeighted(grid_img, 0.3, image, 0.7, 0)
    return blended

# Apply the digital grid to the final image
final_img = add_digital_grid(final_img, grid_size=100, color=(0, 255, 255), thickness=2)

# Add a color overlay with a blue tint for a cooler, futuristic feel
def add_color_overlay(image, color=(100, 150, 255), alpha=0.2):
    """Adds a semi-transparent color overlay to the image."""
    overlay = np.full_like(image, color, dtype=np.uint8)
    return cv2.addWeighted(overlay, alpha, image, 1 - alpha, 0)

# Apply the color overlay to the final image
final_img = add_color_overlay(final_img, color=(100, 150, 255), alpha=0.2)

# 8. Generate Frames

for frame_idx in range(frame_count):
    frame = final_img.copy()

    # Calculate text offsets for scrolling
    text_offset_x1 = (frame_idx * scrolling_speed) % (rect_width1 + len(ad_text1) * 30)
    text_offset_x2 = (frame_idx * scrolling_speed) % (rect_width2 + len(ad_text2) * 30)

    # Draw scrolling text for the first hologram
    for i, char in enumerate(ad_text1):
        # Create a transparent image for each character
        char_img_pil = Image.new("RGBA", (40, 50), (0, 0, 0, 0))  # Increased size for better visibility
        draw_char = ImageDraw.Draw(char_img_pil)
        draw_char.text((5, 5), char, font=font, fill=text_color + (255,))
        char_img = np.array(char_img_pil)

        # Calculate character position
        x = display1_start[0] + rect_width1 - text_offset_x1 + i * 30  # Adjust spacing as needed
        y = display1_start[1] + rect_height1 // 2

        # Overlay character onto the frame if within hologram
        frame = overlay_char(frame, char_img, x, y, holo_polygons)

    # Draw scrolling text for the second hologram
    for i, char in enumerate(ad_text2):
        # Create a transparent image for each character
        char_img_pil = Image.new("RGBA", (40, 50), (0, 0, 0, 0))  # Increased size for better visibility
        draw_char = ImageDraw.Draw(char_img_pil)
        draw_char.text((5, 5), char, font=font, fill=text_color + (255,))
        char_img = np.array(char_img_pil)

        # Calculate character position
        x = display2_start[0] + rect_width2 - text_offset_x2 + i * 30  # Adjust spacing as needed
        y = display2_start[1] + rect_height2 // 2

        # Overlay character onto the frame if within hologram
        frame = overlay_char(frame, char_img, x, y, holo_polygons)

    # Add animated light beams to the frame
    for i, settings in enumerate(beam_settings):
        color = settings["color"]
        angle = angles[i]
        start = settings["start"]
        frame = draw_beam(frame, start, angle, color=color)
        angles[i] += directions[i] * settings["angle_speed"]
        # Reverse direction if angle exceeds range
        if abs(angles[i]) > settings["angle_range"]:
            directions[i] *= -1

    # Convert the frame to RGB and add to the frames list
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frames.append(Image.fromarray(frame_rgb))

    # Optional: Print progress
    if (frame_idx + 1) % 20 == 0:
        print(f"Processed {frame_idx + 1}/{frame_count} frames.")

# 9. Save as GIF

if frames:
    frames[0].save(
        'Dream_City_Zenith_1920x1080.gif',
        save_all=True,
        append_images=frames[1:],
        duration=40,  # Reduced duration for faster frame rate
        loop=0  # Loop indefinitely
    )
    print("GIF saved as 'Dream_City_Zenith_1920x1080.gif'")

