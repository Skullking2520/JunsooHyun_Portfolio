import cv2

# Load the final cropped road image
final_cropped_road_img = cv2.imread('extracted_road_with_railing_final_wide.jpg')

# Get the current dimensions
height, width = final_cropped_road_img.shape[:2]

# Calculate the new height to maintain aspect ratio
new_width = 960
aspect_ratio = new_width / width
new_height = int(height * aspect_ratio)

# Resize the image
resized_img = cv2.resize(final_cropped_road_img, (new_width, new_height), interpolation=cv2.INTER_LINEAR)

# Save the resized image
cv2.imwrite('resized_road_with_railing_960.jpg', resized_img)
print("Resized image saved as 'resized_road_with_railing_960.jpg'.")
