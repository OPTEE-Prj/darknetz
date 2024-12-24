import os
import json

def convert_coco_to_yolo(coco_json_path, output_dir, images_dir):
    with open(coco_json_path) as f:
        data = json.load(f)
    
    # Load categories
    categories = {cat['id']: cat['name'] for cat in data['categories']}
    
    # Create output directory
    os.makedirs(output_dir, exist_ok=True)

    # Parse annotations
    annotations = {}
    for ann in data['annotations']:
        img_id = ann['image_id']
        bbox = ann['bbox']
        category_id = ann['category_id']
        
        # YOLO format bbox: [center_x, center_y, width, height]
        x_center = bbox[0] + bbox[2] / 2
        y_center = bbox[1] + bbox[3] / 2
        width = bbox[2]
        height = bbox[3]
        
        # Normalize coordinates
        img_info = next(img for img in data['images'] if img['id'] == img_id)
        img_width = img_info['width']
        img_height = img_info['height']
        x_center /= img_width
        y_center /= img_height
        width /= img_width
        height /= img_height

        # Append annotation
        if img_id not in annotations:
            annotations[img_id] = []
        annotations[img_id].append(f"{category_id - 1} {x_center} {y_center} {width} {height}")

    # Write YOLO annotation files
    for img in data['images']:
        img_id = img['id']
        img_filename = img['file_name']
        txt_filename = os.path.join(output_dir, os.path.splitext(img_filename)[0] + ".txt")
        
        if img_id in annotations:
            with open(txt_filename, "w") as f:
                f.write("\n".join(annotations[img_id]))

if __name__ == "__main__":
    coco_json_path = "/path/to/instances_train2017.json"
    output_dir = "/mnt/host/share/mjcho/data/train2017"
    images_dir = "/mnt/host/share/mjcho/data/train2017"
    convert_coco_to_yolo(coco_json_path, output_dir, images_dir)
