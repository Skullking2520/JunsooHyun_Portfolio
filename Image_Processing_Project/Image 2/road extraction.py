import cv2
import numpy as np

# Load and resize the original road image
road_img = cv2.imread('road.jpg')
road_img = cv2.resize(road_img, (1920, 1080))

# Define road and railing coordinates based on user input
road_coordinates = np.array([
    [5, 884], [32, 977], [103, 974], [191, 964], [305, 904], [379, 871],
    [485, 826], [587, 782], [675, 742], [783, 708], [880, 697], [945, 670],
    [1022, 683], [1055, 678], [1135, 633], [1173, 605], [1243, 574],
    [1307, 560], [1343, 558], [1389, 533], [1401, 509], [1368, 501],
    [1328, 498], [1267, 496], [1233, 494]
], dtype=np.int32)

# Create an empty mask with the same dimensions as the road image
mask = np.zeros(road_img.shape[:2], dtype=np.uint8)
cv2.fillPoly(mask, [road_coordinates], 255)

# Define color ranges for road, railing, and very wide forest color range
road_lower_bound = np.array([150, 150, 150])
road_upper_bound = np.array([220, 220, 220])

railing_lower_bound = np.array([60, 60, 60])
railing_upper_bound = np.array([200, 200, 200])

# Expanded range to capture various greenish-gray shades
forest_lower_bound = np.array([50, 50, 50])
forest_upper_bound = np.array([130, 130, 120])

# Apply masks for road, railing, and forest
road_color_mask = cv2.inRange(road_img, road_lower_bound, road_upper_bound)
railing_color_mask = cv2.inRange(road_img, railing_lower_bound, railing_upper_bound)
forest_color_mask = cv2.inRange(road_img, forest_lower_bound, forest_upper_bound)

# Combine road and railing masks, and exclude forest
combined_color_mask = cv2.bitwise_or(road_color_mask, railing_color_mask)
final_mask = cv2.bitwise_and(combined_color_mask, combined_color_mask, mask=mask)
final_mask = cv2.bitwise_and(final_mask, cv2.bitwise_not(forest_color_mask))

# Smooth the mask for a more natural blend
smoothed_final_mask = cv2.GaussianBlur(final_mask, (9, 9), 0)

# Extract road and railing area using the final mask
extracted_road_with_railing = cv2.bitwise_and(road_img, road_img, mask=smoothed_final_mask)

# Save the result
cv2.imwrite('extracted_road_with_railing_final_wide.jpg', extracted_road_with_railing)
