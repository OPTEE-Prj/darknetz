import os
import json

def convert_coco_to_yolo(coco_json_path, output_dir):
    with open(coco_json_path, 'r') as f:
        coco_data = json.load(f)

    # Create output directory if not exists
    os.makedirs(output_dir, exist_ok=True)

    # Mapping for category IDs to ensure it's 0-indexed
    category_map = {cat['id']: idx for idx, cat in enumerate(coco_data['categories'])}

    # Process each annotation
    for annotation in coco_data['annotations']:
        image_id = annotation['image_id']
        bbox = annotation['bbox']
        category_id = annotation['category_id']

        # YOLO format: [class_id, x_center, y_center, width, height]
        x_min, y_min, width, height = bbox
        x_center = x_min + width / 2
        y_center = y_min + height / 2

        # Normalize values to be between 0 and 1
        image_info = next(img for img in coco_data['images'] if img['id'] == image_id)
        img_width = image_info['width']
        img_height = image_info['height']
        x_center /= img_width
        y_center /= img_height
        width /= img_width
        height /= img_height

        # Generate YOLO annotation
        yolo_annotation = f"{category_map[category_id]} {x_center} {y_center} {width} {height}\n"

        # Write to corresponding txt file
        image_filename = os.path.splitext(image_info['file_name'])[0]
        annotation_path = os.path.join(output_dir, f"{image_filename}.txt")
        with open(annotation_path, 'a') as file:
            file.write(yolo_annotation)

if __name__ == "__main__":
    # Paths
    coco_json_path = "/mnt/host/share/mjcho/data/annotations/instances_train2017.json"
    output_dir = "/mnt/host/share/mjcho/data/train2017/"

    # Convert
    convert_coco_to_yolo(coco_json_path, output_dir)
