import cv2
import numpy as np
from PIL import Image

# Load the background image and resize to 1920x1080 pixels
background_img = cv2.imread('cloudcity_with_road_overlay.jpg')
background_img = cv2.resize(background_img, (1920, 1080))

# Load the drone image with transparency and resize it
drone_img = cv2.imread('drone-7844660.png', cv2.IMREAD_UNCHANGED)
drone_img = cv2.resize(drone_img, (20, 20))

# Define the road path coordinates
road_path = np.array([
    (470, 1080), (485, 1050), (493, 1035), (544, 1023), (619, 1006),
    (743, 970), (833, 938), (926, 907), (1040, 880), (1141, 855),
    (1220, 832), (1303, 812), (1354, 793), (1400, 774), (1405, 761)
])

# Light beam parameters
beam_color = (255, 255, 150)  # Soft yellow color for light
beam_width = 15
num_frames = 50  # Number of frames for the GIF
frames = []

# Initialize drone positions and movements
num_drones = 10
drone_positions = []
for _ in range(num_drones):
    start_x = np.random.randint(0, background_img.shape[1] - 20)
    start_y = np.random.randint(0, background_img.shape[0] - 20)
    direction_x = np.random.choice([-1, 1]) * np.random.uniform(0.5, 1.5)  # Adjust speed
    direction_y = np.random.uniform(-0.5, 0.5)
    drone_positions.append([start_x, start_y, direction_x, direction_y])

# Generate frames with moving light beams and drones
for frame_idx in range(num_frames):
    # Copy the background image for the current frame
    frame = background_img.copy()

    # Draw the light beam along the road path
    for i in range(len(road_path) - 1):
        start_point = road_path[i]
        end_point = road_path[(i + frame_idx) % len(road_path)]
        alpha = max(0, (len(road_path) - i) / len(road_path))  # Beam fade effect

        # Draw the light beam segment
        overlay = frame.copy()
        cv2.line(overlay, start_point, end_point, beam_color, beam_width)
        frame = cv2.addWeighted(overlay, alpha, frame, 1 - alpha, 0)

    # Update and draw drones
    for drone in drone_positions:
        x, y, dx, dy = drone

        # Update drone position
        x += dx
        y += dy

        # Check boundaries and reverse direction if necessary
        if x < 0 or x > background_img.shape[1] - 20:
            dx *= -1
        if y < 0 or y > background_img.shape[0] - 20:
            dy *= -1

        # Update drone data
        drone[0], drone[1], drone[2], drone[3] = x, y, dx, dy

        # Rotate the drone image based on direction
        drone_img_rotated = drone_img
        if dx < 0:
            drone_img_rotated = cv2.flip(drone_img, 1)  # Flip horizontally

        # Place the drone on the frame
        x_int, y_int = int(x), int(y)
        if drone_img_rotated.shape[2] == 4:  # If image has alpha channel
            alpha_channel = drone_img_rotated[:, :, 3] / 255.0
            for c in range(3):  # For each color channel
                frame[y_int:y_int + drone_img_rotated.shape[0], x_int:x_int + drone_img_rotated.shape[1], c] = (
                    alpha_channel * drone_img_rotated[:, :, c] +
                    (1 - alpha_channel) * frame[y_int:y_int + drone_img_rotated.shape[0], x_int:x_int + drone_img_rotated.shape[1], c]
                )
        else:
            frame[y_int:y_int + drone_img_rotated.shape[0], x_int:x_int + drone_img_rotated.shape[1]] = drone_img_rotated

    # Apply Gaussian Blur to the frame
    frame = cv2.GaussianBlur(frame, (5, 5), 0)

    # Convert the frame to RGB and add to the frames list
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frames.append(Image.fromarray(frame_rgb))

# Save the frames as a GIF animation
frames[0].save('combined_animation_drones_and_light_beam_with_effects.gif', save_all=True, append_images=frames[1:], duration=100, loop=0)
print("Combined animation with drones and light beam saved as 'combined_animation_drones_and_light_beam_with_effects.gif'.")
