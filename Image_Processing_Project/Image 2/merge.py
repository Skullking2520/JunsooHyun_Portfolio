import cv2
import numpy as np

# Load the cloud city background and resized road image
background_img = cv2.imread('cloudcity.jpg')
road_img = cv2.imread('resized_road_with_railing_960.jpg')

# Resize background to 1920x1080 for consistency
background_img = cv2.resize(background_img, (1920, 1080))

# Define the insertion coordinates
top_left = (480, 750)
bottom_right = (1440, 1080)

# Calculate the overlay dimensions
overlay_width = bottom_right[0] - top_left[0]
overlay_height = bottom_right[1] - top_left[1]

# Resize road image to fit into the defined overlay area
resized_road_img = cv2.resize(road_img, (overlay_width, overlay_height), interpolation=cv2.INTER_LINEAR)

# Create a mask to remove the background from the road image
gray_road = cv2.cvtColor(resized_road_img, cv2.COLOR_BGR2GRAY)
_, road_mask = cv2.threshold(gray_road, 1, 255, cv2.THRESH_BINARY)
road_mask_inv = cv2.bitwise_not(road_mask)

# Extract only the road without the background
road_only = cv2.bitwise_and(resized_road_img, resized_road_img, mask=road_mask)

# Define the region of interest on the background image
background_region = background_img[top_left[1]:top_left[1] + overlay_height, top_left[0]:top_left[0] + overlay_width]

# Mask the region on the background to create a "hole" for the road
background_with_hole = cv2.bitwise_and(background_region, background_region, mask=road_mask_inv)

# Combine the road with the background region
combined = cv2.add(background_with_hole, road_only)

# Place the combined result back onto the background image
background_img[top_left[1]:top_left[1] + overlay_height, top_left[0]:top_left[0] + overlay_width] = combined

# Save the output image
cv2.imwrite('cloudcity_with_road_overlay.jpg', background_img)
print("Road overlay applied and saved as 'cloudcity_with_road_overlay.jpg'.")
